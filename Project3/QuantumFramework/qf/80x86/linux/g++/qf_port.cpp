//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++ port to Linux/P-threads, g++
// Version: Compatible with QF/C++ 3.1.yy
// Updated: Feb 10, 2006
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
//
// This QF/C++ port is part of the Quantum Leaps QF/C++ software, and may be
// distributed and modified under the terms of the GNU General Public License
// version 2 (GPL) as published by the Free Software Foundation and appearing
// in the file GPL.TXT included in the packaging of this file. Please note
// that GPL Section 2[b] requires that all works based on this software must
// also be made publicly available under the terms of the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified in conjunction
// with a valid QF/C++ Quantum Leaps commercial license. Quantum Leaps
// commercial licenses are designed for users who want to retain proprietary
// status of their code. The users who license this software under one of
// Quantum Leaps commercial licenses do not use this software under the GPL
// and therefore are not subject to any of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qep.h"
#include "qassert.h"
#include "qf_port.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>                                        // for mlockall()
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

Q_DEFINE_THIS_MODULE(qf_port)

// Package-scope objects -----------------------------------------------------
pthread_mutex_t QF_pThreadMutex_ = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
uint8_t QF_running_;

// Local objects -------------------------------------------------------------
struct termios l_tsav;             // structure with saved terminal attributes

//............................................................................
const char *QF::getPortVersion(void) {
    return "1.1.05";
}
//............................................................................
void QF::init(void) {
                             // lock memory so we're never swapped out to disk
    //mlockall(MCL_CURRENT | MCL_FUTURE);          // uncomment when supported
    QF_running_ = (uint8_t)1;
    tcgetattr(0, &l_tsav);             // save the current terminal attributes
}
//............................................................................
void QF::start(void) {
    struct sched_param sparam;

                    // try to maximize the priority of this thread, see NOTE01
    sparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sparam) == 0) {
                        // success, this application has sufficient privileges
    }
    else {
            // setting priority failed, probably due to insufficient privieges
    }
}
//............................................................................
void QF::exit(void) {
    QF_running_ = (uint8_t)0;
}
//............................................................................
void QF::run(void) {
    struct timeval timeout = { 0 };                    // timeout for select()

    QF::start();                                               // start the QF

    while (QF_running_) {
        QF_SCHED_LOCK();
        QF::tick();                                     // process a time tick
        QF_SCHED_UNLOCK();
                                           // sleep for the full tick , NOTE06
        timeout.tv_usec = 8000;
        select(0, 0, 0, 0, &timeout);
    }
    tcsetattr(0, TCSANOW, &l_tsav);            // restore the saved attributes
    pthread_mutex_destroy(&QF_pThreadMutex_);
}
//............................................................................
static void *run(void *me) {       // use exactly the expected POSIX signature
    ((QActive *)me)->run();
    return (void *)0;
}
//............................................................................
void QActive::start(uint8_t prio,
                    QEvent const *qSto[], uint32_t qLen,
                    void *stkSto, uint32_t stkSize,
                    QEvent const *ie)
{
    Q_REQUIRE(stkSto == (void *)0);     // p-threads allocate stack internally

    eQueue_.init(qSto, qLen);
    pthread_cond_init(&osObject_, 0);

    prio_ = prio;
    QF::add_(this);                     // make QF aware of this active object
    init(ie);                                // execute the initial transition

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // SCHED_FIFO corresponds to real-time preemptive priority-based scheduler
    // NOTE: This scheduling policy requires the superuser priviledges
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
                                                                 // see NOTE04
    struct sched_param param;
    param.sched_priority = prio
                           + (sched_get_priority_max(SCHED_FIFO)
                              - QF_MAX_ACTIVE - 3);

    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&thread_, &attr, &::run, this) != 0) {
               // Creating the p-thread with the SCHED_FIFO policy failed.
               // Most probably this application has no superuser privileges,
               // so we just fall back to the default SCHED_OTHER policy
               // and priority 0.

        pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        param.sched_priority = 0;
        pthread_attr_setschedparam(&attr, &param);
        Q_ALLEGE(pthread_create(&thread_, &attr, &::run, this) == 0);
    }
    pthread_attr_destroy(&attr);
}
//............................................................................
void QActive::stop(void) {
    running_ = (uint8_t)0;                              // stop the run() loop

    pthread_cond_destroy(&osObject_);        // cleanup the condition variable
       // make sure posting events to the event queue raises assertion, NOTE05
    eQueue_.nTot_ = (QEQueueCtr)0;
}

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// In Linux, the scheduler policy closest to real-time is the SCHED_FIFO
// policy, available only with superuser privileges. QF::run() attempts to set
// this policy as well as to maximize its priority, so that the ticking
// occurrs in the most timely manner (as close to an interrupt as possible).
// However, setting the SCHED_FIFO policy might fail, most probably due to
// insufficient privileges.
//
// NOTE02:
// On some Linux systems nanosleep() might actually not deliver the finest
// time granularity. For example, on some Linux implementations, nanosleep()
// could not block for shorter intervals than 20ms, while the underlying
// clock tick period was only 10ms. Sometimes, the select() system call can
// provide a finer granularity.
//
// NOTE03:
// Any blocking system call, such as nanosleep() or select() system call can
// be interrupted by a signal, such as ^C from the keyboard. In this case this
// QF port breaks out of the event-loop and returns to main() that exits and
// terminates all spawned p-threads.
//
// NOTE04:
// According to the man pages (for pthread_attr_setschedpolicy) the only value
// supported in the Linux p-threads implementation is PTHREAD_SCOPE_SYSTEM,
// meaning that the threads contend for CPU time with all processes running on
// the machine. In particular, thread priorities are interpreted relative to
// the priorities of all other processes on the machine.
//
// This is good, because it seems that if we set the priorities high enough,
// no other process (or thread running within) can gain control over the CPU.
//
// However, QF limits the number of priority levels to QF_MAX_ACTIVE.
// Assuming that a QF application will be real-time, this port reserves the
// three highest Linux priorities for the ISR-like threads (e.g., the ticker,
// I/O), and the rest highest-priorities for the active objects.
//
// NOTE05:
// QACTIVE_OSOBJECT_SIGNAL_(me_) in the Linux port qf_port.h makes the
// following assertion:
// Q_ASSERT((me_)->eQueue_.nTot_ != (QEQueueCtr)0);
//
// NOTE06:
// The select() system call seems to deliver the finest time granularity of
// 1 clock tick. The timeout value passed to select() is rounded up to the
// nearest tick (10 ms on desktop Linux). The timeout cannot be too short,
// because the system might choose to busy-wait for very short timeouts.
// An alternative, POSIX nanosleep() system call seems to deliver only 20ms
// granularity.
//
// Here the select() call is used not just as a fairly portable way to sleep
// with subsecond precision. The select() call is also used to detect any
// characters typed on the console.
//
// Also according to man pages, on Linux, the function select() modifies
// timeout to reflect the amount of time not slept; most other implementations
// do not do this. This causes problems both when Linux code which reads
// timeout is ported to other operating systems, and when code is ported to
// Linux that reuses a struct timeval for multiple selects in a loop without
// reinitializing it. Here the microsecond part of the structure is re-
// initialized before each select() call.
