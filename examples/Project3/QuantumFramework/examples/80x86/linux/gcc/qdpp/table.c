/*****************************************************************************
* Product: QDPP example
* Version: Compatible with QF/C 3.1.yy
* Updated: Jan 29, 2006
*
* Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
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
#include "qassert.h"
#include "qdpp.h"

#include <stdlib.h>

Q_DEFINE_THIS_FILE

/* local types -------------------------------------------------------------*/
typedef struct TableTag Table;
struct TableTag {
    QActive super_;
    int fork__[N];
    int isHungry__[N];
};

void Table_ctor(Table *me);
void Table_initial(Table *me, QEvent const *e);
QSTATE Table_serving(Table *me, QEvent const *e);

#define RIGHT(n_) ((uint8_t)(((n_) + (N - 1)) % N))
#define LEFT(n_)  ((uint8_t)(((n_) + 1) % N))
enum { FREE = 0, USED = 1 };

/* local objects -----------------------------------------------------------*/
static Table l_table;

/* Public-scope objects ----------------------------------------------------*/
QActive *QDPP_table = (QActive *)&l_table;

/*..........................................................................*/
void tableStart(uint8_t prio,
                QEvent const *qSto[], uint32_t qLen,
                void *stkSto, uint32_t stkSize)
{
    Table_ctor(&l_table);                             /* explicit Ctor call */
    QActive_start((QActive *)&l_table, prio,
                  qSto, qLen, stkSto, stkSize,
                  (QEvent *)0);
}
/*..........................................................................*/
void Table_ctor(Table *me) {
    QActive_ctor_(&me->super_, (QState)&Table_initial);
}
/*..........................................................................*/
void Table_initial(Table *me, QEvent const *e) {
    uint8_t n;
    (void)e;                               /* suppress the compiler warning */
    QActive_subscribe_((QActive *)me, HUNGRY_SIG);
    QActive_subscribe_((QActive *)me, DONE_SIG);
    QActive_subscribe_((QActive *)me, TERMINATE_SIG);
    for (n = 0; n < N; ++n) {
        me->fork__[n] = FREE;
        me->isHungry__[n] = 0;
    }
    Q_INIT(&Table_serving);
}
/*..........................................................................*/
QSTATE Table_serving(Table *me, QEvent const *e) {
    uint8_t n, m;
    TableEvt *pe;

    switch (e->sig) {
        case HUNGRY_SIG: {
            n = ((TableEvt *)e)->philNum;
            Q_ASSERT(n < N && !me->isHungry__[n]);
            displyPhilStat(n, "hungry  ");
            m = LEFT(n);
            if (me->fork__[m] == FREE && me->fork__[n] == FREE) {
                me->fork__[m] = me->fork__[n] = USED;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philNum = n;
                QF_publish((QEvent *)pe);
                displyPhilStat(n, "eating  ");
            }
            else {
                me->isHungry__[n] = 1;
            }
            return 0;
        }
        case DONE_SIG: {
            n = ((TableEvt *)e)->philNum;
            Q_ASSERT(n < N);
            displyPhilStat(n, "thinking");
            me->fork__[LEFT(n)] = me->fork__[n] = FREE;
            m = RIGHT(n);                       /* check the right neighbor */
            if (me->isHungry__[m] && me->fork__[m] == FREE) {
                me->fork__[n] = me->fork__[m] = USED;
                me->isHungry__[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philNum = m;
                QF_publish((QEvent *)pe);
                displyPhilStat(m, "eating  ");
            }
            m = LEFT(n);                         /* check the left neighbor */
            n = LEFT(m);
            if (me->isHungry__[m] && me->fork__[n] == FREE) {
                me->fork__[m] = me->fork__[n] = USED;
                me->isHungry__[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philNum = m;
                QF_publish((QEvent *)pe);
                displyPhilStat(m, "eating  ");
            }
            return 0;
        }
        case TERMINATE_SIG: {
            QF_exit();
            return 0;
        }
    }
    return (QSTATE)&QHsm_top;
}
