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
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qmp_get)

/// \ingroup qf
/// \file qmp_get.cpp
/// \brief QMPool::get() and QF::getPoolMargin() implementation.

//............................................................................
void *QMPool::get(void) {
    QFreeBlock *fb;
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    if (nFree_ > (QMPoolCtr)0) {                      // free block available?
        fb = (QFreeBlock *)free_;                        // get the free block
        free_ = fb->next;           // adjust list head to the next free block
        if (--nFree_ < nMin_) {                              // one less block
            nMin_ = nFree_;                     // remember the minimum so far
        }
    }
    else {
        fb = (QFreeBlock *)0;                       // no free block available
    }

    QS_BEGIN_NOLOCK_(QS_QF_MPOOL_GET, QS::mpObj_, start_);
        QS_TIME_();                                               // timestamp
        QS_OBJ_(start_);                    // the memory managed by this pool
        QS_MPC_(nFree_);              // the number of free blocks in the pool
        QS_MPC_(nMin_);      // the mninimum number of free blocks in the pool
    QS_END_NOLOCK_();

    QF_INT_UNLOCK_();
    return fb;               // return the block or NULL pointer to the caller
}
//............................................................................
uint32_t QF::getPoolMargin(uint8_t poolId) {
    Q_REQUIRE((1 <= poolId) && (poolId <= QF_maxPool_));

    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
    uint32_t margin = (uint32_t)QF_pool_[poolId - 1].nMin_;
    QF_INT_UNLOCK_();

    return margin;
}
