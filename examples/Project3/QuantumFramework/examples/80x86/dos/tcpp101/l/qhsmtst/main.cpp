//////////////////////////////////////////////////////////////////////////////
// Product: QHsmTst Example, DOS, Large model, Turbo C++ 1.01
// Version: Compatible with QEP/C++ 3.1.yy
// Updated: Jan 29, 2006
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
#include "qhsmtst.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

// Local objects -------------------------------------------------------------
static QHsmTst l_qhsmTst;
static FILE *l_outFile = (FILE *)0;
static void dispatch(QSignal sig);

//............................................................................
int main(int argc, char *argv[ ]) {
    if (argc > 0) {                                     // file name provided?
        l_outFile = fopen(argv[1], "w");
    }

    if (l_outFile == (FILE *)0) {                      // interactive version?
        l_outFile = stdout;

        printf("QHsmTst example, built on %s at %s, QEP/C++ %s\n",
               __DATE__, __TIME__, QEP::getVersion());

        l_qhsmTst.init((QEvent const *)0);       // trigger initial transition

        for (;;) {                                               // event loop
            printf("\n>");

            int c;
            c = getche();        // get a character from the console with echo
            printf(": ");

            QEvent e;
            if ('a' <= c && c <= 'i') {                           // in range?
                e.sig = (QSignal)(c - 'a' + A_SIG);
            }
            else if ('A' <= c && c <= 'I') {                      // in range?
                e.sig = (QSignal)(c - 'A' + A_SIG);
            }
            else if (c == '\33') {                             // the ESC key?
                e.sig = TERMINATE_SIG;       // terminate the interactive test
            }
            else {
                e.sig = IGNORE_SIG;
            }

            l_qhsmTst.dispatch(&e);                      // dispatch the event
        }
    }
    else {                                                    // batch version
        printf("QHsmTst, output saved to %s\n", argv[1]);
        fprintf(l_outFile,
                "QHsmTst example, QEP/C++ %s\n", QEP::getVersion());

        l_qhsmTst.init((QEvent const *)0);       // trigger initial transition

                                                        // dynamic transitions
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(I_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(G_SIG);
        dispatch(H_SIG);
        dispatch(H_SIG);
        dispatch(C_SIG);
        dispatch(G_SIG);
        dispatch(C_SIG);
        dispatch(C_SIG);

                                                         // static transitions
        dispatch(C_SIG);
        dispatch(A_SIG);
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(I_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(A_SIG);
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(G_SIG);
        dispatch(H_SIG);
        dispatch(G_SIG);
        dispatch(H_SIG);
        dispatch(F_SIG);
        dispatch(H_SIG);
        dispatch(F_SIG);
        dispatch(C_SIG);
        dispatch(G_SIG);
        dispatch(G_SIG);

        fclose(l_outFile);
    }

    return 0;
}
//............................................................................
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
//............................................................................
void QHsmTst::display(char const *msg) {
    fprintf(l_outFile, msg);
}
//............................................................................
void QHsmTst::exit(void) {
    printf("Bye, Bye!");
    ::exit(0);
}
//............................................................................
static void dispatch(QSignal sig) {
    QEvent e;
    Q_REQUIRE((A_SIG <= sig) && (sig <= I_SIG));
    e.sig = sig;
    fprintf(l_outFile, "\n%c:", 'A' + sig - A_SIG);
    l_qhsmTst.dispatch(&e);                              // dispatch the event
}

