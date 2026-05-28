//////////////////////////////////////////////////////////////////////////////
// Product: QHsmTst Example, DOS, Turbo C++ 1.01, Large model
// Version: Compatible with QEP/C++ 3.x.yy
// Updated: Aug 09, 2005
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
#ifndef qhsmtst_h
#define qhsmtst_h

enum QHsmTstSignals {
    A_SIG = Q_USER_SIG,
    B_SIG,
    C_SIG,
    D_SIG,
    E_SIG,
    F_SIG,
    G_SIG,
    H_SIG,
    I_SIG,
    TERMINATE_SIG,
    IGNORE_SIG,
    MAX_SIG
};

class QHsmTst : public QHsm {
    int8_t foo_;                                    // extended state variable

public:
    QHsmTst() : QHsm((QState)&QHsmTst::initial) {                      // ctor
    }

protected:
    static void initial(QHsmTst *me, QEvent const *e);  // initial pseudostate

    static QSTATE d(QHsmTst *me, QEvent const *e);            // state-handler
    static QSTATE d1(QHsmTst *me, QEvent const *e);           // state-handler
    static QSTATE d11(QHsmTst *me, QEvent const *e);          // state-handler
    static QSTATE d2(QHsmTst *me, QEvent const *e);           // state-handler
    static QSTATE d21(QHsmTst *me, QEvent const *e);          // state-handler
    static QSTATE d211(QHsmTst *me, QEvent const *e);         // state-handler

    static QSTATE s(QHsmTst *me, QEvent const *e);            // state-handler
    static QSTATE s1(QHsmTst *me, QEvent const *e);           // state-handler
    static QSTATE s11(QHsmTst *me, QEvent const *e);          // state-handler
    static QSTATE s2(QHsmTst *me, QEvent const *e);           // state-handler
    static QSTATE s21(QHsmTst *me, QEvent const *e);          // state-handler
    static QSTATE s211(QHsmTst *me, QEvent const *e);         // state-handler

             // platform-dependent callback function to dispaly a text message
    void display(char const *msg);
    void exit(void);
};

#endif                                                            // qhsmtst_h
