/*******************************************************************************/
/** @file                          qf_port.h
This file contains the macros necessary to port the Quantum Framework to
Thread-X.
The main requirements are to support some form of locking around critical
sections of code, and to provide hooks to allow threads to be started.
<HR>
********************************************************************************/
// Copyright (c) 2006, Insitu Inc.

#ifndef qf_port_h
#define qf_port_h

#include "tx_api.h"
#include <errno.h>

/** the queue type we use is a queue built internally */
#define QF_EQUEUE_TYPE              QEQueue
/**
Each active object uses a flag to indicate if there is anything to process.
In order to be most efficient with Thread-X we use a semaphore so a thread
can just hang out until something is available and then wake up immediately
*/
#define QF_OS_OBJECT_TYPE           TX_SEMAPHORE

/** Each active object is a thread. This is the type used to hold that information */

#define QF_THREAD_TYPE              TX_THREAD

/**
These constants are used to indicate the size of various items.
Since the 555 maintains pointers as 32-bit values the 4 is sufficient.
The only value that might need tweaking is QF_MAX_ACTIVE which represents
the maximum number of active objects we have available. If this is changed
the QActive::start function needs to be modified to choose priorities
of the thread in a different way.
*/
#define QF_MAX_ACTIVE               31
#define QF_EVENT_SIZ_SIZE           4
#define QF_EQUEUE_CTR_SIZE          4
#define QF_MPOOL_SIZ_SIZE           4
#define QF_MPOOL_CTR_SIZE           4
#define QF_TIMEEVT_CTR_SIZE         4

/**
In order to protect critical sections of the code we need to lock out other
changes. A mutex is used to manage this and these macros access it. The arguments
are unused
*/
#define QF_INT_LOCK(key_)           tx_mutex_get(&QF_threadXMutex_, TX_WAIT_FOREVER)
#define QF_INT_UNLOCK(key_)         tx_mutex_put(&QF_threadXMutex_)

/**
Another method of locking used by QF. The implementation has no different requirements so
we use the same mutex
*/
#define QF_SCHED_LOCK()             tx_mutex_get(&QF_threadXMutex_, TX_WAIT_FOREVER)
#define QF_SCHED_UNLOCK()           tx_mutex_put(&QF_threadXMutex_)

// these includes are placed here since they depend on the #defines above. Don't move
// them to the top of the file
#include "qep_port.h"              // QEP port
#include "qequeue.h"               // use the internal queue
#include "qmpool.h"
#include "qf.h"                    // QF platform-independent public interface

//////////////////////////////////////////////////////////////////////////////
// interface used only inside QF, but not in applications
//

/**
This macro is used by active objects to wait for work to perform. It basically
sits on the object semaphore waiting until it is indicated as available.
There is a short cut that if something is already in the queue it won't check
the semaphore again.
*/
#define QACTIVE_OSOBJECT_WAIT_(me_) \
    if ((me_)->eQueue_.frontEvt_ == (QEvent const *)0) { \
        do { \
            QF_INT_UNLOCK(0); \
            tx_semaphore_get(&(me_)->osObject_, TX_WAIT_FOREVER); \
            QF_INT_LOCK(0); \
        } while ((me_)->eQueue_.frontEvt_ == (QEvent const *)0); \
    } else ((void)0)

/** This macro places a message in the queue of an active object. */    
#define QACTIVE_OSOBJECT_SIGNAL_(me_) \
    QF_INT_UNLOCK(0); \
    Q_ASSERT((me_)->eQueue_.nTot_ != (QEQueueCtr)0); \
    tx_semaphore_put(&(me_)->osObject_)

#define QACTIVE_OSOBJECT_ONIDLE_(me_) ((void)0)

/**
These macros are to handle the queues in the system. We use the
QF-provided queues
*/
#define QACTIVE_POST_FIFO_(me_, e_) ((me_)->postFIFO_(e_))
#define QACTIVE_POST_LIFO_(me_, e_) ((me_)->postLIFO_(e_))
#define QACTIVE_GET_(me_, e_)       ((e_) = (me_)->get_())

/**
These macros are to handle the memory pools in the system. We use the
QF-provided pools
*/
#define QF_EPOOL_TYPE_              QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    (p_).init(poolSto_, poolSize_, evtSize_)
#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_).getBlockSize())
#define QF_EPOOL_GET_(p_, e_)       ((e_) = (QEvent *)(p_).get())
#define QF_EPOOL_PUT_(p_, e_)       ((p_).put(e_))

/** We create a Thread-X mutex to lock critical sections. */
extern TX_MUTEX QF_threadXMutex_;

/** This flag is used to indicate if QF has been successfully started */
extern volatile uint8_t QF_running_;

// NOTES: ////////////////////////////////////////////////////////////////////
//
// NOTE01:
// QF, like all real-time frameworks, needs to execute certain sections of
// code indivisibly to avoid data corruption. The most straightforward way of
// protecting such critical sections of code is disabling and enabling
// interrupts, which Linux does not allow.
//
// This QF port uses a Thread-X mutex to protect all critical sections. The
// mutex is locked upon the entry to each critical sectioni and unlocked upon
// exit.
//
// Using the single mutex for all critical section guarantees that only one
// thread at a time can execute inside a critical section. This prevents race
// conditions and data corruption.
//
// Please note, however, that the mutex implementation of a critical section
// behaves differently than the standard interrupt locking. A common mutex
// ensures that only one thread at a time can execute a critical section, but
// it does not guarantee that a context switch cannot occur within the
// critical section. In fact, such context switches probably will happen, but
// they shouldnot cause concurrency hazards because the mutex eliminates all
// race conditionis.
//
// NOTE02:
// QF uses scheduler locking when it publishes events to multiple subscribers
// and in this port also when QF::tick() is invoked.
//
// The Thread-X API does not support scheduler locking.
//
// Therefore, this port implements scheduler locking as a critical section,
// which in turn is implemented with the common mutex QF_threadXMutex_.
//
// Please note that the consequence of using critical section for scheduler
// locking is nesting of critical sections. This means that the common mutex
// QF_threadXMutex_ must be initialized as a RECURSIVE mutex.
//

#endif                                                            // qf_port_h
