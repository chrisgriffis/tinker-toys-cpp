//////////////////////////////////////////////////////////////////////////////
// Product: Product:  QCalc example, DOS, Large model, Turbo C++ 1.01
// Last Updated for Version: 3.1.01
// Date of the Last Update:  Jan 29, 2006
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
#include "qassert.h"
#include "qcalc.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

// Local objects -------------------------------------------------------------
static QCalc l_qcalc;         // quantum calculator hierarchical state machine

//............................................................................
int main() {
    printf("QCalc example, built on %s at %s,\n"
           "QEP/C++: %s.\nPress ESC to quit...\n",
           __DATE__, __TIME__, QEP::getVersion());

    l_qcalc.init();                              // trigger initial transition

    for (;;) {                                                   // event loop
        printf("\n[%s] >", l_qcalc.getDisplay());          // show the display

        QCalcEvt e;
        e.keyId = (uint8_t)getche();  // get a char from the console with echo
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

        l_qcalc.dispatch(&e);                            // dispatch the event
    }

    return 0;
}
//............................................................................
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
//............................................................................
void QCalc::updateState(char const *s) {
    printf("%s;", s);
}
//............................................................................
void QCalc::exit(void) {
    printf("Bye, Bye!");
    ::exit(0);
}

