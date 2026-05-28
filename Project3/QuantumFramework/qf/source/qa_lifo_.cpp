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

Q_DEFINE_THIS_MODULE(qa_lifo_)

/// \ingroup qf
/// \file qa_lifo_.cpp
/// \brief QActive::postLIFO_() implementation.
///
/// This implementation is linked in only when the native QF event queue
/// is used

//............................................................................
void QActive::postLIFO_(QEvent const *e) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    QS_BEGIN_NOLOCK_(QS_QF_ACTIVE_POST_LIFO, QS::aoObj_, this);
        QS_TIME_();                                               // timestamp
        QS_SIG_(e->sig);                           // the signal of this event
        QS_OBJ_(this);                                   // this active object
        QS_U8_(e->attrQF__);                  // the QF attribute of the event
        QS_EQC_(eQueue_.nUsed_);                     // number of used entries
        QS_EQC_(eQueue_.nMax_);                  // max number of used entries
    QS_END_NOLOCK_();

    if (eQueue_.frontEvt_ == (QEvent const *)0) {       // is the queue empty?
        eQueue_.frontEvt_ = e;                       // deliver event directly
        QACTIVE_OSOBJECT_SIGNAL_(this);// unblock thread waiting on this queue
                      // NOTE: the critical section is exited within the macro
    }
    else {               // queue is not empty, leave event in the ring-buffer
                                        // queue must accept all posted events
        Q_ASSERT(eQueue_.nUsed_ < eQueue_.nTot_);
        if (eQueue_.tail_ == (QEQueueCtr)0) {        // need to wrap the tail?
            eQueue_.tail_ = (QEQueueCtr)(eQueue_.end_ - 1);   // wrap the tail
        }
        else {
            --eQueue_.tail_;
        }
        eQueue_.ring_[eQueue_.tail_] = eQueue_.frontEvt_;
        eQueue_.frontEvt_ = e;                           // put event to front

        ++eQueue_.nUsed_;             // update number of events in the buffer
        if (eQueue_.nUsed_ > eQueue_.nMax_) {            // update # of events
            eQueue_.nMax_ = eQueue_.nUsed_;       // store maximum used so far
        }
        QF_INT_UNLOCK_();
    }
}
