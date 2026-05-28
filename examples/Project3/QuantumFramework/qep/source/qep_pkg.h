//////////////////////////////////////////////////////////////////////////////
// Product: QEP/C++
// Last Updated for Version: 3.1.01
// Date of the Last Update:  Nov 21, 2005
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
#ifndef qep_pkg_h
#define qep_pkg_h

/// \ingroup qep
/// \file qep_pkg.h
/// \brief Internal (package scope) QEP/C++ interface.

#include "qep_port.h"                                              // QEP port

#ifdef Q_SPY
    #include "qs_port.h"                                            // QS port
#else
    #define QS_BEGIN_(rec_, refObj_, obj_)  if (0) { ((void)0)
    #define QS_END_()                       } else ((void)0)
    #define QS_U8_(data_)                   ((void)0)
    #define QS_U16_(data_)                  ((void)0)
    #define QS_U32_(data_)                  ((void)0)
    #define QS_TIME_()                      ((void)0)
    #define QS_SIG_(sig_)                   ((void)0)
    #define QS_OBJ_(obj_)                   ((void)0)
    #define QS_FUN_(fun_)                   ((void)0)
#endif

extern QF_API QEvent const QEP_reservedEvt_[];      ///< preallocated reserved events

/// internal QEP constants
enum QEPConst {
    QEP_EMPTY_SIG_ = 0,                ///< empty signal for internal use only

    /// maximum depth of state nesting (including the top level), must be >= 3
    QEP_MAX_NEST_DEPTH_ = 10
};

/// helper macro to trigger internal event in an HSM
#define QEP_TRIG_(state_, sig_) \
    ((QHsmState)(*(state_))(this, &QEP_reservedEvt_[sig_]))

#endif                                                            // qep_pkg_h
