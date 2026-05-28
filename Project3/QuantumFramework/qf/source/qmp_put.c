/*****************************************************************************
* Product:  QF/C
* Last Updated for Version: 3.1.04
* Date of the Last Update:  Dec 01, 2005
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

Q_DEFINE_THIS_MODULE(qmp_put)

/*..........................................................................*/
/** \ingroup qf
* \file qmp_put.c
* \brief QMPool_put() implementation.
*/

/*..........................................................................*/
void QMPool_put(QMPool *me, void *b) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    Q_INVARIANT(me->nFree__ < me->nTot__); /* # free blocks must be < total */

    /*lint -e946                 ignore MISRA Rule 103 in this precondition */
    Q_REQUIRE((me->start__ <= b) && (b <= me->end__)); /*  must be in range */

    ((QFreeBlock *)b)->next = (QFreeBlock *)me->free__;/*link into free list*/
    me->free__ = b;                     /* set as new head of the free list */
    ++me->nFree__;                      /* one more free block in this pool */

    QS_BEGIN_NOLOCK_(QS_QF_MPOOL_PUT, QS_mpObj_, me->start__);
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(me->start__);            /* the memory managed by this pool */
        QS_MPC_(me->nFree__);      /* the number of free blocks in the pool */
    QS_END_NOLOCK_();

    QF_INT_UNLOCK_();
}
