/*****************************************************************************
* Product:  QF/C
* Last Updated for Version: 3.1.03
* Date of the Last Update:  Nov 14, 2005
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

Q_DEFINE_THIS_MODULE(qa_lifo)

/*..........................................................................*/
/** \ingroup qf
* \file qa_lifo.c
* \brief QActive_postLIFO() definition.
*/

/*..........................................................................*/
void QActive_postLIFO(QActive *me, QEvent const *e) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
    if (e->attrQF__ != (uint8_t)0) {                 /* is it a pool event? */
        ++((QEvent *)e)->attrQF__;       /* increment the reference counter */
                   /* NOTE: cast the 'const' away, which is legitimate because
                    * it's a pool event
                    */
    }
    QF_INT_UNLOCK_();

    QACTIVE_POST_LIFO_(me, e);            /* asserts if the queue overflows */
}
