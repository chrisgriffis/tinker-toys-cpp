//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++, vanilla port
// Last Updated for Version: 3.1.04
// Date of the Last Update:  Dec 06, 2005
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

                           // Vanilla QF/C++ port event queue and thread types
#define QF_EQUEUE_TYPE              QEQueue
#define QF_OS_OBJECT_TYPE           int8_t
#define QF_THREAD_TYPE              int8_t

                        // various QF object sizes configuration for this port
#define QF_MAX_ACTIVE               63
#define QF_EVENT_SIZ_SIZE           2
#define QF_EQUEUE_CTR_SIZE          1
#define QF_MPOOL_SIZ_SIZE           2
#define QF_MPOOL_CTR_SIZE           2
#define QF_TIMEEVT_CTR_SIZE         2

                                            // DOS critical section entry/exit
// QF_INT_KEY_TYPE not defined
#define QF_INT_LOCK(key_)           disable()
#define QF_INT_UNLOCK(key_)         enable()

           // Vanilla QF/C++ port scheduler locking/unlocking -- not necessary
#define QF_SCHED_LOCK()             ((void)0)
#define QF_SCHED_UNLOCK()           ((void)0)

#include <dos.h>                                                    // DOS API
#undef outportb  // don't use the macro because it has a bug in Turbo C++ 1.01

#include "qep_port.h"                                              // QEP port
#include "qequeue.h"                  // Vanilla QF/C++ port needs event-queue
#include "qmpool.h"                   // Vanilla QF/C++ port needs memory-pool
#include "qf.h"                    // QF platform-independent public interface
#include "qpset.h"                   // Vanilla QF/C++ port needs priority-set


//////////////////////////////////////////////////////////////////////////////
// interface used only inside QF, but not in applications
//

 // Vanilla QF/C++ port OS-object operations (OS-object is the priority of AO)
#define QACTIVE_OSOBJECT_WAIT_(me_) \
    Q_ASSERT((me_)->eQueue_.frontEvt_ != (QEvent *)0)

#define QACTIVE_OSOBJECT_SIGNAL_(me_) \
    QF_readySet_.insert((me_)->osObject_); \
    QF_INT_UNLOCK_()

#define QACTIVE_OSOBJECT_ONIDLE_(me_) \
    QF_readySet_.remove((me_)->osObject_)

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


extern QPSet QF_readySet_;        // package-scope ready set of active objects

#endif                                                            // qf_port_h
