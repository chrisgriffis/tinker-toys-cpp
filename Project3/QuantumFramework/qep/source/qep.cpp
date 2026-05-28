#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QEP/C++
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
/// \file qep.cpp
/// \brief ::QEP_reservedEvt_ definition and QEP::getVersion() implementation.

// Package-scope objects -----------------------------------------------------
QEvent const QEP_reservedEvt_[] = {
    { (QSignal)QEP_EMPTY_SIG_, (uint8_t)0 },
    { (QSignal)Q_ENTRY_SIG,    (uint8_t)0 },
    { (QSignal)Q_EXIT_SIG,     (uint8_t)0 },
    { (QSignal)Q_INIT_SIG,     (uint8_t)0 }
};
//............................................................................
//lint -e970 -e971               ignore MISRA rules 13 and 14 in this function
char const Q_ROM *QEP::getVersion(void) {
    static char const Q_ROM version[] = "3.2.00";
    return version;
}
