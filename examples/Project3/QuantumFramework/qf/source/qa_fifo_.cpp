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

Q_DEFINE_THIS_MODULE(qa_fifo_)

/// \ingroup qf
/// \file qa_fifo_.cpp
/// \brief QActive::postFIFO_() implementation.
///
/// This implementation is linked in only when the native QF event queue
/// is used

//............................................................................
void QActive::postFIFO_(QEvent const *e) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    QS_BEGIN_NOLOCK_(QS_QF_ACTIVE_POST_FIFO, QS::aoObj_, this);
        QS_TIME_();                                               // timestamp
        QS_SIG_(e->sig);                            // the signal of the event
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
        eQueue_.ring_[eQueue_.head_] = e;   // insert e into the buffer (FIFO)
        ++eQueue_.head_;
        if (eQueue_.head_ == eQueue_.end_) {
            eQueue_.head_ = (QEQueueCtr)0;                    // wrap the head
        }

        ++eQueue_.nUsed_;                                // update # of events
        if (eQueue_.nUsed_ > eQueue_.nMax_) {
            eQueue_.nMax_ = eQueue_.nUsed_;       // store maximum used so far
        }
        QF_INT_UNLOCK_();
    }
}
