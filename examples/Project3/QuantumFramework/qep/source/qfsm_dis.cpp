#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QEP/C++
// Last Updated for Version: 3.1.00
// Date of the Last Update:  Sep 25, 2005
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
#include "qep_pkg.h"

//............................................................................
/// \ingroup qep
/// \file qfsm_dis.cpp
/// \brief QFsm::dispatch() implementation.

//............................................................................
void QFsm::dispatch(QEvent const *e) {
    QState s = state_.fsm;
    (*s)(this, e);
    if (tran_ != Q_TRAN_NONE_TYPE) {
        (*s)(this, &QEP_reservedEvt_[Q_EXIT_SIG]);          // exit the source

        (*state_.fsm)(this, &QEP_reservedEvt_[Q_ENTRY_SIG]);//enter the target

        tran_ = Q_TRAN_NONE_TYPE;             // get ready for next transition

        QS_BEGIN_(QS_QEP_TRAN, QS::smObj_, this);
            QS_TIME_();                                          // time stamp
            QS_SIG_(e->sig);                        // the signal of the event
            QS_OBJ_(this);                        // this state machine object
            QS_FUN_(s);                        // the source of the transition
            QS_FUN_(state_.fsm);                       // the new active state
        QS_END_();

    }
    else {
        QS_BEGIN_(QS_QEP_INTERN_TRAN, QS::smObj_, this);
            QS_TIME_();                                          // time stamp
            QS_SIG_(e->sig);                        // the signal of the event
            QS_OBJ_(this);                        // this state machine object
            QS_FUN_(s);                    // the state that handled the event
        QS_END_();
    }
}
