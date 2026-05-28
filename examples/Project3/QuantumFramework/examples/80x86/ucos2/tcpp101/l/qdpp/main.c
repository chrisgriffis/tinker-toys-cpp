/*****************************************************************************
* Product: QDPP example
* Version: Compatible with QF/C 3.1.yy
* Updated: Nov 18, 2005
*
* Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
*
* This example is part of the Quantum Leaps QP/C software, and may be
* distributed and modified under the terms of the GNU General Public License
* version 2 (GPL) as published by the Free Software Foundation and appearing
* in the file GPL.TXT included in the packaging of this file. Please note
* that GPL Section 2[b] requires that all works based on this software must
* also be made publicly available under the terms of the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified in conjunction
* with a valid QP/C Quantum Leaps commercial license. Quantum Leaps
* commercial licenses are designed for users who want to retain proprietary
* status of their code. The users who license this software under one of
* Quantum Leaps commercial licenses do not use this software under the GPL
* and therefore are not subject to any of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
* Quantum Leaps products:  http://www.quantum-leaps.com/products
* e-mail:                  sales@quantum-leaps.com
*****************************************************************************/
#include "qf_port.h"
#include "qdpp.h"
#include "video.h"
#include "qassert.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

/* local objects -----------------------------------------------------------*/
static QEvent const *l_philoQueueSto[N][10];
static QEvent const *l_tableQueueSto[N];
static QEvent const *l_kbdMgrQueueSto[2];
static void *l_regPoolSto[100/sizeof(void*)];/*100 bytes for the event pool */
static QSubscrList l_subscrSto[MAX_PUB_SIG];

static int l_philoStk[N][256]; /* stacks for the Philosopher active objects */
static int l_tableStk[256];            /* stack for the Table active object */
static int l_kbdMgrStk[256];          /* stack for the KbdMgr active object */

static void setupScreen(void);
static uint32_t l_delayCtr;

/*..........................................................................*/
int main(int argc, char *argv[]) {
    uint8_t n;

    if (argc > 1) {
        l_delayCtr = atol(argv[1]);
    }

    setupScreen();                                      /* setup the screen */

    QF_init();     /* initialize the framework and the underlying RT kernel */

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

                                               /* initialize event pools... */
    QF_poolInit(l_regPoolSto, sizeof(l_regPoolSto), sizeof(l_regPoolSto[0]));

                                                 /* start active objects... */
    for (n = 0; n < N; ++n) {
        philosopherStart(n, (uint8_t)(n*10 + 1),
                         l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                         l_philoStk[n], sizeof(l_philoStk[n]));
    }

    tableStart((uint8_t)(N*10 + 1),
               l_tableQueueSto, Q_DIM(l_tableQueueSto),
               l_tableStk, sizeof(l_tableStk));           /* uC/OS-II stack */

    kbdMgrStart((uint8_t)(N*10 + 2),
                l_kbdMgrQueueSto, Q_DIM(l_kbdMgrQueueSto),
                l_kbdMgrStk, sizeof(l_kbdMgrStk));        /* uC/OS-II stack */
    QF_run();                                     /* run the QF application */

    return 0;
}
/*..........................................................................*/
static void setupScreen(void) {
    uint8_t n;

    Video_clearScreen(VIDEO_BGND_BLACK);
    Video_clearRect( 0,  0, 80,  7, VIDEO_BGND_LIGHT_GRAY);
    Video_clearRect( 0, 11, 80, 12, VIDEO_BGND_LIGHT_GRAY);
    Video_clearRect( 0, 12, 41, 23, VIDEO_BGND_BLUE);
    Video_clearRect(41, 12, 80, 23, VIDEO_BGND_RED);
    Video_clearRect( 0, 23, 80, 24, VIDEO_BGND_LIGHT_GRAY);

    n = VIDEO_FGND_BLUE;
    Video_printStrAt(10, 0, n, "  __");
    Video_printStrAt(10, 1, n, " /  |      _   _ -|-     _ _");
    Video_printStrAt(10, 2, n, " \\__| | |  _\\ | \\ | | | | \\ \\");
    Video_printStrAt(10, 3, n, "    | \\_/ |_| | | | \\_| | | |");
    Video_printStrAt(10, 4, n, "    |");
    n = VIDEO_FGND_RED;
    Video_printStrAt(43, 0, n, "    _       __ ");
    Video_printStrAt(43, 1, n, "|  /_\\     |  \\  TM");
    Video_printStrAt(43, 2, n, "|  \\_   _  |__/ _");
    Video_printStrAt(43, 3, n, "|       _\\ |   |_");
    Video_printStrAt(43, 4, n, "|___   |_| |    _|");
    Video_printStrAt(10, 5, VIDEO_FGND_BLUE,
                     "_____________________________________________________");
    Video_printStrAt(10, 6, VIDEO_FGND_RED,
                     "i n n o v a t i n g   e m b e d d e d   s y s t e m s");
    Video_printStrAt(18,  7, VIDEO_FGND_WHITE,
                     "Quantum Dining Philosophers Example");
    Video_printStrAt(18,  8, VIDEO_FGND_WHITE, "QEP/C");
    Video_printStrAt(28,  8, VIDEO_FGND_YELLOW, QEP_getVersion());
    Video_printStrAt(18,  9, VIDEO_FGND_WHITE, "QF/C");
    Video_printStrAt(28,  9, VIDEO_FGND_YELLOW, QF_getVersion());
    Video_printStrAt(36,  9, VIDEO_FGND_WHITE, "Port to uC/OS-II");
    Video_printStrAt(53,  9, VIDEO_FGND_YELLOW, QF_getPortVersion());
    Video_printStrAt(18, 10, VIDEO_FGND_WHITE, "uC/OS-II");

    /* uC/OS-II version is returned as an integer value multiplied by 100 */
    Video_printNumAt(29, 10, VIDEO_FGND_YELLOW, OSVersion()%100);
    Video_printStrAt(28, 10, VIDEO_FGND_YELLOW, "2.");

    Video_printStrAt( 1, 11, VIDEO_FGND_BLUE,
                     "Active Object   State        Data");

    for (n = 0; n < N; ++n) {
        Video_printStrAt( 1, 12 + n, VIDEO_FGND_WHITE, "Philosopher");
        Video_printNumAt(12, 12 + n, VIDEO_FGND_WHITE, n + 1);
    }
    Video_printStrAt( 1, 12 + N, VIDEO_FGND_WHITE,  "Table");
    Video_printStrAt(17, 12 + N, VIDEO_FGND_YELLOW, "serving");
    Video_printStrAt( 1, 12 + N + 1, VIDEO_FGND_WHITE,  "KbdMgr");
    Video_printStrAt(17, 12 + N + 1, VIDEO_FGND_YELLOW, "active");

    Video_printStrAt(4, 23, VIDEO_FGND_BLUE,
         "* Copyright (c) 2002-2005 Quantum Leaps, LLC "
         "* www.quantum-leaps.com *");
    Video_printStrAt(28, 24, VIDEO_FGND_LIGHT_RED,
         "<< Press Esc to quit >>");
}
/*..........................................................................*/
void busyDelay(void) {
    volatile uint32_t i = l_delayCtr;
    while (i-- > 0) {
    }
}
