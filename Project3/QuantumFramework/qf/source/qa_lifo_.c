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

Q_DEFINE_THIS_MODULE(qa_lifo_)

/*..........................................................................*/
/** \ingroup qf
* \file qa_lifo_.c
* \brief QActive_postLIFO__() definition. NOTE: this function is linked
* when the native QF event queue is used (rather than a message queue
* of an RTOS).
*/

/*..........................................................................*/
void QActive_postLIFO__(QActive *me, QEvent const *e) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();

    QS_BEGIN_NOLOCK_(QS_QF_ACTIVE_POST_LIFO, QS_aoObj_, me);
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                  /* this active object */
        QS_U8_(e->attrQF__);               /* the QF attribute of the event */
        QS_EQC_(me->eQueue__.nUsed__);            /* number of used entries */
        QS_EQC_(me->eQueue__.nMax__);         /* max number of used entries */
    QS_END_NOLOCK_();

    if (me->eQueue__.frontEvt__ == (QEvent const *)0) {     /* empty queue? */
        me->eQueue__.frontEvt__ = e;              /* deliver event directly */
        QACTIVE_OSOBJECT_SIGNAL_(me);   /* unblock the active object thread */
                   /* NOTE: the critical section is exited within the macro */
    }
    else {            /* queue is not empty, leave event in the ring-buffer */
            /* the queue must be able to accept the event (cannot overflow) */
        Q_ASSERT(me->eQueue__.nUsed__ < me->eQueue__.nTot__);
        if (me->eQueue__.tail__ == (QEQueueCtr)0) {/*need to wrap the tail? */
            me->eQueue__.tail__ = (QEQueueCtr)(me->eQueue__.end__ - 1);
        }
        else {
            --me->eQueue__.tail__;
        }
        me->eQueue__.ring__[me->eQueue__.tail__] = me->eQueue__.frontEvt__;
        me->eQueue__.frontEvt__ = e;                  /* put event to front */

        ++me->eQueue__.nUsed__;                  /* update number of events */
        if (me->eQueue__.nUsed__ > me->eQueue__.nMax__) {
            me->eQueue__.nMax__ = me->eQueue__.nUsed__; /* store max so far */
        }
        QF_INT_UNLOCK_();
    }
}
