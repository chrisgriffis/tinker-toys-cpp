//////////////////////////////////////////////////////////////////////////////
// Product: QDPP example
// Version: Compatible with QF/C 3.1.yy
// Updated: Dec 09, 2005
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
// terms of Quantum Leaps commercial licenses, which are designed for users
// who want to retain proprietary status of their code. This "dual-licensing"
// model is possible because Quantum Leaps owns the copyright to this source
// code and as such can license its intelectual property any number of times.
// The users who license this software under one of Quantum Leaps commercial
// licenses do not use this software under the GPL and therefore are not
// subject to any of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_port.h"
#include "qdpp.h"
#include "qassert.h"

Q_DEFINE_THIS_FILE

enum {
    THINK_TIME = 7,
    EAT_TIME = 5,
                                  // internal events (not published to the QF)
    TIMEOUT_SIG = MAX_PUB_SIG
};

//............................................................................
class Philosopher : public QActive {
private:
    uint8_t num_;                                // number of this philosopher
    QTimeEvt timeEvt_;                        // to timeout thinking or eating

public:
    Philosopher::Philosopher()
        : QActive((QState)&Philosopher::initial),
          timeEvt_(TIMEOUT_SIG)
    {}

protected:
    static void initial(Philosopher *me, QEvent const *e);
    static QSTATE thinking(Philosopher *me, QEvent const *e);
    static QSTATE hungry(Philosopher *me, QEvent const *e);
    static QSTATE eating(Philosopher *me, QEvent const *e);
};

//............................................................................
void philosopherStart(uint8_t n, uint8_t prio,
                      QEvent const *qSto[], uint32_t qLen,
                      void *stkSto, uint32_t stkSize)
{
    static Philosopher philo[N];
    TableEvt ie;                                       // initialization event

    Q_REQUIRE(n < N);
    ie.philNum = n;
    philo[n].start(prio,
                   qSto, qLen, stkSto, stkSize,
                   (QEvent *)&ie);
}
//............................................................................
void Philosopher::initial(Philosopher *me, QEvent const *e) {
    me->num_ = ((TableEvt const *)e)->philNum;
    me->subscribe(EAT_SIG);
    Q_INIT(&Philosopher::thinking);
}
//............................................................................
QSTATE Philosopher::thinking(Philosopher *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->timeEvt_.postIn(me, THINK_TIME);
            return 0;
        }
        case TIMEOUT_SIG: {
            Q_TRAN(&Philosopher::hungry);
            return 0;
        }
    }
    return (QSTATE)&QHsm::top;
}
//............................................................................
QSTATE Philosopher::hungry(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            pe = Q_NEW(TableEvt, HUNGRY_SIG);
            pe->philNum = me->num_;
            QDPP_table->postFIFO(pe);
            return 0;
        }
        case EAT_SIG: {
            if (((TableEvt *)e)->philNum == me->num_) {
                Q_TRAN(&Philosopher::eating);
                return 0;
            }
            break;
        }
    }
    return (QSTATE)&QHsm::top;
}
//............................................................................
QSTATE Philosopher::eating(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->timeEvt_.postIn(me, EAT_TIME);
            return 0;
        }
        case TIMEOUT_SIG: {
            Q_TRAN(&Philosopher::thinking);
            return 0;
        }
        case Q_EXIT_SIG: {
            pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philNum = me->num_;
            QF::publish(pe);
            return 0;
        }
    }
    return (QSTATE)&QHsm::top;
}

