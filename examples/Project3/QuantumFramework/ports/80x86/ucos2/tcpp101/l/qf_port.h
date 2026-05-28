//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++ port to 80x86, uC/OS-II, Turbo C++ 1.01, Large model
// Last Updated for Version: 3.1.04
// Date of the Last Update:  Dec 11, 2005
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
                                      // uC/OS-II event queue and thread types
#define QF_EQUEUE_TYPE              OS_EVENT *
#define QF_OS_OBJECT_TYPE           uint8_t
#define QF_THREAD_TYPE              int8_t

                        // various QF object sizes configuration for this port
#define QF_MAX_ACTIVE               OS_MAX_TASKS
#define QF_EVENT_SIZ_SIZE           2
#define QF_EQUEUE_CTR_SIZE          1
#define QF_MPOOL_SIZ_SIZE           2
#define QF_MPOOL_CTR_SIZE           2
#define QF_TIMEEVT_CTR_SIZE         2

             // uC/OS-II critical section operations (critical section type 3)
#define QF_INT_KEY_TYPE             OS_CPU_SR
#define QF_INT_LOCK(key_)           ((key_) = OSCPUSaveSR())
#define QF_INT_UNLOCK(key_)         OSCPURestoreSR(key_)

                                       // uC/OS-II scheduler locking/unlocking
#define QF_SCHED_LOCK()             OSSchedLock()
#define QF_SCHED_UNLOCK()           OSSchedUnlock()

#include <dos.h>                                                    // DOS API
#undef outportb  // don't use the macro because it has a bug in Turbo C++ 1.01

#include "qep_port.h"                                              // QEP port
#ifndef UCOS2_INCLUDES_H
    #define UCOS2_INCLUDES_H
    #include "includes.h"                      // master uC/OS-II include file
#endif
#include "qf.h"                    // QF platform-independent public interface

//////////////////////////////////////////////////////////////////////////////
// interface used only inside QF, but not in applications
//
                                          // uC/OS-II active object operations
#define QACTIVE_POST_FIFO_(me_, e_) \
    Q_ALLEGE(OSQPost((me_)->eQueue_, (void *)(e_)) == OS_NO_ERR)

#define QACTIVE_POST_LIFO_(me_, e_) \
    Q_ALLEGE(OSQPostFront((me_)->eQueue_, (void *)(e_)) == OS_NO_ERR)

#define QACTIVE_GET_(me_, e_) do { \
    INT8U err; \
    (e_) = (QEvent *)OSQPend((OS_EVENT *)(me_)->eQueue_, 0, &err); \
} while (0)

                                             // uC/OS-II event pool operations
#define QF_EPOOL_TYPE_              OS_MEM *
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) do { \
    INT8U err; \
    (p_) = OSMemCreate(poolSto_, (INT32U)((poolSize_)/(evtSize_)), \
                       (INT32U)(evtSize_), &err); \
    Q_ASSERT((p_) != (OS_MEM *)0); \
} while (0)

#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_)->OSMemBlkSize)
#define QF_EPOOL_GET_(p_, e_) do { \
    INT8U err; \
    ((e_) = (QEvent *)OSMemGet((p_), &err)); \
} while (0)

#define QF_EPOOL_PUT_(p_, e_)       OSMemPut((p_), (void *)(e_))

#endif                                                            // qf_port_h
