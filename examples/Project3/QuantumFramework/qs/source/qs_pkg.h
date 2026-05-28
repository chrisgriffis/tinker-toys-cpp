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
#ifndef qs_pkg_h
#define qs_pkg_h

/// \ingroup qs
/// \file qs_pkg.h
/// \brief Internal (package scope) QS/C++ interface.

/// \brief Defining this macro enables the QS trace generation.
///
/// The QS trace is DISABLED at compile-time if the macro is not defined.
/// This should be defined only on the compile command-line
//#define Q_SPY
#include "qs_port.h"                                                // QS port

/// \brief QS ring buffer counter and offset type
typedef uint16_t QSCtr;

/// \brief Internal QS macro to insert an un-escaped byte into
/// the QS buffer
////
#define QS_INSERT_BYTE(b_) \
    if (QS_ring_) {\
        QS_ring_[QS_head_++] = (b_); \
        if (QS_head_ == QS_end_) { \
            QS_head_ = (QSCtr)0; \
        } \
        ++QS_used_;\
    }

/// \brief Internal QS macro to insert an escaped byte into the QS buffer
#define QS_INSERT_ESC_BYTE(b_) \
    QS_chksum_ = (uint8_t)(QS_chksum_ + (b_)); \
    if (((b_) == QS_FRAME) || ((b_) == QS_ESC)) { \
        QS_INSERT_BYTE(QS_ESC) \
        QS_INSERT_BYTE((uint8_t)((b_) ^ QS_ESC_XOR)) \
    } \
    else { \
        QS_INSERT_BYTE(b_) \
    }

/// \brief Internal QS macro to insert a escaped checksum byte into
/// the QS buffer
#define QS_INSERT_CHKSUM_BYTE() \
    QS_chksum_ = (uint8_t)~QS_chksum_; \
    if ((QS_chksum_ == QS_FRAME) || (QS_chksum_ == QS_ESC)) { \
        QS_INSERT_BYTE(QS_ESC) \
        QS_INSERT_BYTE((uint8_t)(QS_chksum_ ^ QS_ESC_XOR)) \
    } \
    else { \
        QS_INSERT_BYTE(QS_chksum_) \
    }


/// \brief Frame character of the QS output protocol
#define QS_FRAME    ((uint8_t)0x7E)

/// \brief Escape character of the QS output protocol
#define QS_ESC      ((uint8_t)0x7D)

/// \brief Escape modifier of the QS output protocol
///
/// The escaped byte is XOR-ed with the escape modifier before it is inserted
/// into the QS buffer.
#define QS_ESC_XOR  0x20

//............................................................................
extern QF_API uint8_t *QS_ring_;         ///< pointer to the start of the ring buffer
extern QF_API QSCtr QS_end_;                ///< offset of the end of the ring buffer
extern QF_API QSCtr QS_head_;         ///< offset to where next byte will be inserted
extern QF_API QSCtr QS_tail_;       ///< offset of where next event will be extracted
extern QF_API QSCtr QS_used_;       ///< number of bytes currently in the ring buffer
extern QF_API uint8_t QS_seq_;                        ///< the record sequence number
extern QF_API uint8_t QS_chksum_;             ///< the checksum of the current record
extern QF_API uint8_t QS_full_;              ///< the ring buffer is temporarily full

#endif                                                             // qs_pkg_h
