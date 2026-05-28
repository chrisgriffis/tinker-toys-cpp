#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QEP/C++
// Last Updated for Version: 3.1.01
// Date of the Last Update:  Oct 31, 2005
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
/// \file qhsm_in.cpp
/// \brief QHsm::isIn() implementation.

//............................................................................
uint8_t QHsm::isIn(QHsmState state) {
    QHsmState s = state_.hsm;
    uint8_t inState = (uint8_t)0;    // assume that this HSM is not in 'state'
    do {
        if (s == state) {                              // do the states match?
            inState = (uint8_t)1;                  // match found, return TRUE
            s = (QHsmState)0;                         // break out of the loop
        }
        else {
            s = QEP_TRIG_(s, QEP_EMPTY_SIG_);
        }
    } while (s != (QHsmState)0);
    return inState;                                       // return the status
}
