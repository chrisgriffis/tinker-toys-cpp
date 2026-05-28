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

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Q_DEFINE_THIS_FILE

// Local objects -------------------------------------------------------------
static QCalc l_qcalc;         // quantum calculator hierarchical state machine
struct termios l_saved;

//............................................................................
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    tcsetattr(0, TCSANOW, &l_saved);
    exit(-1);
}
//............................................................................
void QCalc::updateState(char const *s) {
    printf("%s;", s);
}
/*..........................................................................*/
void QCalc::exit(void) {
    printf("Bye, Bye!");
    tcsetattr(0, TCSANOW, &l_saved);
    ::exit(0);
}
//............................................................................
int main() {
    struct termios tio;                        // modified terminal attributes

    tcgetattr(0, &l_saved);            // save the current terminal attributes

    memcpy(&tio, &l_saved, sizeof(struct termios));         // init attributes
    tio.c_lflag &= ~ICANON;                          // disable canonical mode
    tcsetattr(0, TCSANOW, &tio);                     // set the new attributes

    printf("QCalc example, built on %s at %s,\n"
           "QEP/C++: %s.\nPress ESC to quit...\n",
           __DATE__, __TIME__, QEP::getVersion());

    l_qcalc.init((QEvent const *)0);             // trigger initial transition

    for (;;) {                                                   // event loop
        printf("\n[%s] >", l_qcalc.getDisplay());          // show the display
        fflush(stdout);

        QCalcEvt e;
        read(0, &e.keyId, 1);           // read one character from the console
        printf(": ");
        fflush(stdout);

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
