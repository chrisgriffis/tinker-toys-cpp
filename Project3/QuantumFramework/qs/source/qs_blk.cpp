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
/// \file qs_blk.cpp
/// \brief QS::getBlock() implementation

//............................................................................
// get up to *pn bytes of contiguous memory
uint8_t const *QS::getBlock(uint16_t *pNbytes) {
    if (QS_used_ == (QSCtr)0) {
        *pNbytes = (uint16_t)0;
        return (uint8_t *)0;                   // no bytes to return right now
    }
    QSCtr n = (QSCtr)(QS_end_ - QS_tail_);
    if (n > QS_used_) {
        n = QS_used_;
    }
    if (n > (QSCtr)(*pNbytes)) {
        n = (QSCtr)(*pNbytes);
    }
    *pNbytes = (uint16_t)n;
    QS_used_ = (QSCtr)(QS_used_ - n);
    QSCtr t = QS_tail_;
    QS_tail_ = (QSCtr)(QS_tail_ + n);
    if (QS_tail_ == QS_end_) {
        QS_tail_ = (QSCtr)0;
    }
    return &QS_ring_[t];
}
