//////////////////////////////////////////////////////////////////////////////
// Product: QDPP example, DOS, Large model, Turbo C++ 1.01
// Version: Compatible with QF/C 3.1.yy
// Updated: Dec 08, 2005
//
// Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
//
// This example is part of the Quantum Leaps QP/C software, and may be
// distributed and modified under the terms of the GNU General Public License
// version 2 (GPL) as published by the Free Software Foundation and appearing
// in the file GPL.TXT included in the packaging of this file. Please note
// that GPL Section 2[b] requires that all works based on this software must
// also be made publicly available under the terms of the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified in conjunction
// with a valid QP/C Quantum Leaps commercial license. Quantum Leaps
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
#include "qf_port.h"
#include "qdpp.h"
#include "qassert.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

// Local-scope objects -------------------------------------------------------
static QEvent const *l_tableQueueSto[N];
static QEvent const *l_philQueueSto[N][N];
static TableEvt l_regPoolSto[N + 10];
static QSubscrList l_subscrSto[MAX_PUB_SIG];
static uint32_t l_delayCtr;

//............................................................................
int main(int argc, char *argv[]) {
    uint8_t n;
    printf("Quantum DPP\nQEP %s\nQF  %s, QF/DOS/L/TCPP101 port %s\n"
           "Press ESC to quit...\n",
           QEP::getVersion(),
           QF::getVersion(), QF::getPortVersion());

    if (argc > 1) {
        l_delayCtr = atol(argv[1]);
    }

    QF::init();       // initialize the framework and the underlying RT kernel

    QF::psInit(l_subscrSto, Q_DIM(l_subscrSto));     // init publish-subscribe

                                                  // initialize event pools...
    QF::poolInit((QEvent *)l_regPoolSto, sizeof(l_regPoolSto),
                 sizeof(l_regPoolSto[0]));
                                                // start the active objects...
    for (n = 0; n < N; ++n) {
        philosopherStart(n, (uint8_t)(n*10 + 1),
                         l_philQueueSto[n], Q_DIM(l_philQueueSto[n]),
                         (void *)0, 0);
    }
    tableStart((uint8_t)(N*10 + 1), l_tableQueueSto, Q_DIM(l_tableQueueSto),
               (void *)0, 0);

    QF::run();                                       // run the QF application

    return 0;
}
//............................................................................
void busyDelay(void) {
    volatile uint32_t i = l_delayCtr;
    while (i-- > 0) {
    }
}
