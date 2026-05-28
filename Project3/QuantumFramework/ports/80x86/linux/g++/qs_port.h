//////////////////////////////////////////////////////////////////////////////
// Product: QS/C++  port to 80x86, Linux, g++
// Last Updated for Version: 3.1.06
// Date of the Last Update:  May 06, 2006
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (c) 2002-2006 Quantum Leaps, LLC. All rights reserved.
//
// Internet: www.quantum-leaps.com     Licensing: sales@quantum-leaps.com
//
// This Software is protected by the United States copyright laws and
// international treaties. Distribution of products containing this Software
// or based upon this Software (Derivative Works) requires a valid Quantum
// Leaps Distribution License. Any other distribution, in source or binary
// format is illegal.
//////////////////////////////////////////////////////////////////////////////
#ifndef qs_port_h
#define qs_port_h


#define QS_TIME_SIZE                4
#define QS_OBJ_PTR_SIZE             4
#define QS_FUN_PTR_SIZE             4

// QS critical section must be the same as the QF critical section, see NOTE01
// QS_INT_KEY_TYPE not defined
#define QS_INT_LOCK(key_)           pthread_mutex_lock(&QF_pThreadMutex_)
#define QS_INT_UNLOCK(key_)         pthread_mutex_unlock(&QF_pThreadMutex_)

#ifndef __USE_GNU
#define __USE_GNU
#include <pthread.h>
#undef __USE_GNU
#else
#include <pthread.h>
#endif
#include "qs.h"                    // QS platform-independent public interface

extern pthread_mutex_t QF_pThreadMutex_;


//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// QS might be used with or without the QF (e.g., only with QEP, or even
// without any other QP components). In this case, the separate QS critical
// section policy is needed. However, when QS is used with the QF, it must
// use the *same* critical section because QF performs the QS output within
// QF's critical sections.
//
#endif                                                            // qs_port_h
