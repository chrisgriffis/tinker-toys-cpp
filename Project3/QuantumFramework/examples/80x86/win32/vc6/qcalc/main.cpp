//////////////////////////////////////////////////////////////////////////////
// Product: Quantum Calculator Example for Win32
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

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

Q_DEFINE_THIS_FILE

// Local objects -------------------------------------------------------------
static QCalc qcalc;           // quantum calculator hierarchical state machine

//............................................................................
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
//............................................................................
void QCalc::updateState(char const *s) {
    printf("%s;", s);
}
/*..........................................................................*/
void QCalc::exit(void) {
    printf("Bye, Bye!");
    ::exit(0);
}
//............................................................................
int main() {
    printf("QCalc example, built on %s at %s,\n"
           "QEP/C++: %s.\nPress ESC to quit...\n",
           __DATE__, __TIME__, QEP::getVersion());

    qcalc.init((QEvent const *)0);               // trigger initial transition

    for (;;) {                                                   // event loop
        printf("\n[%s] >", qcalc.getDisplay());            // show the display

        QCalcEvt e;
        e.keyId = (uint8_t)_getche(); // get a char from the console with echo
        printf(": ");

        switch (e.keyId) {
            case 'c':                            // intentionally fall through
            case 'C': {
                e.sig = C_SIG;
                break;
            }
            case 'e':                            // intentionally fall through
            case 'E': {
                e.sig = CE_SIG;
                break;
            }
            case '0': {
                e.sig = DIGIT_0_SIG;
                break;
            }
            case '1':                            // intentionally fall through
            case '2':                            // intentionally fall through
            case '3':                            // intentionally fall through
            case '4':                            // intentionally fall through
            case '5':                            // intentionally fall through
            case '6':                            // intentionally fall through
            case '7':                            // intentionally fall through
            case '8':                            // intentionally fall through
            case '9': {
                e.sig = DIGIT_1_9_SIG;
                break;
            }
            case '.': {
                e.sig = POINT_SIG;
                break;
            }
            case '+':                            // intentionally fall through
            case '-':                            // intentionally fall through
            case '*':                            // intentionally fall through
            case '/': {
                e.sig = OPER_SIG;
                break;
            }
            case '=':                            // intentionally fall through
            case '\r': {                                          // Enter key
                e.sig = EQUALS_SIG;
                break;
            }
            case '\33': {                                           // ESC key
                e.sig = TERMINATE_SIG;
                break;
            }
            default: {
                e.sig = IGNORE_SIG;
                break;
            }
        }

        qcalc.dispatch(&e);                              // dispatch the event
    }

    return 0;
}
