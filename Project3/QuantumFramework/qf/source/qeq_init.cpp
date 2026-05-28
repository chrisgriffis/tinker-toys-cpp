#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.1.05
// Date of the Last Update:  Feb 10, 2006
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
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_pkg.h"

/// \ingroup qf
/// \file qeq_init.cpp
/// \brief QEQueue::init() implementation.

//............................................................................
void QEQueue::init(QEvent const *qSto[], QEQueueCtr qLen) {
    ring_     = &qSto[0];
    nTot_     = qLen;
    end_      = nTot_;
    head_     = (QEQueueCtr)0;
    tail_     = (QEQueueCtr)0;
    nUsed_    = (QEQueueCtr)0;
    nMax_     = (QEQueueCtr)0;
    frontEvt_ = (QEvent const *)0;                   // no events in the queue

    QS_BEGIN_(QS_QF_EQUEUE_INIT, QS::eqObj_, this);
        QS_OBJ_(qSto);                                  // this QEQueue object
        QS_EQC_(qLen);                              // the length of the queue
    QS_END_();
}
