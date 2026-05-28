//////////////////////////////////////////////////////////////////////////////
// Product: QBomb/C++ example
// Version: Compatible with QEP/C++ 3.x.yy
// Updated: Nov 29, 2005
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
#ifndef qbomb_h
#define qbomb_h

enum QBombSignals {
    ARM_SIG = Q_USER_SIG,
    TICK_SIG,
    UP_SIG,
    DOWN_SIG
};

struct QBombInitEvt : public QEvent {
    uint8_t defuse;                                  // the secret defuse code
};

class QBomb : public QFsm {
    uint8_t timeout_;                      // number of seconds till explosion
    uint8_t defuse_;                                 // the secret defuse code
    uint8_t code_;                            // the current defuse code entry

public:
    QBomb() : QFsm((QState)&QBomb::initial) {
    }
    uint8_t getTimeout(void);

protected:
    static void initial(QBomb *me, QEvent const *e);
    static void setting(QBomb *me, QEvent const *e);
    static void timing(QBomb *me, QEvent const *e);
    static void blast(QBomb *me, QEvent const *e);

private:
                // platform-dependent callback function to print out the state
    void updateState(char const *s);

public:
               // platform-dependent callback function to exit the application
    void exit(void);
};

#endif                                                              // qbomb_h
