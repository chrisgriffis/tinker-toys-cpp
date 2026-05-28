//////////////////////////////////////////////////////////////////////////////
// Product: QDPP example
// Version: Compatible with QF/C 3.1.yy
// Updated: Dec 11, 2005
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
// Alternatively, this software may be distributed and modified in conjunction
// with a valid QP/C Quantum Leaps commercial license. Quantum Leaps
// commercial licenses are designed for users who want to retain proprietary
// status of their code. The users who license this software under one of
// Quantum Leaps commercial licenses do not use this software under the GPL
// and therefore are not subject to any of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_port.h"
#include "qassert.h"
#include "qdpp.h"

#include <stdlib.h>

Q_DEFINE_THIS_FILE

// local objects -------------------------------------------------------------
class Table : public QActive {
private:
    uint8_t fork_[N];
    uint8_t isHungry_[N];

public:
    Table::Table() : QActive((QState)&Table::initial) {
    }

protected:
    static void initial(Table *me, QEvent const *e);
    static QSTATE serving(Table *me, QEvent const *e);

    #define RIGHT(n_) ((uint8_t)(((n_) + (N - 1)) % N))
    #define LEFT(n_)  ((uint8_t)(((n_) + 1) % N))
    enum { FREE = 0, USED = 1 };
};

// Local objects -------------------------------------------------------------
static Table l_table;                                    // local Table object

// Public-scope objects ------------------------------------------------------
QActive *QDPP_table = &l_table;                        // global Table pointer

//............................................................................
void tableStart(uint8_t prio,
                QEvent const *qSto[], uint32_t qLen,
                void *stkSto, uint32_t stkSize)
{
    l_table.start(prio,
                  qSto, qLen, stkSto, stkSize,
                  (QEvent *)0);
}
//............................................................................
void Table::initial(Table *me, QEvent const *) {
    uint8_t n;
    me->subscribe(HUNGRY_SIG);
    me->subscribe(DONE_SIG);
    me->subscribe(TERMINATE_SIG);
    me->subscribe(STOP_SIG);
    for (n = 0; n < N; ++n) {
        me->fork_[n] = FREE;
        me->isHungry_[n] = 0;
    }
    Q_INIT(&Table::serving);
}
//............................................................................
QSTATE Table::serving(Table *me, QEvent const *e) {
    uint8_t n, m;
    TableEvt *pe;

    switch (e->sig) {
        case HUNGRY_SIG: {
            n = ((TableEvt *)e)->philNum;
            Q_ASSERT(n < N && !me->isHungry_[n]);
            displyPhilStat(n, "hungry  ");
            m = LEFT(n);
            if (me->fork_[m] == FREE && me->fork_[n] == FREE) {
                me->fork_[m] = me->fork_[n] = USED;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philNum = n;
                QF::publish(pe);
                displyPhilStat(n, "eating  ");
            }
            else {
                me->isHungry_[n] = 1;
            }
            return (QSTATE)0;
        }
        case DONE_SIG: {
            n = ((TableEvt *)e)->philNum;
            Q_ASSERT(n < N);
            displyPhilStat(n, "thinking");
            me->fork_[LEFT(n)] = me->fork_[n] = FREE;
            m = RIGHT(n);                          // check the right neighbor
            if (me->isHungry_[m] && me->fork_[m] == FREE) {
                me->fork_[n] = me->fork_[m] = USED;
                me->isHungry_[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philNum = m;
                QF::publish(pe);
                displyPhilStat(m, "eating  ");
            }
            m = LEFT(n);                            // check the left neighbor
            n = LEFT(m);
            if (me->isHungry_[m] && me->fork_[n] == FREE) {
                me->fork_[m] = me->fork_[n] = USED;
                me->isHungry_[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philNum = m;
                QF::publish(pe);
                displyPhilStat(m, "eating  ");
            }
            return (QSTATE)0;
        }
        case STOP_SIG: {
            n = ((TableEvt *)e)->philNum;
            displyPhilStat(n, "stopped");
            return (QSTATE)0;
        }
        case TERMINATE_SIG: {
            QF::exit();
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsm::top;
}
