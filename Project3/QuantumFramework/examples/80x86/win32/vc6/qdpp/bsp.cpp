//////////////////////////////////////////////////////////////////////////////
// Product: QDPP example, 80x86, Win32, Visual C++ 6.0
// Version: Compatible with QF/C 3.1.yy
// Updated: Feb 13, 2006
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
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
#include "qassert.h"
#include "qdpp.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>                        // for _beginthread()/_endthread()

Q_DEFINE_THIS_FILE

//............................................................................
static void __cdecl idleThread(void *me) {       // signature for _beginthread
    (void)me;
    while (QF_running_) {
        Sleep(10);                                         // wait for a while
        if (kbhit()) {                                     // any key pressed?
            if (getch() == 0x1B) {               // see if the ESC key pressed
                QF::publish(Q_NEW(QEvent, TERMINATE_SIG));
            }
        }
    }
    _endthread();       // cleanup after the thead and close the thread handle
}
//............................................................................
void BSP_init(int argc, char *argv[]) {
    HANDLE hIdle = (HANDLE)_beginthread(&idleThread, 1024, 0);
    Q_ASSERT(hIdle != (HANDLE)0);                    // thread must be created
    SetThreadPriority(hIdle, THREAD_PRIORITY_LOWEST);
}
//............................................................................
void displyPhilStat(uint8_t n, char const *stat) {
    printf("Philosopher %2d is %s\n", (int)n, stat);
}
//............................................................................
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF::exit();
}



