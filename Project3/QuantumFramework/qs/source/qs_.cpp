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
/// \file qs_.cpp
/// \brief QS functions for internal use inside QP components

//............................................................................
void const *QS::smObj_;                  // local state machine for QEP filter
void const *QS::aoObj_;                   // local active object for QF filter
void const *QS::mpObj_;                     //  local event pool for QF filter
void const *QS::eqObj_;                      //  local raw queue for QF filter
void const *QS::teObj_;                     //  local time event for QF filter
void const *QS::apObj_;                    //  local object Application filter

//............................................................................
void QS::u8_(uint8_t data) {
    QS_INSERT_ESC_BYTE(data);
}
//............................................................................
void QS::u16_(uint16_t data) {
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
}
//............................................................................
void QS::u32_(uint32_t data) {
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
}
//............................................................................
//lint -e970 -e971               ignore MISRA rules 13 and 14 in this function
void QS::str_(char const *s) {
    while (*s != '\0') {
                                       // ASCII characters don't need escaping
        QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)*s);
        QS_INSERT_BYTE((uint8_t)*s)
        ++s;
    }
    QS_INSERT_BYTE((uint8_t)0);
}
