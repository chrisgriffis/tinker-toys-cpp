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

Q_DEFINE_THIS_MODULE(qte_ctor)

/// \ingroup qf
/// \file qte_ctor.cpp
/// \brief QTimeEvt::QTimeEvt() implementation.

//............................................................................
QTimeEvt::QTimeEvt(QSignal s)
    : prev_((QTimeEvt *)0),
      next_((QTimeEvt *)0),
      act_((QActive *)0),
      ctr_((QTimeEvtCtr)0),
      interval_((QTimeEvtCtr)0)
{
    Q_REQUIRE(s >= (QSignal)Q_USER_SIG);                       // valid signal
    sig = s;
    attrQF__ = (uint8_t)0;         // static event not from a pool, see NOTE01
}

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// Setting attrQF__ to zero is correct only for events not allocated from
// event pools. In the future releases of QF, time events actually could be
// allocated dynamically. However, for simplicity in this release of QF, time
// events are limited to be statically allocated.
//////////////////////////////////////////////////////////////////////////////
