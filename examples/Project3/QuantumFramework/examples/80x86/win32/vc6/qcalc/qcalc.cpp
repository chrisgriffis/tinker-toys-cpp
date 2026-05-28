//////////////////////////////////////////////////////////////////////////////
// Product: Quantum Calculator Example
// Version: Compatible with QEP/C++ 3.x.yy
// Updated: Oct 21, 2005
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
#include "qcalc.h"
#include "qassert.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Q_DEFINE_THIS_FILE

#define KEY_UNKNOWN '?'
#define KEY_PLUS    '+'
#define KEY_MINUS   '-'
#define KEY_MULT    '*'
#define KEY_DIVIDE  '/'

//............................................................................
void QCalc::clear(void) {
    memset(display_, ' ', DISP_WIDTH - 1);
    display_[DISP_WIDTH - 1] = '0';
    display_[DISP_WIDTH] = '\0';
    len_ = 0;
}
//............................................................................
void QCalc::insert(int keyId) {
    if (len_ == 0) {
        display_[DISP_WIDTH - 1] = (char)keyId;
        ++len_;
    }
    else if (len_ < (DISP_WIDTH - 1)) {
        memmove(&display_[0], &display_[1], DISP_WIDTH - 1);
        display_[DISP_WIDTH - 1] = (char)keyId;
        ++len_;
    }
    else {
    }
}
//............................................................................
void QCalc::negate(void) {
    clear();
    display_[DISP_WIDTH - 2] = '-';
}
//............................................................................
uint8_t QCalc::eval(void) {
    uint8_t ok = (uint8_t)1;
    double result = 0.0;
    switch (opKey_) {
        case KEY_PLUS: {
            result = operand1_ + operand2_;
            break;
        }
        case KEY_MINUS: {
            result = operand1_ - operand2_;
            break;
        }
        case KEY_MULT: {
            result = operand1_ * operand2_;
            break;
        }
        case KEY_DIVIDE: {
            if ((operand2_ < -1e-10) || (1e-10 < operand2_)) {
                result = operand1_ / operand2_;
            }
            else {
                strcpy(display_, "Error 0");                 // divide by zero
                ok = 0;
            }
            break;
        }
        default: {
            Q_ERROR();
            break;
        }
    }
    if (ok) {
        if ((-1.0e10 < result) && (result < 1.0e10)) {
            sprintf(display_, "%14.11g", result);
        }
        else {
            strcpy(display_, "Error 1");                // result out of range
            ok = 0;
        }
    }

    return ok;
}
// HSM definition ------------------------------------------------------------
void QCalc::initial(QCalc *me, QEvent const *) {
    me->operand1_ = 0.0;
    me->operand2_ = 0.0;
    me->opKey_ = KEY_UNKNOWN;
    me->clear();
    Q_INIT(&QCalc::on);
}
//............................................................................
QSTATE QCalc::on(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("on-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("on-EXIT");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->updateState("on-INIT");
            Q_INIT(&QCalc::ready);
            return (QSTATE)0;
        }
        case C_SIG: {
            me->clear();
            Q_TRAN(&QCalc::on);                          // transition to self
            return (QSTATE)0;
        }
        case TERMINATE_SIG: {
            me->exit();
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsm::top;
}
//............................................................................
QSTATE QCalc::error(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("error-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("error-EXIT");
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::ready(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("ready-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("ready-EXIT");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->updateState("ready-INIT");
            Q_INIT(&QCalc::begin);
            return (QSTATE)0;
        }
        case DIGIT_0_SIG: {
            me->clear();
            Q_TRAN(&QCalc::zero1);
            return (QSTATE)0;
        }
        case DIGIT_1_9_SIG: {
            me->clear();
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::int1);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->clear();
            me->insert((int)'0');
            me->insert((int)'.');
            Q_TRAN(&QCalc::frac1);
            return (QSTATE)0;
        }
        case OPER_SIG: {
            me->operand1_ = strtod(me->display_, (char **)0);
            me->opKey_ = ((QCalcEvt *)e)->keyId;
            Q_TRAN(&QCalc::opEntered);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::result(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("result-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("result-EXIT");
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::ready;
}
//............................................................................
QSTATE QCalc::begin(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("begin-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("begin-EXIT");
            return (QSTATE)0;
        }
        case OPER_SIG: {
            if (((QCalcEvt *)e)->keyId == KEY_MINUS) {
                Q_TRAN(&QCalc::negated1);
                return (QSTATE)0;                             // event handled
            }
            break;
        }
    }
    return (QSTATE)&QCalc::ready;
}
//............................................................................
QSTATE QCalc::negated1(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("negated1-ENTRY");
            me->negate();
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("negated1-EXIT");
            return (QSTATE)0;
        }
        case CE_SIG: {
            me->clear();
            Q_TRAN(&QCalc::begin);
            return (QSTATE)0;
        }
        case DIGIT_0_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::zero1);
            return (QSTATE)0;
        }
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::int1);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::frac1);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::negated2(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("negated2-ENTRY");
            me->negate();
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("negated2-EXIT");
            return (QSTATE)0;
        }
        case CE_SIG: {
            Q_TRAN(&QCalc::opEntered);
            return (QSTATE)0;
        }
        case DIGIT_0_SIG: {
            Q_TRAN(&QCalc::zero2);
            return (QSTATE)0;
        }
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::int2);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::frac2);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::operand1(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("operand1-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("operand1-EXIT");
            return (QSTATE)0;
        }
        case CE_SIG: {
            me->clear();
            Q_TRAN(&QCalc::begin);
            return (QSTATE)0;
        }
        case OPER_SIG: {
            me->operand1_ = strtod(me->display_, (char **)0);
            me->opKey_ = ((QCalcEvt *)e)->keyId;
            Q_TRAN(&QCalc::opEntered);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::zero1(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("zero1-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("zero1-EXIT");
            return (QSTATE)0;
        }
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::int1);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::frac1);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::int1(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("int1-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("int1-EXIT");
            return (QSTATE)0;
        }
        case DIGIT_0_SIG:                        // intentionally fall through
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::frac1);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::operand1;
}
//............................................................................
QSTATE QCalc::frac1(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("frac1-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("frac1-EXIT");
            return (QSTATE)0;
        }
        case DIGIT_0_SIG:                        // intentionally fall through
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::operand1;
}
//............................................................................
QSTATE QCalc::opEntered(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("opEntered-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("opEntered-EXIT");
            return (QSTATE)0;
        }
        case OPER_SIG: {
            if (((QCalcEvt *)e)->keyId == KEY_MINUS) {
                me->clear();
                Q_TRAN(&QCalc::negated2);
                return (QSTATE)0;                             // event handled
            }
            break;
        }
        case DIGIT_0_SIG: {
            me->clear();
            Q_TRAN(&QCalc::zero2);
            return (QSTATE)0;
        }
        case DIGIT_1_9_SIG: {
            me->clear();
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::int2);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->clear();
            me->insert((int)'0');
            me->insert((int)'.');
            Q_TRAN(&QCalc::frac2);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::operand2(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("operand2-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("operand2-EXIT");
            return (QSTATE)0;
        }
        case CE_SIG: {
            me->clear();
            Q_TRAN(&QCalc::opEntered);
            return (QSTATE)0;
        }
        case OPER_SIG: {
            me->operand2_ = strtod(me->display_, (char **)0);
            if (me->eval()) {
                me->operand1_ = strtod(me->display_, (char **)0);
                me->opKey_ = ((QCalcEvt *)e)->keyId;
                Q_TRAN(&QCalc::opEntered);
            }
            else {
                Q_TRAN(&QCalc::error);
            }
            return (QSTATE)0;
        }
        case EQUALS_SIG: {
            me->operand2_ = strtod(me->display_, (char **)0);
            if (me->eval()) {
                Q_TRAN(&QCalc::result);
            }
            else {
                Q_TRAN(&QCalc::error);
            }
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::on;
}
//............................................................................
QSTATE QCalc::zero2(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("zero2-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("zero2-EXIT");
            return (QSTATE)0;
        }
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::int2);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::frac2);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::operand2;
}
//............................................................................
QSTATE QCalc::int2(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("int2-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("int2-EXIT");
            return (QSTATE)0;
        }
        case DIGIT_0_SIG:                        // intentionally fall through
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            return (QSTATE)0;
        }
        case POINT_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            Q_TRAN(&QCalc::frac2);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::operand2;
}
//............................................................................
QSTATE QCalc::frac2(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->updateState("frac2-ENTRY");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->updateState("frac2-EXIT");
            return (QSTATE)0;
        }
        case DIGIT_0_SIG:                        // intentionally fall through
        case DIGIT_1_9_SIG: {
            me->insert(((QCalcEvt *)e)->keyId);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QCalc::operand2;
}

