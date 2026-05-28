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

Q_DEFINE_THIS_MODULE(qte_rarm)

/*..........................................................................*/
/** \ingroup qf
* \file qte_rarm.c
* \brief QTimeEvt_rearm() implementation.
*/

/*..........................................................................*/
uint8_t QTimeEvt_rearm(QTimeEvt *me, QTimeEvtCtr nTicks) {
    uint8_t isArmed;
    QF_INT_LOCK_KEY_
    Q_REQUIRE((nTicks > (QTimeEvtCtr)0)        /* cannot rearm with 0 ticks */
              && (me->super_.sig >= (QSignal)Q_USER_SIG));  /* valid signal */
    QF_INT_LOCK_();
    me->ctr__ = nTicks;
    if (me->prev__ == (QTimeEvt *)0) {      /* is this time event disarmed? */
        isArmed = (uint8_t)0;
        me->next__ = QF_timeEvtListHead_;
        if (QF_timeEvtListHead_ != (QTimeEvt *)0) {
            QF_timeEvtListHead_->prev__ = me;
        }
        QF_timeEvtListHead_ = me;
        me->prev__ = me;                      /* mark the time event in use */
    }
    else {                                       /* the time event is armed */
        isArmed = (uint8_t)1;
    }

    QS_BEGIN_NOLOCK_(QS_QF_TIMEEVT_REARM, QS_teObj_, me);
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(me);                              /* this time event object */
        QS_OBJ_(me->act__);                            /* the active object */
        QS_TEC_(me->ctr__);                          /* the number of ticks */
        QS_TEC_(me->interval__);                            /* the interval */
        QS_U8_(isArmed);                            /* was the timer armed? */
    QS_END_NOLOCK_();

    QF_INT_UNLOCK_();
    return isArmed;
}
