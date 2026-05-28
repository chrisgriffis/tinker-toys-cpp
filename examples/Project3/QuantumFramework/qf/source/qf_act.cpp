#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.2.00
// Date of the Last Update:  Aug 10, 2006
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
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

Q_DEFINE_THIS_MODULE(qf_act)

/// \ingroup qf
/// \file qf_act.cpp
/// \brief QF::active_[], QF::getVersion(), and QF::add_()/QF::remove_()
/// implementation.

// public objects ------------------------------------------------------------
QActive *QF::active_[QF_MAX_ACTIVE + 1];        // to be used by QF ports only
uint8_t QF_intLockNest_;                       // interrupt-lock nesting level
uint8_t QF_isrNest_;                                 // ISR-call nesting level

QActive::QActive(QState initial) : QF_ACTIVE_SUPER_(initial) {
}
QActive::~QActive() {
}

//............................................................................
//lint -e970 -e971               ignore MISRA rules 13 and 14 in this function
const char Q_ROM *QF::getVersion(void) {
    static const char Q_ROM version[] = "3.2.00";
    return version;
}
//............................................................................
void QF::add_(QActive *a) {
    uint8_t p = a->prio_;
    Q_REQUIRE(((uint8_t)0 < p)
              && (p <= (uint8_t)QF_MAX_ACTIVE)
              && (active_[p] == (QActive *)0));

    QS_BEGIN_(QS_QF_ACTIVE_ADD, QS::aoObj_, a);
        QS_TIME_();                                               // timestamp
        QS_OBJ_(a);                                       // the active object
        QS_U8_(p);                        // the priority of the active object
    QS_END_();

    active_[p] = a;
}
//............................................................................
void QF::remove_(QActive const *a) {
    uint8_t p = a->prio_;
    Q_REQUIRE(((uint8_t)0 < p) && (p <= (uint8_t)QF_MAX_ACTIVE));

    QS_BEGIN_(QS_QF_ACTIVE_REMOVE, QS::aoObj_, a);
        QS_TIME_();                                               // timestamp
        QS_OBJ_(a);                                       // the active object
        QS_U8_(p);                        // the priority of the active object
    QS_END_();

    active_[p] = (QActive *)0;                   // free-up the priority level
}
