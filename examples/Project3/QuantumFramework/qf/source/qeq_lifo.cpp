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

Q_DEFINE_THIS_MODULE(qeq_lifo)

/// \ingroup qf
/// \file qeq_lifo.cpp
/// \brief QEQueue::postLIFO() implementation.

//............................................................................
void QEQueue::postLIFO(QEvent const *e) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    QS_BEGIN_NOLOCK_(QS_QF_EQUEUE_POST_LIFO, QS::eqObj_, this);
        QS_TIME_();                                               // timestamp
        QS_SIG_(e->sig);                           // the signal of this event
        QS_OBJ_(this);                                    // this queue object
        QS_U8_(e->attrQF__);                  // the QF attribute of the event
        QS_EQC_(nUsed_);                             // number of used entries
        QS_EQC_(nMax_);                          // max number of used entries
    QS_END_NOLOCK_();

    if (e->attrQF__ != (uint8_t)0) {                    // is it a pool event?
        ++((QEvent *)e)->attrQF__;          // increment the reference counter
                   // NOTE: cast the 'const' away, which is legitimate because
                   // it's a pool event
    }

    if (frontEvt_ != (QEvent const *)0) {           // is the queue not empty?
               // the queue must be able to accept the event (cannot overflow)
        Q_ASSERT(nUsed_ < nTot_);
        if (tail_ == (QEQueueCtr)0) {                // need to wrap the tail?
            tail_ = (QEQueueCtr)(end_ - 1);
        }
        else {
            --tail_;
        }
        ring_[tail_] = frontEvt_;                  // buffer the old front evt

        ++nUsed_;                                   // update number of events
        if (nUsed_ > nMax_) {
            nMax_ = nUsed_;                       // store maximum used so far
        }
    }

    frontEvt_ = e;                         // stick the new event to the front

    QF_INT_UNLOCK_();
}
