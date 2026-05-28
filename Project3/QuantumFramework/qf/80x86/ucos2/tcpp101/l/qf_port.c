/*****************************************************************************
* Product:  QF/C, port to 80x86, uC/OS-II, Turbo C++ 1.01, Large model
* Last Updated for Version: 3.2.00
* Date of the Last Update:  Aug 06, 2006
*
* Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly allow the
* licensees to retain the proprietary status of their code. The licensees
* who use this software under one of Quantum Leaps commercial licenses do
* not use this software under the GPL and therefore are not subject to any
* of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
* Quantum Leaps products:  http://www.quantum-leaps.com/products
* e-mail:                  sales@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qf_port)

/* local objects -----------------------------------------------------------*/
static void interrupt (*l_dosTickISR)(void);
static void interrupt (*l_dosSpareISR)(void);

#define TMR_VECTOR         0x08
#define DOS_CHAIN_VECTOR   0x81
/*..........................................................................*/
const char Q_ROM *QF_getPortVersion(void) {
   return (const char Q_ROM *)"1.1.05";
}
/*..........................................................................*/
void QF_init(void) {
    OSInit();                                        /* initialize uC/OS-II */
    l_dosTickISR  = getvect(TMR_VECTOR);
    l_dosSpareISR = getvect(uCOS);
}
/*..........................................................................*/
void QF_start(void) {
                                      /* divisor for the 8254 timer/counter */
    uint16_t count = (uint16_t)(((1193180 * 2) / OS_TICKS_PER_SEC + 1) >> 1);
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
                                  /* install uC/OS-II context switch vector */
    setvect(uCOS, (void interrupt (*)(void))OSCtxSw);
             /* install the original DOS timer vector for uC/OS-II to chain */
    setvect(DOS_CHAIN_VECTOR, l_dosTickISR);
                                        /* install the uC/OS-II tick vector */
    setvect(TMR_VECTOR, (void interrupt (*)(void))OSTickISR);

    outportb(0x43, 0x36);             /* use mode-3 for timer 0 in the 8254 */
    outportb(0x40, count & 0xFF);              /* load low  byte of timer 0 */
    outportb(0x40, (count >> 8) & 0xFF);       /* load high byte of timer 0 */
    QF_INT_UNLOCK_();
}
/*..........................................................................*/
void QF_run(void) {
    OSStart();                               /* start uC/OS-II multitasking */
}
/*..........................................................................*/
void QF_exit(void) {
    QF_INT_LOCK_KEY_
    QF_INT_LOCK_();
    outportb(0x43, 0x36);             /* use mode-3 for timer 0 in the 8254 */
    outportb(0x40, 0);                         /* load low  byte of timer 0 */
    outportb(0x40, 0);                         /* load high byte of timer 0 */
    setvect(TMR_VECTOR, l_dosTickISR);   /* restore the original DOS vector */
    setvect(uCOS, l_dosSpareISR);        /* restore the original DOS vector */
    QF_INT_UNLOCK_();
    _exit(0);                                                /* exit to DOS */
}
/*..........................................................................*/
static void run(void *me) {           /* use exactly the expected signature */
    ((QActive *)me)->running__ = (uint8_t)1;/* allow the thread-loop to run */
    do {
        QEvent const *e;
                               /* get the next event for this active object */
        QACTIVE_GET_((QActive *)me, e);

        QF_ACTIVE_DISPATCH_(me, e);   /* dispatch to the active object's SM */

        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    } while (((QActive *)me)->running__);
    QActive_unsubscribeAll_((QActive *)me); /* unsubscribe from all signals */
    QF_remove_((QActive *)me); /* remove this object from any subscriptions */
    OSTaskDel(OS_PRIO_SELF);
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                   QEvent const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvent const *ie)
{
    int8_t ucosPrio;
    me->eQueue__ = OSQCreate((void **)qSto, qLen);
    Q_ASSERT(me->eQueue__ != (OS_EVENT *)0);      /* uC/OS-II queue created */
    me->prio__ = prio;                              /* save the QF priority */
    QF_add_(me);                     /* make QF aware of this active object */
    ucosPrio = (uint8_t)(QF_MAX_ACTIVE - me->prio__);/*map QF prio to uC/OS */
    QF_ACTIVE_INIT_(me, ie);                  /* execute initial transition */
    Q_ALLEGE(OSTaskCreate(&run,
                     me,
                     &(((OS_STK *)stkSto)[(stkSize / sizeof(OS_STK)) - 1]),
                     ucosPrio) == OS_NO_ERR);
                        /* uC/OS task is represented by its unique priority */
    me->thread__ = (uint8_t)ucosPrio;
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    INT8U err;
    ((QActive *)me)->running__ = (uint8_t)0;        /* stop the thread loop */
    OSQDel(((QActive *)me)->eQueue__, OS_DEL_ALWAYS, &err); /*cleanup queue */
    /* make sure posting events to the event queue raises assertion, NOTE01 */
    ((QActive *)me)->eQueue__ = (OS_EVENT *)0;
}

/*****************************************************************************
* NOTE01:
* The qf_port.h file for the uC/OS-II port asserts the error-free posing
* of events in the macros QACTIVE_POST_FIFO_(me_) and QACTIVE_POST_LIFO_()
* The uC/OS-II message queue functions OSQPost() and OSQPostFront() return
* error OS_ERR_PEVENT_NULL when the queue pointer is NULL.
*/
