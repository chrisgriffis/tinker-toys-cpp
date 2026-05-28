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
/// \file qs_byte.cpp
/// \brief QS::getByte() implementation

//............................................................................
uint16_t QS::getByte(void) {
    if (QS_used_ == (QSCtr)0) {
        return QS_EOD;                                   // return End-Of-Data
    }
    uint8_t byte = QS_ring_[QS_tail_];               // set the byte to return
    ++QS_tail_;                                            // advance the tail
    if (QS_tail_ == QS_end_) {                            // tail wrap around?
        QS_tail_ = (QSCtr)0;
    }
    --QS_used_;                                          // one less byte used
    return (uint16_t)byte;                                  // return the byte
}
