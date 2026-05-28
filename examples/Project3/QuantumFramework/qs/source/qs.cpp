#include "precompiled.h"
//////////////////////////////////////////////////////////////////////////////
// Product: QS/C++
// Last Updated for Version: 3.2.00
// Date of the Last Update:  Aug 11, 2006
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

/// \ingroup qs
/// \file qs.cpp
/// \brief QS internal variables definitions and core QS functions
/// implementations.

//............................................................................
uint8_t QS::glbFilter_[32];                                // global QS filter

//............................................................................
uint8_t *QS_ring_;                  // pointer to the start of the ring buffer
QSCtr QS_end_;                         // offset of the end of the ring buffer
QSCtr QS_head_;                  // offset to where next byte will be inserted
QSCtr QS_tail_;                 // offset of where next byte will be extracted
QSCtr QS_used_;                // number of bytes currently in the ring buffer
uint8_t QS_seq_;                                 // the record sequence number
uint8_t QS_chksum_;                      // the checksum of the current record
uint8_t QS_full_;                       // the ring buffer is temporarily full

//............................................................................
char const Q_ROM *QS::getVersion(void) {
    static char const Q_ROM version[] = "3.2.00";
    return version;
}
//............................................................................
void QS::initBuf(uint8_t sto[], uint32_t stoSize) {
    QS_ring_ = &sto[0];
    QS_end_  = (QSCtr)stoSize;
}
//............................................................................
void QS::filterOn(uint8_t rec) {
    if (rec == QS_ALL_RECORDS) {
        uint8_t i;
        for (i = (uint8_t)0; i < (uint8_t)sizeof(glbFilter_); ++i) {
            glbFilter_[i] = (uint8_t)0xFF;
        }
    }
    else {
        glbFilter_[rec >> 3] |= (uint8_t)(1U << (rec & 0x07));
    }
}
//............................................................................
void QS::filterOff(uint8_t rec) {
    if (rec == QS_ALL_RECORDS) {
        uint8_t i;
        for (i = (uint8_t)0; i < (uint8_t)sizeof(glbFilter_); ++i) {
            glbFilter_[i] = (uint8_t)0;
        }
    }
    else {
        glbFilter_[rec >> 3] &= (uint8_t)(~(1U << (rec & 0x07)));
    }
}
//............................................................................
void QS::begin(uint8_t rec) {
    QS_chksum_ = (uint8_t)0;                             // clear the checksum
    ++QS_seq_;                         // always increment the sequence number
    QS_INSERT_ESC_BYTE(QS_seq_)                   // store the sequence number
    QS_INSERT_ESC_BYTE(rec)                             // store the record ID
}
//............................................................................
void QS::end(void) {
    QS_INSERT_CHKSUM_BYTE()
    QS_INSERT_BYTE(QS_FRAME)
    if (QS_used_ > QS_end_) {                    // overrun over the old data?
        QS_tail_ = QS_head_;                 // shift the tail to the old data
        QS_used_ = QS_end_;                        // the whole buffer is used
    }
}
//............................................................................
void QS::u8(uint8_t format, uint8_t data) {
    QS_INSERT_ESC_BYTE(format);
    QS_INSERT_ESC_BYTE(data);
}
//............................................................................
void QS::u16(uint8_t format, uint16_t data) {
    QS_INSERT_ESC_BYTE(format);
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
}
//............................................................................
void QS::u32(uint8_t format, uint32_t data) {
    QS_INSERT_ESC_BYTE(format);
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
    data >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)data)
}