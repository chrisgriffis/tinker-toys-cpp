/*****************************************************************************
* Product: QDPP example
* Version: Compatible with QF/C 3.1.yy
* Updated: Nov 24, 2005
*
* Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
*
* This example is part of the Quantum Leaps QP/C software, and may be
* distributed and modified under the terms of the GNU General Public License
* version 2 (GPL) as published by the Free Software Foundation and appearing
* in the file GPL.TXT included in the packaging of this file. Please note
* that GPL Section 2[b] requires that all works based on this software must
* also be made publicly available under the terms of the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified in conjunction
* with a valid QP/C Quantum Leaps commercial license. Quantum Leaps
* commercial licenses are designed for users who want to retain proprietary
* status of their code. The users who license this software under one of
* Quantum Leaps commercial licenses do not use this software under the GPL
* and therefore are not subject to any of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
* Quantum Leaps products:  http://www.quantum-leaps.com/products
* e-mail:                  sales@quantum-leaps.com
*****************************************************************************/
#include "qf_port.h"
#include "qdpp.h"
#include "qassert.h"

#include <stdlib.h>

Q_DEFINE_THIS_FILE

typedef struct PhilosopherTag Philosopher;
struct PhilosopherTag {
    QActive super_;
    uint8_t num__;                            /* number of this philosopher */
    QTimeEvt timeEvt__;                     /* to timeout thining or eating */
};

void Philosopher_ctor(Philosopher *me);
void Philosopher_initial(Philosopher *me, QEvent const *e);
QSTATE Philosopher_thinking(Philosopher *me, QEvent const *e);
QSTATE Philosopher_hungry(Philosopher *me, QEvent const *e);
QSTATE Philosopher_eating(Philosopher *me, QEvent const *e);


enum {
    THINK_TIME = 7,
    EAT_TIME = 5,

    /* internal events (not published to the QF) */
    TIMEOUT_SIG = MAX_PUB_SIG
};

/*..........................................................................*/
void philosopherStart(uint8_t n, uint8_t prio,
                      QEvent const *qSto[], uint32_t qLen,
                      void *stkSto, uint32_t stkSize)
{
    static Philosopher philo[N];
    TableEvt ie;                                    /* initialization event */

    Q_REQUIRE(n < N);

    Philosopher_ctor(&philo[n]);                      /* explicit Ctor call */
    ie.philNum = n;
    QActive_start((QActive *)&philo[n], prio,
                  qSto, qLen, stkSto, stkSize,
                  (QEvent *)&ie);
}
/*..........................................................................*/
void Philosopher_ctor(Philosopher *me) {
    QActive_ctor_(&me->super_, (QState)&Philosopher_initial);
    QTimeEvt_ctor(&me->timeEvt__, TIMEOUT_SIG);
}
/*..........................................................................*/
void Philosopher_initial(Philosopher *me, QEvent const *e) {
    me->num__ = ((TableEvt const *)e)->philNum;
    QActive_subscribe_((QActive *)me, EAT_SIG);
    Q_INIT(&Philosopher_thinking);
}
/*..........................................................................*/
QSTATE Philosopher_thinking(Philosopher *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_postIn(&me->timeEvt__, (QActive *)me, THINK_TIME);
            return 0;
        }
        case TIMEOUT_SIG: {
            busyDelay();
            Q_TRAN(&Philosopher_hungry);
            return 0;
        }
    }
    return (QSTATE)&QHsm_top;
}
/*..........................................................................*/
QSTATE Philosopher_hungry(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            pe = Q_NEW(TableEvt, HUNGRY_SIG);
            pe->philNum = me->num__;
            QActive_postFIFO(QDPP_table, (QEvent *)pe);
            return 0;
        }
        case EAT_SIG: {
            if (((TableEvt *)e)->philNum == me->num__) {
                busyDelay();
                Q_TRAN(&Philosopher_eating);
            }
            return 0;
        }
    }
    return (QSTATE)&QHsm_top;
}
/*..........................................................................*/
QSTATE Philosopher_eating(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_postIn(&me->timeEvt__, (QActive *)me, EAT_TIME);
            return 0;
        }
        case TIMEOUT_SIG: {
            Q_TRAN(&Philosopher_thinking);
            return 0;
        }
        case Q_EXIT_SIG: {
            busyDelay();
            pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philNum = me->num__;
            QF_publish((QEvent *)pe);
            return 0;
        }
    }
    return (QSTATE)&QHsm_top;
}

