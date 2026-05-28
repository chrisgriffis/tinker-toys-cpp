//////////////////////////////////////////////////////////////////////////////
// Product: QHsmTst Example
// Version: Compatible with QEP/C++ 3.1.yy
// Updated: Jan 29, 2006
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
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
#include "qhsmtst.h"
#include "qassert.h"

//............................................................................
void QHsmTst::initial(QHsmTst *me, QEvent const *) {
    me->display("top-INIT;");
    me->foo_ = 0;                        // initialize extended state variable
    Q_INIT(&QHsmTst::d2);
}

//............................................................................
QSTATE QHsmTst::d(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("d-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("d-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("d-INIT;");
            Q_INIT(&QHsmTst::d11);
            return (QSTATE)0;
        }
        case C_SIG: {
            me->display("d-C;");
            Q_TRAN_DYN(&QHsmTst::s);
            return (QSTATE)0;
        }
        case E_SIG: {
            me->display("d-E;");
            Q_TRAN_DYN(&QHsmTst::d11);
            return (QSTATE)0;
        }
        case I_SIG: {                      // internal transition with a guard
            if (me->foo_) {
                me->display("d-I;");
                me->foo_ = 0;
                return (QSTATE)0;
            }
            break;
        }
        case TERMINATE_SIG: {
            me->exit();
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsm::top;
}
//............................................................................
QSTATE QHsmTst::d1(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("d1-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("d1-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("d1-INIT;");
            Q_INIT(&QHsmTst::d11);
            return (QSTATE)0;
        }
        case A_SIG: {
            me->display("d1-A;");
            Q_TRAN_DYN(&QHsmTst::d1);
            return (QSTATE)0;
        }
        case B_SIG: {
            me->display("d1-B;");
            Q_TRAN_DYN(&QHsmTst::d11);
            return (QSTATE)0;
        }
        case C_SIG: {
            me->display("d1-C;");
            Q_TRAN_DYN(&QHsmTst::d2);
            return (QSTATE)0;
        }
        case D_SIG: {                               // transition with a gurad
            if (!me->foo_) {
                me->display("d1-D;");
                me->foo_ = 1;
                Q_TRAN_DYN(&QHsmTst::d);
                return (QSTATE)0;                             // event handled
            }
            break;
        }
        case F_SIG: {
            me->display("d1-F;");
            Q_TRAN_DYN(&QHsmTst::d211);
            return (QSTATE)0;
        }
        case I_SIG: {                                   // internal transition
            me->display("d1-I;");
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::d;
}
//............................................................................
QSTATE QHsmTst::d11(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("d11-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("d11-EXIT;");
            return (QSTATE)0;
        }
        case D_SIG: {                               // transition with a gurad
            if (me->foo_) {
                me->display("d11-D;");
                me->foo_ = 0;
                Q_TRAN_DYN(&QHsmTst::d1);
                return (QSTATE)0;                             // event handled
            }
            break;
        }
        case G_SIG: {
            me->display("d11-G;");
            Q_TRAN_DYN(&QHsmTst::d211);
            return (QSTATE)0;
        }
        case H_SIG: {
            me->display("d11-H;");
            Q_TRAN_DYN(&QHsmTst::d);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::d1;
}
//............................................................................
QSTATE QHsmTst::d2(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("d2-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("d2-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("d2-INIT;");
            Q_INIT(&QHsmTst::d211);
            return (QSTATE)0;
        }
        case F_SIG: {
            me->display("d2-F;");
            Q_TRAN_DYN(&QHsmTst::d11);
            return (QSTATE)0;
        }
        case I_SIG: {                      // internal transition with a guard
            if (!me->foo_) {
                me->display("d2-I;");
                me->foo_ = (uint8_t)1;
                return (QSTATE)0;
            }
            break;
        }
    }
    return (QSTATE)&QHsmTst::d;
}
//............................................................................
QSTATE QHsmTst::d21(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("d21-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("d21-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("d21-INIT;");
            Q_INIT(&QHsmTst::d211);
            return (QSTATE)0;
        }
        case A_SIG: {
            me->display("d21-A;");
            Q_TRAN_DYN(&QHsmTst::d21);
            return (QSTATE)0;
        }
        case B_SIG: {
            me->display("d21-B;");
            Q_TRAN_DYN(&QHsmTst::d211);
            return (QSTATE)0;
        }
        case G_SIG: {
            me->display("d21-G;");
            Q_TRAN_DYN(&QHsmTst::d1);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::d2;
}
//............................................................................
QSTATE QHsmTst::d211(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("d211-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("d211-EXIT;");
            return (QSTATE)0;
        }
        case D_SIG: {
            me->display("d211-D;");
            Q_TRAN_DYN(&QHsmTst::d21);
            return (QSTATE)0;
        }
        case H_SIG: {
            me->display("d211-H;");
            Q_TRAN_DYN(&QHsmTst::d);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::d21;
}

//............................................................................
QSTATE QHsmTst::s(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("s-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("s-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("s-INIT;");
            Q_INIT(&QHsmTst::s11);
            return (QSTATE)0;
        }
        case C_SIG: {
            me->display("s-C;");
            Q_TRAN_STA(&QHsmTst::d);
            return (QSTATE)0;
        }
        case E_SIG: {
            me->display("s-E;");
            Q_TRAN_STA(&QHsmTst::s11);
            return (QSTATE)0;
        }
        case I_SIG: {
            if (me->foo_) {
                me->display("s-I;");
                me->foo_ = 0;
                return (QSTATE)0;                             // event handled
            }
            break;
        }
        case TERMINATE_SIG: {
            me->exit();
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsm::top;
}
//............................................................................
QSTATE QHsmTst::s1(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("s1-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("s1-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("s1-INIT;");
            Q_INIT(&QHsmTst::s11);
            return (QSTATE)0;
        }
        case A_SIG: {
            me->display("s1-A;");
            Q_TRAN_STA(&QHsmTst::s1);
            return (QSTATE)0;
        }
        case B_SIG: {
            me->display("s1-B;");
            Q_TRAN_STA(&QHsmTst::s11);
            return (QSTATE)0;
        }
        case C_SIG: {
            me->display("s1-C;");
            Q_TRAN_STA(&QHsmTst::s2);
            return (QSTATE)0;
        }
        case D_SIG: {                               // transition with a gurad
            if (!me->foo_) {
                me->display("s1-D;");
                me->foo_ = 1;
                Q_TRAN_STA(&QHsmTst::s);
                return (QSTATE)0;                             // event handled
            }
            break;
        }
        case F_SIG: {
            me->display("s1-F;");
            Q_TRAN_STA(&QHsmTst::s211);
            return (QSTATE)0;
        }
        case I_SIG: {                                   // internal transition
            me->display("d1-I;");
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::s;
}
//............................................................................
QSTATE QHsmTst::s11(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("s11-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("s11-EXIT;");
            return (QSTATE)0;
        }
        case D_SIG: {                               // transition with a gurad
            if (me->foo_) {
                me->display("s11-D;");
                me->foo_ = 0;
                Q_TRAN_STA(&QHsmTst::s1);
                return (QSTATE)0;                             // event handled
            }
            break;
        }
        case G_SIG: {
            me->display("s11-G;");
            Q_TRAN_STA(&QHsmTst::s211);
            return (QSTATE)0;
        }
        case H_SIG: {
            me->display("s11-H;");
            Q_TRAN_STA(&QHsmTst::s);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::s1;
}
//............................................................................
QSTATE QHsmTst::s2(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("s2-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("s2-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("s2-INIT;");
            Q_INIT(&QHsmTst::s211);
            return (QSTATE)0;
        }
        case F_SIG: {
            me->display("s2-F;");
            Q_TRAN_STA(&QHsmTst::s11);
            return (QSTATE)0;
        }
        case I_SIG: {                      // internal transition with a guard
            if (!me->foo_) {
                me->display("s2-I;");
                me->foo_ = (uint8_t)1;
                return (QSTATE)0;
            }
            break;
        }
    }
    return (QSTATE)&QHsmTst::s;
}
//............................................................................
QSTATE QHsmTst::s21(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("s21-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("s21-EXIT;");
            return (QSTATE)0;
        }
        case Q_INIT_SIG: {
            me->display("s21-INIT;");
            Q_INIT(&QHsmTst::s211);
            return (QSTATE)0;
        }
        case A_SIG: {
            me->display("s21-A;");
            Q_TRAN_STA(&QHsmTst::s21);
            return (QSTATE)0;
        }
        case B_SIG: {
            me->display("s21-B;");
            Q_TRAN_STA(&QHsmTst::s211);
            return (QSTATE)0;
        }
        case G_SIG: {
            me->display("s21-G;");
            Q_TRAN_STA(&QHsmTst::s1);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::s2;
}
//............................................................................
QSTATE QHsmTst::s211(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->display("s211-ENTRY;");
            return (QSTATE)0;
        }
        case Q_EXIT_SIG: {
            me->display("s211-EXIT;");
            return (QSTATE)0;
        }
        case D_SIG: {
            me->display("s211-D;");
            Q_TRAN_STA(&QHsmTst::s21);
            return (QSTATE)0;
        }
        case H_SIG: {
            me->display("s211-H;");
            Q_TRAN_STA(&QHsmTst::s2);
            return (QSTATE)0;
        }
    }
    return (QSTATE)&QHsmTst::s21;
}
