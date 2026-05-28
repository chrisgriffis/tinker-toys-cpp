/*****************************************************************************
* Product:  QF/C
* Last Updated for Version: 3.1.03
* Date of the Last Update:  Nov 07, 2005
*
* Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly allow the
* licensees to retain the proprietary status of their code. The licensees
* who use this software under one of Quantum Leaps commercial licenses do
* not use this software under the GPL and therefore are not subject to any
* of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
* Quantum Leaps products:  http://www.quantum-leaps.com/products
* e-mail:                  sales@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qte_ctor)

/*..........................................................................*/
/** \ingroup qf
* \file qte_ctor.c
* \brief QTimeEvt_ctor() implementation.
*/

/*..........................................................................*/
void QTimeEvt_ctor(QTimeEvt *me, QSignal sig) {
    Q_REQUIRE(sig >= (QSignal)Q_USER_SIG);                  /* valid signal */
    me->prev__ = (QTimeEvt *)0;
    me->next__ = (QTimeEvt *)0;
    me->act__  = (QActive *)0;
    me->ctr__  = (QTimeEvtCtr)0;
    me->interval__ = (QTimeEvtCtr)0;
    me->super_.sig = sig;
    me->super_.attrQF__ = (uint8_t)0;/*static event not from a pool, NOTE01 */
}

/*****************************************************************************
* NOTE01:
* Setting attrQF__ to zero is correct only for events not allocated from
* event pools. In the future releases of QF, time events actually could be
* allocated dynamically. However, for simplicity in this release of QF, time
* events are limited to be statically allocated.
*****************************************************************************/
