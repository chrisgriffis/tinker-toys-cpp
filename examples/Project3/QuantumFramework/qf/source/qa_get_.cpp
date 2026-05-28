#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.1.04
// Date of the Last Update:  Dec 01, 2005
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

Q_DEFINE_THIS_MODULE(qa_get_)

/// \ingroup qf
/// \file qa_get_.cpp
/// \brief QActive::get_() implementation.
///
/// This implementation is linked in only when the native QF event queue
/// is used

//............................................................................
QEvent const *QActive::get_(void) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    QACTIVE_OSOBJECT_WAIT_(this);         // wait for event to arrive directly

    QEvent const *e = eQueue_.frontEvt_;
    if (eQueue_.nUsed_ != (QEQueueCtr)0) {   // any events in the ring buffer?
                                                 // remove event from the tail
        eQueue_.frontEvt_ = eQueue_.ring_[eQueue_.tail_];
        ++eQueue_.tail_;
        if (eQueue_.tail_ == eQueue_.end_) {           // need to wrap around?
            eQueue_.tail_ = (QEQueueCtr)0;                    // wrap the tail
        }

        --eQueue_.nUsed_;                 // one less event in the ring buffer

        QS_BEGIN_NOLOCK_(QS_QF_ACTIVE_GET, QS::aoObj_, this);
            QS_TIME_();                                           // timestamp
            QS_SIG_(e->sig);                       // the signal of this event
            QS_OBJ_(this);                               // this active object
            QS_U8_(e->attrQF__);              // the QF attribute of the event
            QS_EQC_(eQueue_.nUsed_);                 // number of used entries
        QS_END_NOLOCK_();
    }
    else {
        eQueue_.frontEvt_ = (QEvent const *)0;      // the queue becomes empty
        QACTIVE_OSOBJECT_ONIDLE_(this);

        QS_BEGIN_NOLOCK_(QS_QF_ACTIVE_GET_LAST, QS::aoObj_, this);
            QS_TIME_();                                           // timestamp
            QS_SIG_(e->sig);                       // the signal of this event
            QS_OBJ_(this);                               // this active object
            QS_U8_(e->attrQF__);              // the QF attribute of the event
        QS_END_NOLOCK_();
    }
    QF_INT_UNLOCK_();
    return e;
}
//............................................................................
uint32_t QF::getQueueMargin(uint8_t prio) {
    Q_REQUIRE((prio <= (uint8_t)QF_MAX_ACTIVE)
              && (active_[prio] != (QActive *)0));

    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
    uint32_t margin = (uint32_t)(active_[prio]->eQueue_.nTot_
                                 - active_[prio]->eQueue_.nMax_);
    QF_INT_UNLOCK_();

    return margin;
}
