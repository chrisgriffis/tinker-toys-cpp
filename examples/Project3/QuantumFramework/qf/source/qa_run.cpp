#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.1.03
// Date of the Last Update:  Nov 14, 2005
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
/// \file qa_run.cpp
/// \brief QActive::run() implementation.

//............................................................................
void QActive::run(void) {
    running_ = (uint8_t)1;                     // allow the thread-loop to run
    do {
        QEvent const *e;

        QACTIVE_GET_(this, e);                               // wait for event

        QF_ACTIVE_SUPER_::dispatch(e);   // dispatch to the active object's SM

        QF::gc(e);      // check if the event is garbage, and collect it if so

    } while (running_);   // loop until running_ is cleared in QActive::stop()

    unsubscribeAll();                          // unsubscribe from all signals
    QF::remove_(this);            // remove this object from any subscriptions
}
