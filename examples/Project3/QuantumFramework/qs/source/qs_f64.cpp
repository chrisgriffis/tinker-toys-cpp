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
/// \file qs_f64.cpp
/// \brief QS::f64() implementation

//............................................................................
void QS::f64(uint8_t format, double d) {
    union F64Rep {
        double d;
        struct UInt2 {
            uint32_t u1, u2;
        } i;
    } data;
    data.d = d;

    QS_INSERT_ESC_BYTE(format);

    QS_INSERT_ESC_BYTE((uint8_t)data.i.u1)
    data.i.u1 >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.i.u1)
    data.i.u1 >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.i.u1)
    data.i.u1 >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.i.u1)

    QS_INSERT_ESC_BYTE((uint8_t)data.i.u2)
    data.i.u2 >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.i.u2)
    data.i.u2 >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.i.u2)
    data.i.u2 >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data.i.u2)
}
