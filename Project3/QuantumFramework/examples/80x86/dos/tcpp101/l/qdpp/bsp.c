/*****************************************************************************
* Product: QDPP example, DOS, Turbo C++ 1.01
* Version: Compatible with QF/C 3.1.yy
* Updated: Aug 03, 2006
*
* Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
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
#include "qassert.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Local-scope objects -----------------------------------------------------*/
static void interrupt (*l_dosTmrISR)();
static void interrupt (*l_dosKbdISR)();
static void interrupt (*l_dosSpareISR)();

#define TMR_VECTOR      0x08
#define KBD_VECTOR      0x09
#define SPARE_VECTOR    0x81

/*..........................................................................*/
static void interrupt tmrISR() {
    QF_INT_UNLOCK(ignore);                             /* unlock interrupts */
    QF_tick();
    QF_INT_LOCK(ignore);                           /* lock interrupts again */
    geninterrupt(SPARE_VECTOR);     /* invoke the DOS timer ISR, see NOTE01 */
}
/*..........................................................................*/
static void interrupt kbdISR() {
    uint8_t key;
    uint8_t kcr;

    key = inport(0x60);        /*key scan code from the 8042 kbd controller */
    kcr = inport(0x61);                    /* get keyboard control register */
    outportb(0x61, (uint8_t)(kcr | 0x80));   /* toggle acknowledge bit high */
    outportb(0x61, kcr);                      /* toggle acknowledge bit low */
    QF_INT_UNLOCK(ignore);                             /* unlock interrupts */

    if (key == (uint8_t)129) {                          /* ESC key pressed? */
        QF_publish(Q_NEW(QEvent, TERMINATE_SIG));
    }

    QF_INT_LOCK(ignore);                           /* lock interrupts again */
    outportb(0x20, 0x20);                    /* write EOI to the master PIC */
}
/*..........................................................................*/
void QF_init(void) {
}
/*..........................................................................*/
void QF_start(void) {
                                      /* save the origingal DOS vectors ... */
    l_dosTmrISR   = getvect(TMR_VECTOR);
    l_dosKbdISR   = getvect(KBD_VECTOR);
    l_dosSpareISR = getvect(SPARE_VECTOR);

    QF_INT_LOCK(ignore);
    setvect(TMR_VECTOR, &tmrISR);
    setvect(KBD_VECTOR, &kbdISR);
    setvect(SPARE_VECTOR, l_dosTmrISR);
    QF_INT_UNLOCK(ignore);
}
/*..........................................................................*/
void QF_onIdle(void) {
    QF_INT_UNLOCK(ignore);
}
/*..........................................................................*/
void QF_exit(void) {
                                    /* restore the original DOS vectors ... */
    QF_INT_LOCK(ignore);
    setvect(TMR_VECTOR, l_dosTmrISR);
    setvect(KBD_VECTOR, l_dosKbdISR);
    setvect(SPARE_VECTOR, l_dosSpareISR);
    QF_INT_UNLOCK(ignore);

    _exit(0);                                                /* exit to DOS */
}
/*..........................................................................*/
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF_exit();
}
/*..........................................................................*/
void displyPhilStat(uint8_t n, char const *stat) {
    printf("Philosopher %2d is %s\n", (int)n, stat);
}
/*****************************************************************************
*
* NOTE01:
* The sofware interrupt generated at the end of the tick ISR invokes the
* original DOS tick ISR handler, which was installed at the spare vector
* 0x81. The DOS tick ISR handler generates the EOI (End-Of-Interrupt)
* to the master 8259A PIC.
*/