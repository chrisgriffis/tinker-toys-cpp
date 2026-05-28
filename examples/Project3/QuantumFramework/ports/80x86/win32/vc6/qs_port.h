//////////////////////////////////////////////////////////////////////////////
// Product: QS/C++  port to 80x86, Win32, Visual C++ 6.0
// Last Updated for Version: 3.1.01
// Date of the Last Update:  Oct 21, 2005
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
#ifndef qs_port_h
#define qs_port_h

////INSITU DLL STUFF////
#ifndef QF_NODLLIMPORT
    #define QF_API __declspec(dllimport)
#else
    #define QF_API
#endif
////END INSITU DLL STUFF////

#define QS_TIME_SIZE                2
#define QS_OBJ_PTR_SIZE             4
#define QS_FUN_PTR_SIZE             4

// QS critical section must be the same as the QF critical section, NOTE01
// QS_INT_KEY_TYPE not defined
#define QS_INT_LOCK(key_)           EnterCriticalSection(&::QF_win32CritSect_)

#define QS_INT_UNLOCK(key_)         LeaveCriticalSection(&::QF_win32CritSect_)

#include <windows.h>                                              // Win32 API
#include "qs.h"                    // QS platform-independent public interface

extern QF_API CRITICAL_SECTION QF_win32CritSect_;

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// QS might be used with or without the QF (e.g., only with QEP, or even
// without any other QP components). In this case, the separate QS critical
// section policy is needed. However, when QS is used with the QF, it must
// use the *same* critical section because QF performs the QS output within
// QF's critical sections.

#endif                                                            // qs_port_h
