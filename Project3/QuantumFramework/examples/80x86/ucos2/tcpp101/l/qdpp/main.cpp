//////////////////////////////////////////////////////////////////////////////
// Product: QDPP example, uC/OS-II, Turbo C++ 1.01, Large model
// Version: Compatible with QF/C 3.1.yy
// Updated: Oct 20, 2005
//
// Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
//
// This software may be distributed and modified under the terms of the GNU
// General Public License version 2 (GPL) as published by the Free Software
// Foundation and appearing in the file GPL.TXT included in the packaging of
// this file. Please note that GPL Section 2[b] requires that all works based
// on this software must also be made publicly available under the terms of
// the GPL ("Copyleft").
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
#include "qf_port.h"
#include "qdpp.h"
#include "video.h"
#include "qassert.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

Q_DEFINE_THIS_FILE

// local objects -------------------------------------------------------------
static QEvent const *l_kbdMgrQueueSto[2];
static QEvent const *l_tableQueueSto[N];
static QEvent const *l_philoQueueSto[N][10];
static void *l_regPoolSto[100/sizeof(void*)];  // 100 bytes for the event pool
static QSubscrList l_subscrSto[MAX_PUB_SIG];

static int l_philoStk[N][256];                  // stacks for the philosophers
static int l_tableStk[256];               // stack for the Table active object
static int l_kbdMgrStk[256];             // stack for the KbdMgr active object

static uint32_t l_delayCtr = 0UL;

static void setupScreen(void);

//............................................................................
int main(int argc, char *argv[]) {
    if (argc > 1) {                                        // delay specified?
        l_delayCtr = atol(argv[1]);
    }

    setupScreen();                                         // setup the screen

    QF::init();       // initialize the framework and the underlying RT kernel

    QF::psInit(l_subscrSto, Q_DIM(l_subscrSto));     // init publish-subscribe
                                                  // initialize event pools...
    QF::poolInit((QEvent *)l_regPoolSto, sizeof(l_regPoolSto),
                 sizeof(l_regPoolSto[0]));
                                                    // start active objects...
    uint8_t n;
    for (n = 0; n < N; ++n) {
        philosopherStart(n, (uint8_t)(n*10 + 1),
                         l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                         l_philoStk[n], sizeof(l_philoStk[n]));
    }

    tableStart((uint8_t)(N*10 + 1),
               l_tableQueueSto, Q_DIM(l_tableQueueSto),
               l_tableStk, sizeof(l_tableStk));

    kbdMgrStart((uint8_t)(N*10 + 2),
               l_kbdMgrQueueSto, Q_DIM(l_kbdMgrQueueSto),
               l_kbdMgrStk, sizeof(l_kbdMgrStk));

    QF::run();                                       // run the QF application

    return 0;
}
//............................................................................
static void setupScreen(void) {
    uint8_t n;

    Video::clearScreen(Video::BGND_BLACK);
    Video::clearRect( 0,  0, 80,  7, Video::BGND_LIGHT_GRAY);
    Video::clearRect( 0, 11, 80, 12, Video::BGND_LIGHT_GRAY);
    Video::clearRect( 0, 12, 41, 23, Video::BGND_BLUE);
    Video::clearRect(41, 12, 80, 23, Video::BGND_RED);
    Video::clearRect( 0, 23, 80, 24, Video::BGND_LIGHT_GRAY);

    n = Video::FGND_BLUE;
    Video::printStrAt(10, 0, n, "  __");
    Video::printStrAt(10, 1, n, " /  |      _   _ -|-     _ _");
    Video::printStrAt(10, 2, n, " \\__| | |  _\\ | \\ | | | | \\ \\");
    Video::printStrAt(10, 3, n, "    | \\_/ |_| | | | \\_| | | |");
    Video::printStrAt(10, 4, n, "    |");
    n = Video::FGND_RED;
    Video::printStrAt(43, 0, n, "    _       __ ");
    Video::printStrAt(43, 1, n, "|  /_\\     |  \\  TM");
    Video::printStrAt(43, 2, n, "|  \\_   _  |__/ _");
    Video::printStrAt(43, 3, n, "|       _\\ |   |_");
    Video::printStrAt(43, 4, n, "|___   |_| |    _|");
    Video::printStrAt(10, 5, Video::FGND_BLUE,
                     "_____________________________________________________");
    Video::printStrAt(10, 6, Video::FGND_RED,
                     "i n n o v a t i n g   e m b e d d e d   s y s t e m s");
    Video::printStrAt(18,  7, Video::FGND_WHITE,
                     "Quantum Dining Philosophers Example");
    Video::printStrAt(18,  8, Video::FGND_WHITE, "QEP/C++");
    Video::printStrAt(28,  8, Video::FGND_YELLOW, QEP::getVersion());
    Video::printStrAt(18,  9, Video::FGND_WHITE, "QF/C++");
    Video::printStrAt(28,  9, Video::FGND_YELLOW, QF::getVersion());
    Video::printStrAt(36,  9, Video::FGND_WHITE, "Port to uC/OS-II");
    Video::printStrAt(53,  9, Video::FGND_YELLOW, QF::getPortVersion());
    Video::printStrAt(18, 10, Video::FGND_WHITE, "uC/OS-II");

    // uC/OS-II version is returned as an integer value multiplied by 100
    Video::printNumAt(29, 10, Video::FGND_YELLOW, OSVersion()%100);
    Video::printStrAt(28, 10, Video::FGND_YELLOW, "2.");

    Video::printStrAt( 1, 11, Video::FGND_BLUE,
                     "Active Object   State        Data");

    for (n = 0; n < N; ++n) {
        Video::printStrAt( 1, 12 + n, Video::FGND_WHITE, "Philosopher");
        Video::printNumAt(12, 12 + n, Video::FGND_WHITE, n + 1);
    }
    Video::printStrAt( 1, 12 + N, Video::FGND_WHITE,  "Table");
    Video::printStrAt(17, 12 + N, Video::FGND_YELLOW, "serving");
    Video::printStrAt( 1, 12 + N + 1, Video::FGND_WHITE,  "KbdMgr");
    Video::printStrAt(17, 12 + N + 1, Video::FGND_YELLOW, "active");

    Video::printStrAt(4, 23, Video::FGND_BLUE,
         "* Copyright (c) 2002-2005 Quantum Leaps, LLC "
         "* www.quantum-leaps.com *");
    Video::printStrAt(28, 24, Video::FGND_LIGHT_RED,
         "<< Press Esc to quit >>");
}
//............................................................................
void busyDelay(void) {
    volatile uint32_t i = l_delayCtr;
    while (i-- > 0) {
    }
}

