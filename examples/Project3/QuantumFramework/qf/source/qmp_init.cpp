#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.1.04
// Date of the Last Update:  Dec 06, 2005
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

Q_DEFINE_THIS_MODULE(qmp_init)

/// \ingroup qf
/// \file qmp_init.cpp
/// \brief QMPool::init() implementation.

//............................................................................
void QMPool::init(void *poolSto, uint32_t poolSize, QMPoolSize blockSize) {
    QFreeBlock *fb = (QFreeBlock *)poolSto;

        // the blockSize must not be too close to the top of the dynamic range
    Q_REQUIRE((QMPoolSize)(blockSize + (QMPoolSize)sizeof(QFreeBlock))
              > blockSize);

                // round up the blockSize to fit an integer number of pointers
    blockSize_ = (QMPoolSize)sizeof(QFreeBlock);        // start with just one
            // the number of QFreeBlock* pointers that fit in one memory block
    QMPoolSize n = (QMPoolSize)1;
    while (blockSize_ < blockSize) {
        blockSize_ += (QMPoolSize)sizeof(QFreeBlock);
        ++n;
    }
    blockSize = blockSize_;           // use the rounded-up value from here on

                          // the whole pool buffer must fit at least one block
    Q_ASSERT(poolSize >= (uint32_t)blockSize);

    start_  = poolSto;    // remember the start of memory managed by this pool
    free_ = poolSto;             // set the head of linked-list of free blocks

                                // chain all blocks together in a free-list...
    poolSize -= (uint32_t)blockSize;             // don't chain the last block
    nTot_ = (QMPoolCtr)1;                  // one (the last) block in the pool
    while (poolSize >= (uint32_t)blockSize) {
        fb->next = &fb[n];                              // setup the next link
        fb = fb->next;                                // advance to next block
        poolSize -= (uint32_t)blockSize;
        ++nTot_;
    }
    end_ = fb;  // set the end of memory managed by this pool (the last block)

    fb->next = (QFreeBlock *)0;                   // the last link points to 0
    nFree_ = nTot_;                             // store number of free blocks
    nMin_ = nTot_;                        // the minimum number of free blocks

    QS_BEGIN_(QS_QF_MPOOL_INIT, QS::mpObj_, start_);
        QS_OBJ_(start_);                    // the memory managed by this pool
        QS_MPC_(nTot_);                          // the total number of blocks
    QS_END_();
}
