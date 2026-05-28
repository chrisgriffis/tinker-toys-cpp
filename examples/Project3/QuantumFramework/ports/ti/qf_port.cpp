//////////////////////////////////////////////////////////////////////////////
// Product:  QF/C++ port to TI DSP BIOS
// Version:  Compatible with QF/C 3.2.yy
// Updated:  Feb 07, 2007
// The Insitu Group
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
//
// This QF/C port is part of the Quantum Leaps QF/C software, and may be
// distributed and modified under the terms of the GNU General Public License
// version 2 (GPL) as published by the Free Software Foundation and appearing
// in the file GPL.TXT included in the packaging of this file. Please note
// that GPL Section 2[b] requires that all works based on this software must
// also be made publicly available under the terms of the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified in conjunction
// with a valid QF/C Quantum Leaps commercial license. Quantum Leaps
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
#include "qf_pkg.h"
#include "qassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atm.h>
#include "..\..\..\insituHAL\dp_OVS\dp_OVScfg.h"

Q_DEFINE_THIS_MODULE(qf_port)

extern "C" void *imMemset(void *dst, int val, unsigned int len);

// Global objects ------------------------------------------------------------
uint8_t QF_running_ = 0;

LCK_Handle QF_TI_BIOS_Lck_;

//............................................................................
const char *QF::getPortVersion(void) {
    return "1.2.01";
}
//............................................................................
void QF::init(void) {
	QF_TI_BIOS_Lck_ = LCK_create(0);
}
//............................................................................
void QF::cleanup(void) {
}
//............................................................................

void QF::start(void) {
    QF_running_ = (uint8_t)1;
}
//............................................................................
void QF::stop(void) {
    QF_running_ = (uint8_t)0;
}

//............................................................................
void QF::run(void) {
    QF::start();                                               // start the QF

    while (QF_running_) {

        QF_SCHED_LOCK();
        QF::tick();                                     // process a time tick
        QF_SCHED_UNLOCK();

        TSK_sleep(10);                               // wait for the tick interval
    }
}

void runFxn(TSK_Handle task, QActive *a ) {
   ((QActive *)a)->run();

	// Should never get here!
	Q_ASSERT(0);
}
extern int EXTERNALHEAP;
//............................................................................
void QActive::start(uint8_t prio,
                    QEvent const *qSto[], uint32_t qLen,
                    void *stkSto, uint32_t stkSize,
                    QEvent const *ie)
{
	TSK_Attrs *taskAttrs;
	Q_ASSERT(prio >= TSK_MINPRI);
	Q_ASSERT(prio <= TSK_MAXPRI);
	Q_ASSERT(qSto == 0);    		// DM640 allocates these
	eQueue_ = MBX_create(sizeof(QEvent *), qLen, NULL);
	// Don't allow illegal priority
	if(prio>TSK_MAXPRI){
	   prio = TSK_MAXPRI;
	}
	prio_ = prio;
	QF::add_(this);              // make QF aware of this active object
	init(ie);                                    // execute initial transition

	// Create task initialization structure on heap
	taskAttrs = (TSK_Attrs*)malloc(sizeof(TSK_Attrs));
	imMemset(taskAttrs,0,sizeof(taskAttrs));

	taskAttrs->priority = prio; /* execution priority */
	taskAttrs->stack = stkSto;
	taskAttrs->stacksize = stkSize;
	taskAttrs->stackseg = EXTERNALHEAP;
	taskAttrs->name = myThreadName;

	// Start task running
	thread_ = TSK_create((Fxn)runFxn, taskAttrs, this);
	if (thread_ == NULL) {
	  Q_ASSERT(0);
	}   
}
//............................................................................
void QActive::stop(void) {
   QF::remove_(this);
   TSK_exit();
}
void NetworkDebugString(const char *format, ...);
int UberDebug = 0;
void UberDebugToggle()
{
	UberDebug = !UberDebug;
}
//...................................................................
int postrecsig = 0;

void QActive::postFIFO(QEvent const *e) {
#if 0
   if(UberDebug)
      NetworkDebugString("Posting: %i\tto:%s\r\n",e->sig,this->myThreadName);
#endif
   int tempCurCount = ATM_inci(&curEventCount);

    if (e->attrQF__ != (uint8_t)0) {                    // is it a pool event?
        //lint -e1773                               Attempt to cast away const
        ++((QEvent *)e)->attrQF__;          // increment the reference counter
                   // NOTE: cast the 'const' away, which is legitimate because
                   // it's a pool event
    }

   if(tempCurCount > maxQueuedEvents)
	   maxQueuedEvents = tempCurCount;
   Q_ALLEGE(MBX_post(eQueue_, &e, 0));              // queue cannot overflow
}

QEvent const *QActive::get_(void)
{
	QEvent *e;
	MBX_pend(eQueue_, &e, SYS_FOREVER);          // wait for event
	ATM_deci(&curEventCount);
	return e;
}

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// QACTIVE_OSOBJECT_SIGNAL_(me_) in the Win32 port qf_port.h makes the
// following assertion:
// Q_ASSERT((me_)->osObject_ != (HANDLE)0);
//
