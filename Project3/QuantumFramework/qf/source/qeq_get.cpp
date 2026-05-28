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

/// \ingroup qf
/// \file qeq_get.cpp
/// \brief QEQueue::get() implementation.

//............................................................................
QEvent const *QEQueue::get(void) {
    QEvent const *e;
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    if (frontEvt_ == (QEvent const *)0) {               // is the queue empty?
        e = (QEvent const *)0;              // no event available at this time
    }
    else {
        e = frontEvt_;
        if (nUsed_ != (QEQueueCtr)0) {   // any events in the the ring buffer?
            frontEvt_ = ring_[tail_];            // remove event from the tail
            ++tail_;
            if (tail_ == end_) {                       // need to wrap around?
                tail_ = (QEQueueCtr)0;                        // wrap the tail
            }

            --nUsed_;                     // one less event in the ring buffer

            QS_BEGIN_NOLOCK_(QS_QF_EQUEUE_GET, QS::eqObj_, this);
                QS_TIME_();                                       // timestamp
                QS_SIG_(e->sig);                   // the signal of this event
                QS_OBJ_(this);                            // this queue object
                QS_U8_(e->attrQF__);          // the QF attribute of the event
                QS_EQC_(nUsed_);                     // number of used entries
            QS_END_NOLOCK_();
        }
        else {
            frontEvt_ = (QEvent const *)0;          // the queue becomes empty

            QS_BEGIN_NOLOCK_(QS_QF_EQUEUE_GET_LAST, QS::eqObj_, this);
                QS_TIME_();                                       // timestamp
                QS_SIG_(e->sig);                   // the signal of this event
                QS_OBJ_(this);                            // this queue object
                QS_U8_(e->attrQF__);          // the QF attribute of the event
            QS_END_NOLOCK_();
        }
    }
    QF_INT_UNLOCK_();
    return e;
}
