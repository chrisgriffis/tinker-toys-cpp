/*******************************************************************************/
/** @file                          qf_port.cpp
This file contains the code necessary to port the Quantum Framework to
Thread-X.
The main requirements are to support some form of locking around critical
sections of code, and to provide hooks to allow threads to be started.
<HR>
********************************************************************************/
// Copyright (c) 2006, Insitu Inc.

#include "qep.h"
#include "qassert.h"
#include "qf_port.h"
#include <stdio.h>
#include <stdlib.h>

Q_DEFINE_THIS_MODULE(qf_port)

// Package-scope objects -----------------------------------------------------
/** our global mutex used to control sensitive areas in the QF system */
TX_MUTEX QF_threadXMutex_;

/** this flag indicates if we have successflly started QF */
volatile uint8_t QF_running_;

// Local objects -------------------------------------------------------------
/**
if an active object is started with a stack size less that this value
we will allocate an additional stack area for it of at least this size
*/
const uint32_t MINIMUM_STACK_SIZE = 4096;

//............................................................................
/*******************************************************************************/
// QF::getPortVersion
/**
Returns a string describing the port version number.
 
@return              a pointer to the string
 
********************************************************************************/
const char *QF::getPortVersion(void)
{
    return "0.9.0";
}

/*******************************************************************************/
// QF::init
/**
Initialize the entire QF subsystem. This function is only called once

********************************************************************************/
void QF::init(void)
{
    // attempt to create the main mutex. If this succeeds then indicate we
    // are running
    if (tx_mutex_create(&QF_threadXMutex_, "QF mutex", TX_INHERIT) == TX_SUCCESS)
    {
        QF_running_ = 1;
    }
}

/*******************************************************************************/
// QF::start
/**
Once everything is initialized this function is called to start the QF system.
We need to do nothing.

********************************************************************************/
void QF::start(void)
{
}
/*******************************************************************************/
// QF::exit
/**
Stop everything. In our case we just indicate the system is no longer running.
This is not normally called.

********************************************************************************/
void QF::exit(void)
{
    QF_running_ = 0;
}

/*******************************************************************************/
// QF::run
/**
Once everything is initialized this function sits in a fairly infinite loop
until QF::stop is called.
While it is in that loop it continuously wakes up and processes ticks providing
a timing mechanism for QF.

********************************************************************************/
void QF::run(void)
{
    QF::start();                                        // start the QF

    while (QF_running_)
    {
        QF_SCHED_LOCK();
        QF::tick();                                     // process a time tick
        QF_SCHED_UNLOCK();
        tx_thread_sleep(1);
    }
    tx_mutex_delete(&QF_threadXMutex_);
}

/*******************************************************************************/
// run
/**
Since Thread-X is C-based it cannot directly start a member function of an object.
So we start a static function and the argument passed to it is the this pointer
of the object. From there we can call the member function and get back to C++.

@param  me  A ULONG that is really a pointer to a class. We cast it to the
            class and then can invoke the actual run() function of the class.
********************************************************************************/
static void run(ULONG me)
{
    ((QActive *)me)->run();
}

/*******************************************************************************/
// QActive::start
/**
This function actually creates the thread for an active object.
 
@param  prio        the priority of the object being started
@param  qSto        allocated memory for the message queue        
@param  qLen        how much memory is available for this queue
@param  stkSto      an allocated block of memory for this thread's stack
@param  stkSize     the size of memory allocated for the stack
@param  ie          the initial event being passed in
 
********************************************************************************/
void QActive::start(uint8_t prio,
                    QEvent const *qSto[], uint32_t qLen,
                    void *stkSto, uint32_t stkSize,
                    QEvent const *ie)
{
    // we need to name each task and mutex differently so this counter is used
    // to vary the names
    static int counter = 0;
    // where we will hold the name
    char buffer[16];
    // create a unique mutex name
    sprintf(buffer, "qf_mutex_%d", counter);
    
    // build the message queue for this active object
    eQueue_.init(qSto, qLen);
    
    // this semaphore is used to notify when something has been placed on the
    // empty event queue
    tx_semaphore_create(&osObject_, buffer, 0);

    prio_ = prio;
    
    QF::add_(this);                     // make QF aware of this active object
    init(ie);                           // execute the initial transition

    // do we need to allocate some stack space?
    if ((stkSto == 0) || (stkSize < MINIMUM_STACK_SIZE))
    {
        // not enough stack, create some of our own
        stkSto = new unsigned char[MINIMUM_STACK_SIZE];
        stkSize = MINIMUM_STACK_SIZE;
    }
    
    // now pick a unique name for the thread
    sprintf(buffer, "qf_thread_%d", counter);
    counter += 1;
    
    // start this active object
    tx_thread_create(&thread_, buffer, &::run, (ULONG)this, stkSto, stkSize,
        prio, prio, 1, TX_AUTO_START);
}

/*******************************************************************************/
// QActive::stop
/**
This function stops an active object. Since we don't want to delete the thread
we just shut it down and make sure the system will detect an invalid usage.
 
********************************************************************************/
void QActive::stop(void)
{
    running_ = (uint8_t)0;                              // stop the run() loop

    tx_semaphore_delete(&osObject_);        // cleanup the condition variable
       // make sure posting events to the event queue raises assertion, NOTE05
    eQueue_.nTot_ = (QEQueueCtr)0;
}

