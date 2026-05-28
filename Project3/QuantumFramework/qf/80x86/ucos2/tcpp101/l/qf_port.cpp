//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++ port to x86, uC/OS-II, Turbo C++ 1.01, Large model
// Version: Compatible with QF/C++ 3.1.yy
// Updated: Dec 11, 2005
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
// Alternatively, this software may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which are designed for users
// who want to retain proprietary status of their code. This "dual-licensing"
// model is possible because Quantum Leaps owns the copyright to this source
// code and as such can license its intelectual property any number of times.
// The users who license this software under one of Quantum Leaps commercial
// licenses do not use this software under the GPL and therefore are not
// subject to any of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qf_port)

// local objects -------------------------------------------------------------
static void interrupt (*l_dosTickISR)(...);
static void interrupt (*l_dosSpareISR)(...);

#define TMR_VECTOR         0x08
#define DOS_CHAIN_VECTOR   0x81

//............................................................................
//lint -e970 -e971               ignore MISRA rules 13 and 14 in this function
const char *QF::getPortVersion(void) {
    return "1.1.04";
}
//............................................................................
void QF::init(void) {
    OSInit();                                           // initialize uC/OS-II

    l_dosTickISR  = getvect(TMR_VECTOR);
    l_dosSpareISR = getvect(uCOS);
}
//............................................................................
void QF::start(void) {
                                         // divisor for the 8254 timer/counter
    uint16_t count = (uint16_t)(((1193180 * 2) / OS_TICKS_PER_SEC + 1) >> 1);

    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
                                     // install uC/OS-II context switch vector
    setvect(uCOS, (void interrupt (*)(...))OSCtxSw);
                // install the original DOS timer vector for uC/OS-II to chain
    setvect(DOS_CHAIN_VECTOR, l_dosTickISR);
                                           // install the uC/OS-II tick vector
    setvect(TMR_VECTOR, (void interrupt (*)(...))OSTickISR);

    outportb(0x43, 0x36);             /* use mode-3 for timer 0 in the 8254 */
    outportb(0x40, count & 0xFF);              /* load low  byte of timer 0 */
    outportb(0x40, (count >> 8) & 0xFF);       /* load high byte of timer 0 */
    QF_INT_UNLOCK_();
}
//............................................................................
void QF::run(void) {
    OSStart();                               /* start uC/OS-II multitasking */
}
//............................................................................
void QF::exit(void) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
    outportb(0x43, 0x36);                // use mode-3 for timer 0 in the 8254
    outportb(0x40, 0);                            // load low  byte of timer 0
    outportb(0x40, 0);                            // load high byte of timer 0
    setvect(TMR_VECTOR, l_dosTickISR);      // restore the original DOS vector
    setvect(uCOS, l_dosSpareISR);           // restore the original DOS vector
    QF_INT_UNLOCK_();
    _exit(0);                                                   // exit to DOS
}
//............................................................................
static void run(void *me) {              // use exactly the expected signature
    ((QActive *)me)->run();
    OSTaskDel(OS_PRIO_SELF);                        // clean the uC/OS-II task
}
//............................................................................
void QActive::start(uint8_t prio,
                    QEvent const *qSto[], uint32_t qLen,
                    void *stkSto, uint32_t stkSize,
                    QEvent const *ie)
{
    Q_REQUIRE((qSto != (QEvent *)0) && (stkSto != (void *)0));

    int ucosPrio;
    eQueue_ = OSQCreate((void **)qSto, qLen);
    Q_ASSERT(eQueue_ != (void *)0);                  // uC/OS-II queue created
    prio_ = prio;                                       // set the QF priority
    QF::add_(this);                     // make QF aware of this active object
    ucosPrio = QF_MAX_ACTIVE - prio_;           // map QF priority to uC/OS-II
    init(ie);                                // execute the initial transition
    Q_ALLEGE(OSTaskCreate(&::run,
                     this,
                     &(((OS_STK *)stkSto)[(stkSize / sizeof(OS_STK)) - 1]),
                     ucosPrio) == OS_NO_ERR);
                           // uC/OS task is represented by its unique priority
    thread_ = (uint8_t)ucosPrio;
}
//............................................................................
void QActive::stop(void) {
    running_ = (uint8_t)0;   // clear the loop variable used in QActive::run()

    INT8U err;
    OSQDel(eQueue_, OS_DEL_ALWAYS, &err);        // cleanup the uC/OS-II queue
       // make sure posting events to the event queue raises assertion, NOTE01
    eQueue_ = (OS_EVENT *)0;
}

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// The qf_port.h file for the uC/OS-II port asserts the error-free posing
// of events in the macros QACTIVE_POST_FIFO_(me_) and QACTIVE_POST_LIFO_()
// The uC/OS-II message queue functions OSQPost() and OSQPostFront() return
// error OS_ERR_PEVENT_NULL when the queue pointer is NULL.

