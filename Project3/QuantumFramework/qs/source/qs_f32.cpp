#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QS/C++
// Last Updated for Version: 3.1.06
// Date of the Last Update:  May 06, 2006
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (c) 2002-2006 Quantum Leaps, LLC. All rights reserved.
//
// Internet: www.quantum-leaps.com     Licensing: sales@quantum-leaps.com
//
// This Software is protected by the United States copyright laws and
// international treaties. Distribution of products containing this Software
// or based upon this Software (Derivative Works) requires a valid Quantum
// Leaps Distribution License. Any other distribution, in source or binary
// format is illegal.
//////////////////////////////////////////////////////////////////////////////
#include "qs_pkg.h"

/// \ingroup qs
/// \file qs_f32.cpp
/// \brief QS::f32() implementation

//............................................................................
void QS::f32(uint8_t format, float f) {
    union F32Rep {
        float f;
        uint32_t u;
    } data;
    data.f = f;

    QS_INSERT_ESC_BYTE(format);
    QS_INSERT_ESC_BYTE((uint8_t)data.u)
    data.u >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.u)
    data.u >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.u)
    data.u >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.u)
}
