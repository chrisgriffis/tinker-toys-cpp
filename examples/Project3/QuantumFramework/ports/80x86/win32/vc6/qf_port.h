//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++  port to 80x86, Win32, Visual C++ 6.0
// Last Updated for Version: 3.1.05
// Date of the Last Update:  Feb 10, 2005
//
// Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
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

                                         // Win32 event queue and thread types
#define QF_OS_OBJECT_TYPE           HANDLE
#define QF_EQUEUE_TYPE              QEQueue
#define QF_THREAD_TYPE              HANDLE

                        // various QF object sizes configuration for this port
#define QF_MAX_ACTIVE               63
#define QF_EVENT_SIZ_SIZE           4
#define QF_EQUEUE_CTR_SIZE          4
#define QF_MPOOL_SIZ_SIZE           4
#define QF_MPOOL_CTR_SIZE           4
#define QF_TIMEEVT_CTR_SIZE         4

                              // Win32 interrupt locking/unlocking, see NOTE01
// QF_INT_KEY_TYPE not defined
#define QF_INT_LOCK(key_)           EnterCriticalSection(&QF_win32CritSect_)
#define QF_INT_UNLOCK(key_)         LeaveCriticalSection(&QF_win32CritSect_)

                              // Win32 scheduler locking/unlocking, see NOTE02
#define QF_SCHED_LOCK()             EnterCriticalSection(&QF_win32CritSect_)
#define QF_SCHED_UNLOCK()           LeaveCriticalSection(&QF_win32CritSect_)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // needed to stop windows.h including winsock.h (which later, conflicts with winsock2.h)
#endif
#include <windows.h>                                              // Win32 API

#include "qep_port.h"                                              // QEP port
#include "qequeue.h"                                // Win32 needs event-queue
#include "qmpool.h"                                 // Win32 needs memory-pool
#include "qf.h"                    // QF platform-independent public interface


//////////////////////////////////////////////////////////////////////////////
// interface used only inside QF, but not in applications

                                    // Win32-specific OS-object implementation
#define QACTIVE_OSOBJECT_WAIT_(me_) \
    if ((me_)->eQueue_.frontEvt_ == (QEvent const *)0) { \
        do { \
            QF_INT_UNLOCK_(); \
            (void)WaitForSingleObject((me_)->osObject_, (DWORD)INFINITE); \
            QF_INT_LOCK_(); \
        } while ((me_)->eQueue_.frontEvt_ == (QEvent const *)0); \
    } else ((void)0)

#define QACTIVE_OSOBJECT_SIGNAL_(me_) \
    QF_INT_UNLOCK_(); \
    Q_ASSERT((me_)->osObject_ != (HANDLE)0); \
    (void)SetEvent((me_)->osObject_)

#define QACTIVE_OSOBJECT_ONIDLE_(me_) ((void)0)

                              // Win32-specific active object queue operations
#define QACTIVE_POST_FIFO_(me_, e_) ((me_)->postFIFO_(e_))
#define QACTIVE_POST_LIFO_(me_, e_) ((me_)->postLIFO_(e_))
#define QACTIVE_GET_(me_, e_)       ((e_) = (me_)->get_())

                                       // Win32-specific event pool operations
#define QF_EPOOL_TYPE_              QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    (p_).init(poolSto_, poolSize_, evtSize_)
#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_).getBlockSize())
#define QF_EPOOL_GET_(p_, e_)       ((e_) = (QEvent *)(p_).get())
#define QF_EPOOL_PUT_(p_, e_)       ((p_).put(e_))

                                                     // Win32 critical section
extern QF_API CRITICAL_SECTION QF_win32CritSect_;
                                         // flag indicating when QF is running
extern QF_API uint8_t QF_running_;

//////////////////////////////////////////////////////////////////////////////
//
// NOTE01:
// QF, like all real-time frameworks, needs to execute certain sections of
// code indivisibly to avoid data corruption. The most straightforward way of
// protecting such critical sections of code is disabling and enabling
// interrupts, which Win32 does not allow.
//
// This QF port uses therefore a single package-scope Win32 critical section
// object QF_win32CritSect_ to protect all critical sections.
//
// Using the single critical section object for all crtical section guarantees
// that only one thread at a time can execute inside a critical section. This
// prevents race conditions and data corruption.
//
// Please note, however, that the Win32 critical section implementation
// behaves differently than interrupt locking. A common Win32 critical section
// ensures that only one thread at a time can execute a critical section, but
// it does not guarantee that a context switch cannot occur within the
// critical section. In fact, such context switches probably will happen, but
// they should not cause concurrency hazards because the critical section
// eliminates all race conditionis.
//
// Unlinke simply disabling and enabling interrupts, the critical section
// approach is also subject to priority inversions. Various versions of
// Windows handle priority inversions differently, but it seems that most of
// them recognize priority inversions and dynamically adjust the priorities of
// threads to prevent it. Please refer to the MSN articles for more
// information.
//
//
// NOTE02:
// QF uses scheduler locking when it publishes events to multiple subscribers
// and in this port also when QF::tick() is invoked.
//
// The Win32 API does not support scheduler locking.
//
// Therefore, this port implements scheduler locking as a critical section,
// which in turn is implemented with the common critical section object
// QF_win32CritSect_.
//
// Please note that the consequence of using critical section for scheduler
// locking is nesting of critical sections. Win32 critical sections are
// recursive.
//

#endif                                                            // qf_port_h
