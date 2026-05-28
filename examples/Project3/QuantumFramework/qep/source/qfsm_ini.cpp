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
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qfsm_ini)

//............................................................................
/// \ingroup qep
/// \file qfsm_ini.cpp
/// \brief QFsm::init() implementation.

//............................................................................
void QFsm::init(QEvent const *e) {
    Q_REQUIRE(state_.fsm != (QState)0);          // "this" must be initialized
    QState initial = state_.fsm;

    QS_BEGIN_(QS_QEP_STATE_INIT, QS::smObj_, this);
        QS_TIME_();                                               // timestamp
        QS_OBJ_(this);                            // this state machine object
        QS_FUN_((QState)0);              // the source (not defined for a FSM)
        QS_FUN_(state_.fsm);                   // the target of the transition
    QS_END_();

    (*initial)(this, e);              // execute the initial transition action
    Q_ASSERT(initial != state_.fsm);           // the target cannot be initial
    (*state_.fsm)(this, &QEP_reservedEvt_[Q_ENTRY_SIG]);   // enter the target

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS::smObj_, this);
        QS_TIME_();                                              // time stamp
        QS_OBJ_(this);                            // this state machine object
        QS_FUN_(state_.fsm);                           // the new active state
    QS_END_();
}
//............................................................................
QFsm::~QFsm() {
}
