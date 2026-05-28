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

/// \ingroup qf
/// \file qte_darm.cpp
/// \brief QTimeEvt::disarm() implementation.

//............................................................................
// NOTE: disarm a time evt (no harm in disarming an already disarmed time evt)
uint8_t QTimeEvt::disarm(void) {
    uint8_t wasArmed;
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
    if (prev_ != (QTimeEvt *)0) {         // is the time event actually armed?
        wasArmed = (uint8_t)1;
        if (this == QF_timeEvtListHead_) {
            QF_timeEvtListHead_ = next_;
        }
        else {
            if (next_ != (QTimeEvt *)0) {         // not the last in the list?
                next_->prev_ = prev_;
            }
            prev_->next_ = next_;
        }
        prev_ = (QTimeEvt *)0;              // mark the time event as disarmed

        QS_BEGIN_NOLOCK_(QS_QF_TIMEEVT_DISARM, QS::teObj_, this);
            QS_TIME_();                                           // timestamp
            QS_OBJ_(this);                           // this time event object
            QS_OBJ_(act_);                                // the active object
            QS_TEC_(ctr_);                              // the number of ticks
            QS_TEC_(interval_);                                // the interval
        QS_END_NOLOCK_();
    }
    else {                                     // the time event was not armed
        wasArmed = (uint8_t)0;

        QS_BEGIN_NOLOCK_(QS_QF_TIMEEVT_DISARM_ATTEMPT, QS::teObj_, this);
            QS_TIME_();                                           // timestamp
            QS_OBJ_(this);                           // this time event object
            QS_OBJ_(act_);                                // the active object
        QS_END_NOLOCK_();
    }
    QF_INT_UNLOCK_();
    return wasArmed;
}
