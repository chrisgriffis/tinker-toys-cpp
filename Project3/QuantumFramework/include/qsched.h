/*****************************************************************************
* Product:  QF/C
* Last Updated for Version: 3.2.00
* Date of the Last Update:  Aug 07, 2006
*
* Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly allow the
* licensees to retain the proprietary status of their code. The licensees
* who use this software under one of Quantum Leaps commercial licenses do
* not use this software under the GPL and therefore are not subject to any
* of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
* Quantum Leaps products:  http://www.quantum-leaps.com/products
* e-mail:                  sales@quantum-leaps.com
*****************************************************************************/
#ifndef qf_sched_h
#define qf_sched_h

/** \ingroup qf
* \file qsched.h
* \brief platform-independent interface to the non-preemptive scheduler.
*
* This header file must be included in all "vanilla" QF ports to "bare metal"
* target boards.
*/

                            /* Vanilla QF port event queue and thread types */
#define QF_EQUEUE_TYPE              QEQueue
#define QF_OS_OBJECT_TYPE           int8_t
#define QF_THREAD_TYPE              int8_t

            /* Vanilla QF port scheduler locking/unlocking -- not necessary */
#define QF_SCHED_LOCK()             ((void)0)
#define QF_SCHED_UNLOCK()           ((void)0)

/* Vanilla QF/C port OS-object operations (OS-object is the priority of AO) */
#define QACTIVE_OSOBJECT_WAIT_(me_) \
    Q_ASSERT((me_)->eQueue__.frontEvt__ != (QEvent *)0)

#define QACTIVE_OSOBJECT_SIGNAL_(me_) \
    QPSet_insert(&QF_readySet_, (me_)->osObject__); \
    QF_INT_UNLOCK_()

#define QACTIVE_OSOBJECT_ONIDLE_(me_) \
    QPSet_remove(&QF_readySet_, (me_)->osObject__)

                                        /* native QF event queue operations */
#define QACTIVE_POST_FIFO_(me_, e_) (QActive_postFIFO__((me_), (e_)))
#define QACTIVE_POST_LIFO_(me_, e_) (QActive_postLIFO__((me_), (e_)))
#define QACTIVE_GET_(me_, e_)       ((e_) = QActive_get__(me_))

                                         /* native QF event pool operations */
#define QF_EPOOL_TYPE_              QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    QMPool_init(&(p_), poolSto_, poolSize_, evtSize_)
#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_).blockSize__)
#define QF_EPOOL_GET_(p_, e_)       ((e_) = (QEvent *)QMPool_get(&(p_)))
#define QF_EPOOL_PUT_(p_, e_)       (QMPool_put(&(p_), e_))


extern struct QPSetTag QF_readySet_;    /**< QF-ready set of active objects */

#endif                                                          /* qsched_h */
