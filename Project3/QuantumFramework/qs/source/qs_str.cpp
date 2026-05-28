#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QS/C++
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
#include <wchar.h>
#include "wince_helpers.h" // ahayes: for mapping from wcrtomb_s --> wcrtomb

/// \ingroup qs
/// \file qs_str.cpp
/// \brief QS::str() implementation

//............................................................................

#ifdef UNICODE
void QS::str(char const *s) {
    QS_INSERT_BYTE((uint8_t)QS_STR_T);
    QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)QS_STR_T);
    while (*s != '\0') {
        QS_INSERT_ESC_BYTE((uint8_t)(*s));
        ++s;
    }
    QS_INSERT_BYTE((uint8_t)0);
}
#endif

#ifdef QSPY_UNICODE_TO_MULTIBYTE
//............................................................................
void QS::str(TCHAR const* s)
{
    size_t pReturnValue;
    char mbchar;
    mbstate_t mbstate;

    // Reset to initial conversion state
    memset(&mbstate, 0, sizeof(mbstate));

    QS_INSERT_BYTE((uint8_t)QS_STR_T);
    QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)QS_STR_T);
    for (wcrtomb_s(&pReturnValue, &mbchar, 1, *s, &mbstate); mbchar != '\0'; wcrtomb_s(&pReturnValue, &mbchar, 1, *s, &mbstate)) {
        QS_INSERT_ESC_BYTE((uint8_t)(mbchar));
        ++s;
    }
    QS_INSERT_BYTE((uint8_t)0);
}
#else
//............................................................................
void QS::str(TCHAR const* s)
{
    // All Insitu unicode logging is ASCII encoded in wide chars
    // so truncation is safe for a significant performance gain.
    QS_INSERT_BYTE((uint8_t)QS_STR_T);
    QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)QS_STR_T);
    while (*s != '\0') {
        QS_INSERT_ESC_BYTE((uint8_t)(*s));
        ++s;
    }
    QS_INSERT_BYTE((uint8_t)0);
}
#endif
