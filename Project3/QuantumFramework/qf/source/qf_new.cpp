#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.1.03
// Date of the Last Update:  Nov 13, 2005
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
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qf_new)

/// \ingroup qf
/// \file qf_new.cpp
/// \brief QF::new_() implementation.

//............................................................................
QEvent *QF::new_(uint16_t evtSize, QSignal sig) {
                    // find the pool id that fits the requested event size ...
    uint8_t id = (uint8_t)0;
    while (evtSize > QF_EPOOL_EVENT_SIZE_(QF_pool_[id])) {
        ++id;
        Q_ASSERT(id < QF_maxPool_);      // cannot run out of registered pools
    }

    QS_BEGIN_(QS_QF_NEW, (void *)0, (void *)0);
        QS_TIME_();                                               // timestamp
        QS_EVS_(evtSize);                             // the size of the event
        QS_SIG_(sig);                               // the signal of the event
    QS_END_();

    QEvent *e;
    QF_EPOOL_GET_(QF_pool_[id], e);
    Q_ASSERT(e != (QEvent *)0);             // pool must not run out of events

    e->sig = sig;                                 // set signal for this event
                       // store a pool ID in the event, reference counter == 0
    e->attrQF__ = (uint8_t)((id + 1) << 6);
    return e;
}
