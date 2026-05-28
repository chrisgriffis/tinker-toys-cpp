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

Q_DEFINE_THIS_MODULE(qhsm_ini)

//............................................................................
/// \ingroup qep
/// \file qhsm_ini.cpp
/// \brief QHsm::init() implementation.

//............................................................................
void QHsm::init(QEvent const *e) {
    QHsmState s;
    QHsmState path[QEP_MAX_NEST_DEPTH_];

    Q_REQUIRE(state_.fsm != (QState)0);         // about to dereference state_

    s = &QHsm::top;                          // an HSM starts in the top state
    (*state_.fsm)(this, e);                // take top-most initial transition

    do {                                           // drill into the target...

        QS_BEGIN_(QS_QEP_STATE_INIT, QS::smObj_, this);
            QS_TIME_();                                          // time stamp
            QS_OBJ_(this);                        // this state machine object
            QS_FUN_(s);                                    // the source state
            QS_FUN_(state_.hsm);               // the target of the transition
        QS_END_();

        int8_t ip = (int8_t)0;                  // transition entry path index
        QHsmState t = state_.hsm;
        path[0] = t;
        for (t = QEP_TRIG_(t, QEP_EMPTY_SIG_); t != s;
             t = QEP_TRIG_(t, QEP_EMPTY_SIG_))
        {
            path[++ip] = t;
        }
                                               // entry path must not overflow
        Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_);

        do {           // retrace the entry path in reverse (desired) order...
                                                             // enter path[ip]
            if (QEP_TRIG_(path[ip], Q_ENTRY_SIG) == (QHsmState)0) {

                QS_BEGIN_(QS_QEP_STATE_ENTRY, QS::smObj_, this);
                    QS_TIME_();                                  // time stamp
                    QS_OBJ_(this);                // this state machine object
                    QS_FUN_(path[ip]);                    // the entered state
                QS_END_();
            }

        } while (--ip >= (int8_t)0);

        s = state_.hsm;
    } while (QEP_TRIG_(s, Q_INIT_SIG) == (QHsmState)0);

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS::smObj_, this);
        QS_TIME_();                                              // time stamp
        QS_OBJ_(this);                            // this state machine object
        QS_FUN_(state_.hsm);                           // the new active state
    QS_END_();
}
