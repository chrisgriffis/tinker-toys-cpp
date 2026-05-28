//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.2.00
// Date of the Last Update:  Aug 10, 2006
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
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qf_run)

/// \ingroup qf
/// \file qf_run.cpp
/// \brief "vanilla" QF port: QF::run() non-preemptive scheduler,
/// Active::start(), QActive::stop(), and QF::getPortvesrion() implementation.

// Package-scope objects -----------------------------------------------------
QPSet QF_readySet_;

//............................................................................
const char Q_ROM *QF::getPortVersion(void) {
    static const char Q_ROM version[] = "1.2.00";
    return version;
}
//............................................................................
void QF::run(void) {
    QF::start();                                                // enable ISRs

    for (;;) {                                           // the bacground loop
        QF_INT_LOCK_KEY_
        QF_INT_LOCK_();
        if (!QF_readySet_.isEmpty()) {
            QActive *a = active_[QF_readySet_.findMax()];
            QF_INT_UNLOCK_();

            QEvent const *e;
            QACTIVE_GET_(a, e);   // get the next event for this active object

            a->QF_ACTIVE_SUPER_::dispatch(e);       // dispatch evt to the HSM
            gc(e);       // determine if event is garbage and collect it if so
        }
        else {
#ifndef QF_INT_KEY_TYPE
            QF::onIdle();                                        // see NOTE02
#else
            QF::onIdle(intLockKey__);                            // see NOTE02
#endif                                                      // QF_INT_KEY_TYPE
        }
    }
}
//............................................................................
void QActive::start(uint8_t prio,
                    QEvent const *qSto[], uint32_t qLen,
                    void *stkSto, uint32_t /* stkSize */,
                    QEvent const *ie)
{
    Q_REQUIRE((0 < prio) && (prio <= QF_MAX_ACTIVE)
              && (stkSto == (void *)0));      // does not need per-actor stack
    (void)stkSto;
    prio_ = prio;
    QF::add_(this);                     // make QF aware of this active object
    eQueue_.init(qSto, (QEQueueCtr)qLen);        // initialize the event queue
    osObject_ = prio;                              // initialize the OS object
    QF_ACTIVE_SUPER_::init(ie);                  // execute initial transition
}
//............................................................................
void QActive::stop(void) {
    QF::remove_(this);
}

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// This implemenation of QF::run() represents the non-preeemptive scheduer
// built into QF. This implementation is used only in the "vanilla" QF ports
// to "bare metal" target systems without any underlying RTOS or kernel.
//
// NOTE02:
// QF::onIdle() must be called with interrupts LOCKED because the
// determination of the idle condition (no events in the queues) can change
// at any time by an interrupt posting events to a queue. The QF::onIdle()
// MUST enable interrups internally, perhaps at the same time as putting the
// CPU into a power-saving mode.
//
