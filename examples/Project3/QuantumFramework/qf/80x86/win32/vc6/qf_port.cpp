#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product:  QF/C++ port to 80x86, Win32, Visual C++ 6.0
// Version:  Compatible with QF/C 3.x.yy
// Updated:  Feb 13, 2006
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
//
// This QF/C port is part of the Quantum Leaps QF/C software, and may be
// distributed and modified under the terms of the GNU General Public License
// version 2 (GPL) as published by the Free Software Foundation and appearing
// in the file GPL.TXT included in the packaging of this file. Please note
// that GPL Section 2[b] requires that all works based on this software must
// also be made publicly available under the terms of the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified in conjunction
// with a valid QF/C Quantum Leaps commercial license. Quantum Leaps
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
#include "qf_pkg.h"
#include "qassert.h"

#if !defined WINCE
#include <process.h>                        // for _beginthread()/_endthread()
#include <conio.h>
#endif

#include <stdio.h>

Q_DEFINE_THIS_MODULE(qf_port)

// Global objects ------------------------------------------------------------
CRITICAL_SECTION QF_win32CritSect_;
uint8_t QF_running_;

//............................................................................
const char *QF::getPortVersion(void) {
    return "1.1.05";
}
//............................................................................
void QF::init(void) {
    InitializeCriticalSection(&::QF_win32CritSect_);
    QF_running_ = (uint8_t)1;
}
//............................................................................
void QF::start(void) {
    // raise the priority of this (main) thread to tick more timely
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
}
//............................................................................
void QF::exit(void) {
    QF_running_ = (uint8_t)0;
}
//............................................................................
void QF::run(void) {
    QF::start();                                               // start the QF

    while (QF_running_) {

        QF_SCHED_LOCK();
        QF::tick();                                     // process a time tick
        QF_SCHED_UNLOCK();

        Sleep(10);                               // wait for the tick interval
    }
    DeleteCriticalSection(&::QF_win32CritSect_);
}
//............................................................................
#ifdef WINCE
static DWORD run(void *me) 
#else
static void __cdecl run(void *me) 
#endif
{  // the exact signature for _beginthread()
    ((QActive *)me)->run();
#ifdef WINCE
    ExitThread(0);
    return 0;
#else
    _endthread();      // cleanup after the thead and close the thread_ handle
#endif
}
//............................................................................
void QActive::start(uint8_t prio,
                    QEvent const *qSto[], uint32_t qLen,
                    void *stkSto, uint32_t stkSize,
                    QEvent const *ie)
{
    Q_REQUIRE((stkSto == (void *)0)  /* Windows allocates stack internally */
        && (stkSize != 0));

    eQueue_.init(qSto, (QEQueueCtr)qLen);
    osObject_ = CreateEvent(NULL, FALSE, FALSE, NULL);
    prio_ = prio;
    QF::add_(this);                     // make QF aware of this active object
    init(ie);                                    // execute initial transition

#ifdef WINCE
    DWORD threadId;
    thread_ = (HANDLE)CreateThread(0,stkSize,&::run,  this,0, &threadId);
#else
    thread_ = (HANDLE)_beginthread(&::run, stkSize, this);
#endif

    Q_ASSERT(thread_ != (HANDLE)0);                  // thread must be created

    int p;
    switch (prio_) {                    // remap QF priority to Win32 priority
        case 1:
            p = THREAD_PRIORITY_LOWEST;
            break;
        case 2:
            p = THREAD_PRIORITY_IDLE;
            break;
        case 3:
            p = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case (QF_MAX_ACTIVE - 1):
            p = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case QF_MAX_ACTIVE:
            p = THREAD_PRIORITY_HIGHEST;
            break;
        default:
            p = THREAD_PRIORITY_NORMAL;
            break;
    }
    SetThreadPriority(thread_, p);
}
//............................................................................
void QActive::stop(void) {
    CloseHandle(osObject_);                            // cleanup the OS event
    osObject_ = (HANDLE)0;  // make sure nobody can post events to this object
    running_ = (uint8_t)0;                     // stop the QActive::run() loop
}
