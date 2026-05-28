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

/*..........................................................................*/
/** \ingroup qf
* \file qf_time.c
* \brief QF_tickCtr_ definition and QF_getTime() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
QTimeEvtCtr QF_tickCtr_;                  /* counter incremented every tick */

/*..........................................................................*/
QTimeEvtCtr QF_getTime(void) {                                /* see NOTE01 */
    QTimeEvtCtr tickCtr;
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
    tickCtr = QF_tickCtr_;
    QF_INT_UNLOCK_();
    return tickCtr;
}

/*****************************************************************************
* NOTE01: The value returned by QF_getTime() has no meaningful absolute value
* but is meaningful only relatively to the values returned at other time
* instances. The internal QF_tickCtr_ counter wraps around through 0xFFFFFFFF
* to 0, which is transparent to any time difference calculations.
*
* For example, a time
* difference calculated accross the wrap around might look as follows:
*
* t1 == 0xFFFFFFFB (QF_getTime() value returned at instance 1)
* t2 == 0x00000003 (QF_getTime() value returned 8 ticks after instance 1)
*
* in the 2-complement unsigned arithmetic the difference is still 8 ticks,
* even across the wrap-around point:
*
* dt = t2 - t1 == 0x00000003 - 0xFFFFFFFB == 8
*
*****************************************************************************/
