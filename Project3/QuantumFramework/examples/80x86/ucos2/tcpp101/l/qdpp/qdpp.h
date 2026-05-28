//////////////////////////////////////////////////////////////////////////////
// Product: QDPP example, uC/OS-II, Large model, Turbo C++ 1.01
// Version: Compatible with QF/C++ 3.x.yy
// Updated: Nov 14, 2005
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
#ifndef qdpp_h
#define qdpp_h

enum DPPSignals {
   HUNGRY_SIG = Q_USER_SIG,            // sent when philosopher becomes hungry
   DONE_SIG,                           // sent by philosopher when done eating
   EAT_SIG,                          // sent by Table to let a philosopher eat
   START_SIG,
   TERMINATE_SIG,
   KBD_SIG,
   MAX_PUB_SIG                                    // the last published signal
};

struct TableEvt : public QEvent {
    uint8_t philNum;                                     // philosopher number
};

struct KbdEvt : public QEvent {
    uint8_t key;
};

enum { N = 5 };                                      // number of philosophers

void philosopherStart(uint8_t n, uint8_t prio,
                      QEvent const *qSto[], uint32_t qLen,
                      void *stkSto, uint32_t stkSize);

void tableStart(uint8_t prio,
                QEvent const *qSto[], uint32_t qLen,
                void *stkSto, uint32_t stkSize);

void kbdMgrStart(uint8_t prio, QEvent const *qSto[], uint32_t qLen,
                 void *stkSto, uint32_t stkSize);

                           // implementation-dependent status-display routines
void displyPhilStat(uint8_t n, char const *stat);
void displayKey(uint8_t key);

extern QActive *QDPP_table;

#endif                                                               // qdpp_h
