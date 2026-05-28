/*****************************************************************************
* Product: QDPP example, DOS, Large model, Turbo C++ 1.01
* Version: Compatible with QF/C 3.1.yy
* Updated: Dec 03, 2005
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

Q_DEFINE_THIS_FILE

/*..........................................................................*/
void OSTimeTickHook(void) {
    QF_tick();
}
/*..........................................................................*/
void OSTaskIdleHook(void) {
    if (kbhit()) {
        uint8_t key = (uint8_t)getch();
        if (key == 0x1B) {                          /* is this the ESC key? */
            QF_publish(Q_NEW(QEvent, TERMINATE_SIG));
        }
        else {                                         /* other key pressed */
            KbdEvt *ke = Q_NEW(KbdEvt, KBD_SIG);
            ke->key = key;
            QF_publish((QEvent *)ke);
        }
    }
}
/*..........................................................................*/
void Q_assert_handler(char const *file, int line) {
    Video_clearRect(0, 24, 80, 25, VIDEO_BGND_RED);
    Video_printStrAt(0, 24, VIDEO_FGND_WHITE, "ASSERTION FAILED in file:");
    Video_printStrAt(26, 24, VIDEO_FGND_YELLOW, file);
    Video_printStrAt(57, 24, VIDEO_FGND_WHITE, "line:");
    Video_printNumAt(62, 24, VIDEO_FGND_YELLOW, line);

    QF_exit();                                       /* exit and cleanup QF */
}
/*..........................................................................*/
void displyPhilStat(uint8_t n, char const *stat) {
    Video_printStrAt(17, 12 + n, VIDEO_FGND_YELLOW, stat);
}
/*..........................................................................*/
void displayKey(uint8_t key) {
    Video_printNumAt(30, 12 + N + 1, VIDEO_FGND_YELLOW, key);
}
