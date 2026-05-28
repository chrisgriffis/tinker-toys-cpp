//////////////////////////////////////////////////////////////////////////////
// Product: QBomb Example, DOS, Large model, Turbo C++ 1.01
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
#include "qep_port.h"
#include "qassert.h"
#include "qbomb.h"

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

Q_DEFINE_THIS_FILE

// Local objects -------------------------------------------------------------
static QBomb l_qbomb;                                      // quantum bomb FSM

//............................................................................
int main() {
    printf("QBomb example, built on %s at %s,\n"
           "QEP/C++: %s.\nPress ESC to quit...\n",
           __DATE__, __TIME__, QEP::getVersion());

    QBombInitEvt ie;
    ie.defuse = 0x0D;                                           // 1101 binary
    l_qbomb.init(&ie);                           // trigger initial transition

    for (;;) {                                                   // event loop
        static int tickCtr = 1;
        QEvent e;
        char const *msg = (char *)0;

        delay(100);                              // delay for 100 milliseconds

        if (kbhit()) {
            switch (getch()) {
                case 'a':
                case 'A': {                                         // Arm key
                    e.sig = ARM_SIG;
                    msg = "ARM";
                    break;
                }
                case 'u':
                case 'U': {                                          // Up key
                    e.sig = UP_SIG;
                    msg = "UP";
                    break;
                }
                case 'd':
                case 'D': {                                        // Down key
                    e.sig = DOWN_SIG;
                    msg = "DOWN";
                    break;
                }
                case '\33': {                                       // ESC key
                    l_qbomb.exit();
                    break;
                }
            }
        }
        else if (--tickCtr == 0) {                       // time for the tick?
            tickCtr = 10;
            e.sig = TICK_SIG;
            msg = "TICK";
        }

        if (msg != (char *)0) {
            printf("\n[%2d] %s: ", (int)l_qbomb.getTimeout(), msg);
            l_qbomb.dispatch(&e);                        // dispatch the event
        }
    }

    return 0;
}
//............................................................................
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
//............................................................................
void QBomb::updateState(char const *s) {
    printf("%s;", s);
}
//............................................................................
void QBomb::exit(void) {
    printf("\nBye, Bye!");
    ::exit(0);
}
