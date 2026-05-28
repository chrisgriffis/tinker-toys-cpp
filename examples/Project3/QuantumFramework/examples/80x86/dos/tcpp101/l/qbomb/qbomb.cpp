//////////////////////////////////////////////////////////////////////////////
// Product: QBomb Example, DOS, Large model, Turbo C++ 1.01
// Version: Compatible with QEP/C++ 3.x.yy
// Updated: Sep 21, 2005
//
// Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
//
// This example is part of the Quantum Leaps QP/C++ software, and may be
// distributed and modified under the terms of the GNU General Public License
// version 2 (GPL) as published by the Free Software Foundation and appearing
// in the file GPL.TXT included in the packaging of this file. Please note
// that GPL Section 2[b] requires that all works based on this software must
// also be made publicly available under the terms of the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified in conjunction
// with a valid QP/C++ Quantum Leaps commercial license. Quantum Leaps
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
#include "qep_port.h"
#include "qbomb.h"
#include "qassert.h"

Q_DEFINE_THIS_FILE

//----------------------------------------------------------------------------
enum {
    MIN_TIMEOUT = 1,
    MAX_TIMEOUT = 99,
    INIT_TIMEOUT = 5
};

//............................................................................
void QBomb::initial(QBomb *me, QEvent const *e) {
    Q_REQUIRE(e != (QEvent const *)0);        // initialization event expected
    me->updateState("top-INIT");
    me->timeout_ = INIT_TIMEOUT;
    me->defuse_ = ((QBombInitEvt const *)e)->defuse;
    Q_INIT(&QBomb::setting);
}
//............................................................................
void QBomb::setting(QBomb *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("setting-ENTRY");
            break;
        }
        case Q_EXIT_SIG: {
            me->updateState("setting-EXIT");
            break;
        }
        case UP_SIG: {
            if (me->timeout_ < MAX_TIMEOUT) {
                ++me->timeout_;
            }
            break;
        }
        case DOWN_SIG: {
            if (me->timeout_ > MIN_TIMEOUT) {
                --me->timeout_;
            }
            break;
        }
        case ARM_SIG: {
            Q_TRAN(&QBomb::timing);
            break;
        }
    }
}
//............................................................................
void QBomb::timing(QBomb *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("timing-ENTRY");
            me->code_ = (uint8_t)0;              // invalidate the defuse code
            break;
        }
        case Q_EXIT_SIG: {
            me->updateState("timing-EXIT");
            break;
        }
        case TICK_SIG: {
            if (me->timeout_ > (uint8_t)0) {
                --me->timeout_;
            }
            else {                                          // timeout expired
                Q_TRAN(&QBomb::blast);
            }
            break;
        }
        case UP_SIG: {
            me->code_ = (uint8_t)((me->code_ << 1) | 1);
            break;
        }
        case DOWN_SIG: {
            me->code_ <<= 1;
            break;
        }
        case ARM_SIG: {
            if (me->code_ == me->defuse_) {
                Q_TRAN(&QBomb::setting);
            }
            break;
        }
    }
}
//............................................................................
void QBomb::blast(QBomb *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("blast-ENTRY");
            me->exit();
            break;
        }
    }
}

//............................................................................
uint8_t QBomb::getTimeout(void) {
    return timeout_;
}
