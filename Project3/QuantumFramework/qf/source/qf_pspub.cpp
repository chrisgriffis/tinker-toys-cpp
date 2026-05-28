#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.1.03
// Date of the Last Update:  Nov 13, 2005
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
// terms of Quantum Leaps commercial licenses, which expressly allow the
// licensees to retain the proprietary status of their code. The licensees
// who use this software under one of Quantum Leaps commercial licenses do
// not use this software under the GPL and therefore are not subject to any
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE(qf_pspub)

/// \ingroup qf
/// \file qf_pspub.cpp
/// \brief QF::publish() implementation.

//............................................................................
void QF::publish(QEvent const *e) {
         // make sure that the published signal is within the configured range
    Q_REQUIRE(e->sig < QF_maxSignal_);

    uint8_t n = (uint8_t)0;           // for computing # subscribers to e->sig

    QF_SCHED_LOCK(); // perform multicasting with the scheduler locked, NOTE01

#if (QF_MAX_ACTIVE <= 8)
    uint8_t tmp = QF_subscrList_[e->sig].bits[0];
    while (tmp != (uint8_t)0) {
        uint8_t p = log2Lkup[tmp];
        tmp &= invPwr2Lkup[p];                     // clear the subscriber bit
        Q_ASSERT(active_[p] != (QActive *)0);            // must be registered

                                  // internally asserts if the queue overflows
        QACTIVE_POST_FIFO_(active_[p], e);                       // see NOTE02
        ++n;                                  // one more reference to event e
    }
#else
    uint8_t i = Q_DIM(QF_subscrList_[0].bits);  // number of bytes in the list
    do {                       // go through all bytes in the subsciption list
        --i;
        uint8_t tmp = QF_subscrList_[e->sig].bits[i];
        while (tmp != (uint8_t)0) {
            uint8_t p = log2Lkup[tmp];
            tmp &= invPwr2Lkup[p];                 // clear the subscriber bit
            p = (uint8_t)(p + (i << 3));                // adjust the priority
            Q_ASSERT(active_[p] != (QActive *)0);        // must be registered

                                  // internally asserts if the queue overflows
            QACTIVE_POST_FIFO_(active_[p], e);                   // see NOTE02
            ++n;                              // one more reference to event e
        }
    } while (i != (uint8_t)0);
#endif

    if (n != (uint8_t)0) {                                 // any subscribers?
        if (e->attrQF__ != (uint8_t)0) {    // is it a pool event?, see NOTE02
            ((QEvent *)e)->attrQF__ = (uint8_t)(e->attrQF__ + n); // cast away
              // the 'const', which is legitimate because this ia a pool event
        }

        QS_BEGIN_(QS_QF_PUBLISH, (void *)0, (void *)0);
            QS_TIME_();                                       // the timestamp
            QS_SIG_(e->sig);                        // the signal of the event
            QS_U8_(e->attrQF__);                // the attributes of the event
            QS_U8_(n);               // the number of subscribers to the event
        QS_END_();

        QF_SCHED_UNLOCK();
    }
    else {                  // no subscribers -- recycle the event immediately

        QS_BEGIN_(QS_QF_PUBLISH_ATTEMPT, (void *)0, (void *)0);
            QS_TIME_();                                       // the timestamp
            QS_SIG_(e->sig);                        // the signal of the event
            QS_U8_(e->attrQF__);                // the attributes of the event
        QS_END_();

        QF_SCHED_UNLOCK();
        gc(e);       // determine if the event is garbage and collect it if so
    }
}

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// QF::publish() uses scheduler locking to prevent other active objects from
// running while the event multicasting is in progress. During the publication
// the subscription lists are assumed not to be changing. The latter
// assumption is true as long as QActive::subscribe_()/unsubscribe()/
// unsubscribeAll() are NOT called from interrupts, because only interrupts
// can preempt the current task wile the scheduler is locked.
//
// NOTE02:
// The macro QACTIVE_POST_FIFO_() used for posting events does NOT increment
// the reference counter inside the event. Instead variable 'n' keeps track
// of the reference count. The reference counter is then incremented only
// once after verifying that the event comes from a pool. An alternative
// solution would be to use the method QActive_postFIFO(), which already does
// the correct reference counting. However, for better performance it's
// advantageous to test for the pool event only once rather than than many
// times all the while the scheduler is locked.
//////////////////////////////////////////////////////////////////////////////
