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
/// \file qf_pwr2.cpp
/// \brief QF::pwr2Lkup[], QF::invPwr2Lkup[], and QF::div8Lkup[] definitions.

// Global objects ------------------------------------------------------------
uint8_t const QF::pwr2Lkup[65] = {
    0x00,                                                   // unused location
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

uint8_t const QF::invPwr2Lkup[65] = {
    0xFF,                                                   // unused location
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F
};

uint8_t const QF::div8Lkup[65] = {
    0,                                                      // unused location
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7
};
