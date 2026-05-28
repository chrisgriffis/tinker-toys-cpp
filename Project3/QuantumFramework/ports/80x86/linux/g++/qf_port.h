//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++  port to 80x86, Linux/P-threads, g++
// Last Updated for Version: 3.1.05
// Date of the Last Update:  Feb 10, 2006
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
//
// This software may be distributed and modified under the terms of the GNU
// General Public License version 2 (GPL) as published by the Free Software
// Foundation and appearing in the file GPL.TXT included in the packaging of
// this file. Please note that GPL Section 2[b] requires that all works based
// on this software must also be made publicly available under the terms of
// the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly allow the
// licensees to retain the proprietary status of their code. The licensees
// who use this software under one of Quantum Leaps commercial licenses do
// not use this software under the GPL and therefore are not subject to any
// of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#ifndef qf_port_h
#define qf_port_h
                                         // Linux event queue and thread types
#define QF_EQUEUE_TYPE              QEQueue
#define QF_OS_OBJECT_TYPE           pthread_cond_t
#define QF_THREAD_TYPE              pthread_t

                        // various QF object sizes configuration for this port
#define QF_MAX_ACTIVE               63
#define QF_EVENT_SIZ_SIZE           4
#define QF_EQUEUE_CTR_SIZE          4
#define QF_MPOOL_SIZ_SIZE           4
#define QF_MPOOL_CTR_SIZE           4
#define QF_TIMEEVT_CTR_SIZE         4

                       // QF critical section entry/exit for Linux, see NOTE01
// QF_INT_KEY_TYPE not defined
#define QF_INT_LOCK(key_)           pthread_mutex_lock(&QF_pThreadMutex_)
#define QF_INT_UNLOCK(key_)         pthread_mutex_unlock(&QF_pThreadMutex_)

                       // QF scheduler locking/unlocking for Linux, see NOTE02
#define QF_SCHED_LOCK()             pthread_mutex_lock(&QF_pThreadMutex_)
#define QF_SCHED_UNLOCK()           pthread_mutex_unlock(&QF_pThreadMutex_)

         // __USE_GNU give us access to PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#ifndef __USE_GNU
#define __USE_GNU
#include <pthread.h>
#undef __USE_GNU
#else
#include <pthread.h>
#endif
#include <errno.h>
#include "qep_port.h"                                              // QEP port
#include "qequeue.h"                                // Linux needs event-queue
#include "qmpool.h"                                 // Linux needs memory-pool
#include "qf.h"                    // QF platform-independent public interface

//////////////////////////////////////////////////////////////////////////////
// interface used only inside QF, but not in applications
//
                                         // OS-object implementation for Linux
#define QACTIVE_OSOBJECT_WAIT_(me_) \
    if ((me_)->eQueue_.frontEvt_ == (QEvent const *)0) { \
        do { \
            pthread_cond_wait(&(me_)->osObject_, &QF_pThreadMutex_); \
        } while ((me_)->eQueue_.frontEvt_ == (QEvent const *)0); \
    } else ((void)0)

#define QACTIVE_OSOBJECT_SIGNAL_(me_) \
    QF_INT_UNLOCK(); \
    Q_ASSERT((me_)->eQueue_.nTot_ != (QEQueueCtr)0); \
    pthread_cond_signal(&(me_)->osObject_)

#define QACTIVE_OSOBJECT_ONIDLE_(me_) ((void)0)

                                           // native QF event queue operations
#define QACTIVE_POST_FIFO_(me_, e_) ((me_)->postFIFO_(e_))
#define QACTIVE_POST_LIFO_(me_, e_) ((me_)->postLIFO_(e_))
#define QACTIVE_GET_(me_, e_)       ((e_) = (me_)->get_())

                                            // native QF event pool operations
#define QF_EPOOL_TYPE_              QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    (p_).init(poolSto_, poolSize_, evtSize_)
#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_).getBlockSize())
#define QF_EPOOL_GET_(p_, e_)       ((e_) = (QEvent *)(p_).get())
#define QF_EPOOL_PUT_(p_, e_)       ((p_).put(e_))

                                     // mutex used for the QF critical section
extern pthread_mutex_t QF_pThreadMutex_;
                                         // flag indicating when QF is running
extern uint8_t QF_running_;

// NOTES: ////////////////////////////////////////////////////////////////////
//
// NOTE01:
// QF, like all real-time frameworks, needs to execute certain sections of
// code indivisibly to avoid data corruption. The most straightforward way of
// protecting such critical sections of code is disabling and enabling
// interrupts, which Linux does not allow.
//
// This QF port uses therefore a single package-scope p-thread mutex
// QF_pThreadMutex_ to protect all critical sections. The mutex is locked upon
// the entry to each critical sectioni and unlocked upon exit.
//
// Using the single mutex for all crtical section guarantees that only one
// thread at a time can execute inside a critical section. This prevents race
// conditions and data corruption.
//
// Please note, however, that the mutex implementation of a critical section
// behaves differently than the standard interrupt locking. A common mutex
// ensures that only one thread at a time can execute a critical section, but
// it does not guarantee that a context switch cannot occur within the
// critical section. In fact, such context switches probably will happen, but
// they shouldnot cause concurrency hazards because the mutex eliminates all
// race conditionis.
//
// Unlinke simply disabling and enabling interrupts, the mutex approach is
// also subject to priority inversions. However, the p-thread mutex
// implementation, such as Linux p-threads, should support the priority-
// inheritance protocol.
//
//
// NOTE02:
// QF uses scheduler locking when it publishes events to multiple subscribers
// and in this port also when QF::tick() is invoked.
//
// The Linux API does not support scheduler locking.
//
// Therefore, this port implements scheduler locking as a critical section,
// which in turn is implemented with the common mutex QF_pThreadMutex_.
//
// Please note that the consequence of using critical section for scheduler
// locking is nesting of critical sections. This means that the common mutex
// QF_pThreadMutex_ must be initialized as a RECURSIVE mutex.
//

#endif                                                            // qf_port_h
