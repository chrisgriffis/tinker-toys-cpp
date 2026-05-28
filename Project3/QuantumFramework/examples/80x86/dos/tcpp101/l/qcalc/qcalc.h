//////////////////////////////////////////////////////////////////////////////
// Product: Product: QCalc/C++ example, DOS, Turbo C++ 1.01, Large model
// Last Updated for Version: 3.1.00
// Date of the Last Update:  Sep 19, 2005
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
#ifndef qcalc_h
#define qcalc_h

enum QCalcSignals {
    C_SIG = Q_USER_SIG,
    CE_SIG,
    DIGIT_0_SIG,
    DIGIT_1_9_SIG,
    POINT_SIG,
    OPER_SIG,
    EQUALS_SIG,
    TERMINATE_SIG,
    IGNORE_SIG,
    MAX_SIG
};

struct QCalcEvt : public QEvent {
    uint8_t keyId;                                  // ID of the key depressed
};

#define DISP_WIDTH  14

class QCalc : public QHsm {                // Quantum Calculator state machine
private:
    double operand1_;
    double operand2_;
    char display_[DISP_WIDTH + 1];
    uint8_t len_;
    uint8_t opKey_;

public:
    QCalc() : QHsm((QState)&QCalc::initial) {                          // ctor
    }
    char const *getDisplay() const {
        return display_;
    }

protected:
    static void initial(QCalc *me, QEvent const *e);
    static QSTATE on(QCalc *me, QEvent const *e);
    static QSTATE error(QCalc *me, QEvent const *e);
    static QSTATE ready(QCalc *me, QEvent const *e);
    static QSTATE result(QCalc *me, QEvent const *e);
    static QSTATE begin(QCalc *me, QEvent const *e);
    static QSTATE negated1(QCalc *me, QEvent const *e);
    static QSTATE operand1(QCalc *me, QEvent const *e);
    static QSTATE zero1(QCalc *me, QEvent const *e);
    static QSTATE int1(QCalc *me, QEvent const *e);
    static QSTATE frac1(QCalc *me, QEvent const *e);
    static QSTATE opEntered(QCalc *me, QEvent const *e);
    static QSTATE negated2(QCalc *me, QEvent const *e);
    static QSTATE operand2(QCalc *me, QEvent const *e);
    static QSTATE zero2(QCalc *me, QEvent const *e);
    static QSTATE int2(QCalc *me, QEvent const *e);
    static QSTATE frac2(QCalc *me, QEvent const *e);

protected:
    void clear(void);
    void insert(int keyId);
    void negate(void);
    uint8_t eval(void);

protected:        // platform-dependent methods to update the display and exit
    void updateState(char const *s);
    void exit(void);
};

#endif                                                              // qcalc_h
