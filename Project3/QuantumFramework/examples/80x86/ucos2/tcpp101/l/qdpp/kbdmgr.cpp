//////////////////////////////////////////////////////////////////////////////
// Product: QDPP, for 80x86, uC/OS-II, Turbo C++ 1.01, Large model
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

#include <stdlib.h>
#include <stdio.h>

Q_DEFINE_THIS_FILE

class KbdMgr : public QActive {                            // keyboard manager
public:
    KbdMgr() : QActive((QState)&KbdMgr::initial) {
    }

protected:
    static void initial(KbdMgr *me, QEvent const *e);
    static QSTATE active(KbdMgr *me, QEvent const *e);
};

//............................................................................
void kbdMgrStart(uint8_t prio, QEvent const *qSto[], uint32_t qLen,
                 void *stkSto, uint32_t stkSize)
{
    static KbdMgr kbdMgr;
    kbdMgr.start(prio, qSto, qLen, stkSto, stkSize, (QEvent *)0);
}
//............................................................................
void KbdMgr::initial(KbdMgr *me, QEvent const *) {
    me->subscribe(KBD_SIG);
    me->postFIFO(Q_NEW(QEvent, START_SIG));  // post a start event, see NOTE01
    Q_INIT(&KbdMgr::active);
}
//............................................................................
QSTATE KbdMgr::active(KbdMgr *me, QEvent const *e) {
    switch (e->sig) {
        case START_SIG: {
            QF::start();                  // enable the interrupts, see NOTE01
            return (QSTATE)0;
        }
        case KBD_SIG: {
            displayKey(((KbdEvt const *)e)->key);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsm::top;
}

//////////////////////////////////////////////////////////////////////////////
//
// NOTE01:
// uC/OS-II requires that the interrupts be not enabled until the initial
// context switch occurrs in OSInit(). To arrange for that, an event
// is posted to self and waits in the event queue until the KbdMgr task
// is scheduled in by the uC/OS-II. Only then the internal transition
// triggered by the event calls the QF::start() method that initializes
// and enables the interrupts.
//