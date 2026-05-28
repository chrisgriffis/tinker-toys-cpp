//////////////////////////////////////////////////////////////////////////////
// Product: QS/C++  port to 80x86, Linux/P-threads, g++
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
#include "qs_pkg.h"

// Global objects ------------------------------------------------------------
pthread_mutex_t QF_pThreadMutex_ = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
