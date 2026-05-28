#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QEP/C++
// Last Updated for Version: 3.1.00
// Date of the Last Update:  Sep 25, 2005
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
// of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qep_pkg.h"

//............................................................................
/// \ingroup qep
/// \file qhsm_dis.cpp
/// \brief QHsm::dispatch() implementation.

//............................................................................
void QHsm::dispatch(QEvent const *e) {
    QHsmState s;
    QHsmState t = state_.hsm;
    QHsmState path[QEP_MAX_NEST_DEPTH_];

    path[2] = t;       // save the current state in case a transition is taken

    do {                                // process the event hierarchically...
        s = t;
        t = (QHsmState)((*s)(this, e));    // invoke handler through pointer s
    } while (t != (QHsmState)0);

    if (tran_ != Q_TRAN_NONE_TYPE) {                      // transition taken?
        QTran_ *stran;
        if ((tran_ & Q_TRAN_STA_TYPE) != 0) {            // static transition?
            stran = state_.tran;
        }
        else {
            path[0] = state_.hsm;                // save the transition target
            stran = (QTran_ *)0;
        }
        state_.hsm = path[2];                         // restore current state
        path[1] = s;                             // save the transition source

                     // exit current state to the transition source path[1]...
        for (s = path[2]; s != path[1]; ) {
            t = QEP_TRIG_(s, Q_EXIT_SIG);
            if (t != (QHsmState)0) {                  // exit action unhandled
                s = t;                               // t points to superstate
            }
            else {                                      // exit action handled

                QS_BEGIN_(QS_QEP_STATE_EXIT, QS::smObj_, this);
                    QS_TIME_();                                   // timestamp
                    QS_OBJ_(this);                // this state machine object
                    QS_FUN_(s);                            // the exited state
                QS_END_();

                s = QEP_TRIG_(s, QEP_EMPTY_SIG_);   // find out the superstate
            }
        }

        if (stran != (QTran_ *)0) {                      // static transition?
            uint16_t a = (uint8_t)stran->actions[0];
            if (a != (uint16_t)0) {                 // transition initialized?
                QHsmState const *c = stran->chain;
                for (a = (uint16_t)((uint16_t)(a | (stran->actions[1] << 8))
                                    >> 1);
                     a != (uint16_t)0;
                     a >>= 2)
                {
                    uint8_t sig = (uint8_t)(a & 0x3);
                    (void)(*(*c))(this, &QEP_reservedEvt_[sig]);

                    QS_BEGIN_(sig + (uint8_t)QS_QEP_STATE_EMPTY,
                              QS::smObj_, this);
                        QS_OBJ_(this);            // this state machine object
                        QS_FUN_(*c);       // entered/exited/initialized state
                        if (sig == (QSignal)Q_INIT_SIG) {
                            QS_FUN_(state_.hsm);     // target of initial tran
                        }
                    QS_END_();

                    ++c;         // advance in the chain of the stored actions
                }
                state_.hsm = *c;                              // set new state
            }
            else {         // the static transition object not initialized yet
                path[0] = stran->chain[0];       // save the transition target
                s = path[2];                     // save the transition source
                execTran(path, stran);
            }
        }
        else {                                           // dynamic transition
            s = path[2];                         // save the transition source
            execTran(path, (QTran_ *)0);
        }
        tran_ = Q_TRAN_NONE_TYPE;    // clear the attributes for the next time

        QS_BEGIN_(QS_QEP_TRAN, QS::smObj_, this);
            QS_TIME_();                                          // time stamp
            QS_SIG_(e->sig);                        // the signal of the event
            QS_OBJ_(this);                        // this state machine object
            QS_FUN_(s);                        // the source of the transition
            QS_FUN_(state_.hsm);                       // the new active state
        QS_END_();
		notifyStateHasChanged((QHsmState) state_.hsm);
    }
    else {                                             // transition not taken
        #ifdef Q_SPY
        if (s != &QHsm::top) {                               // event handled?

            QS_BEGIN_(QS_QEP_INTERN_TRAN, QS::smObj_, this);
                QS_TIME_();                                      // time stamp
                QS_SIG_(e->sig);                    // the signal of the event
                QS_OBJ_(this);                    // this state machine object
                QS_FUN_(s);                // the state that handled the event
                QS_FUN_(state_.hsm);                // the object's full state
            QS_END_();

        }
        else {                                              // event unhandled

            QS_BEGIN_(QS_QEP_IGNORED, QS::smObj_, this);
                QS_TIME_();                                      // time stamp
                QS_SIG_(e->sig);                    // the signal of the event
                QS_OBJ_(this);                    // this state machine object
                QS_FUN_(state_.hsm);                       // the active state
            QS_END_();

        }
        #endif
    }
}
