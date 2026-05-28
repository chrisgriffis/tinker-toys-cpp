//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 3.1.04
// Date of the Last Update:  Dec 29, 2005
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
#ifndef qpset_h
#define qpset_h

/// \ingroup qf qk
/// \file qpset.h
/// \brief platform-independent priority sets of 8 or 64 elements.
///
/// This header file must be included in those QF ports that use the
/// cooperative multitasking QF scheduler or the QK.

//////////////////////////////////////////////////////////////////////////////
/// \brief Priority Set useful for building various schedulers, but also
/// useful as a general set of up to 8 elements of any kind
///
/// The priority set represents the set of active objects that are ready to
/// run and need to be considered by scheduling processing. The set is capable
/// of storing up to 8 priority levels.
class QPSet8 {
    //////////////////////////////////////////////////////////////////////////
    /// \brief bimask representing elements of the set
    uint8_t bits_;

public:

    /// \brief the macro evaluates to TRUE if the priority set is empty,
    /// which means that no active objects are ready to run.
    uint8_t isEmpty(void) {
        return (uint8_t)(bits_ == (uint8_t)0);
    }

    /// \brief the macro evaluates to TRUE if the priority set has elements,
    /// which means that some active objects are ready to run.
    uint8_t hasElements(void) {
        return (uint8_t)(bits_ != (uint8_t)0);
    }

    /// \brief the macro evaluates to TRUE if the priority set has the
    /// element \a n.
    uint8_t hasElement(uint8_t n) {
        return (uint8_t)((bits_ & QF::pwr2Lkup[n]) != 0);
    }

    /// \brief insert element n_ into the set, n_= 1..8
    void insert(uint8_t n) {
        bits_ |= QF::pwr2Lkup[n];
    }

    /// \brief remove element n_ from the set, n_= 1..8
    void remove(uint8_t n) {
        bits_ &= QF::invPwr2Lkup[n];
    }

    /// \brief find the maximum element in the set,
    /// \note the set cannot be empty
    uint8_t findMax(void) {
        return QF::log2Lkup[bits_];
    }
};

//////////////////////////////////////////////////////////////////////////////
/// \brief Priority Set useful for building various schedulers, but also
/// useful as a general set of up to 64 elements of any kind
///
/// The priority set represents the set of active objects that are ready to
/// run and need to be considered by scheduling processing. The set is capable
/// of storing up to 64 priority levels.
///
/// The priority set allows to build cooperative multitasking schedulers
/// to manage up to 64 tasks. It is also used in the Quantum Kernel (QK)
/// preemptive scheduler.
class QPSet {

    /// \brief condensed representation of the priority set
    ///
    /// Each bit in the bytes__ attribute represents a byte (8-bits)
    /// in the bits_[] array. More specifically: \n
    /// bit 0 in bytes_ is 1 when any bit in bits_[0] is 1 \n
    /// bit 1 in bytes_ is 1 when any bit in bits_[1] is 1 \n
    /// bit 2 in bytes_ is 1 when any bit in bits_[2] is 1 \n
    /// bit 3 in bytes_ is 1 when any bit in bits_[3] is 1 \n
    /// bit 4 in bytes_ is 1 when any bit in bits_[4] is 1 \n
    /// bit 5 in bytes_ is 1 when any bit in bits_[5] is 1 \n
    /// bit 6 in bytes_ is 1 when any bit in bits_[6] is 1 \n
    /// bit 7 in bytes_ is 1 when any bit in bits_[7] is 1 \n
    uint8_t bytes_;

    /// \brief Bitmasks representing elements in the set. Specifically: \n
    /// bits_[0] represent elements 1..8   \n
    /// bits_[1] represent elements 9..16  \n
    /// bits_[2] represent elements 17..24 \n
    /// bits_[3] represent elements 25..32 \n
    /// bits_[4] represent elements 33..40 \n
    /// bits_[5] represent elements 41..48 \n
    /// bits_[6] represent elements 49..56 \n
    /// bits_[7] represent elements 57..64 \n
    uint8_t bits_[8];

public:

    /// \brief the macro evaluates to TRUE if the priority set is empty,
    /// which means that no active objects are ready to run.
    uint8_t isEmpty(void) {
        return (uint8_t)(bytes_ == (uint8_t)0);
    }

    /// \brief the macro evaluates to TRUE if the priority set has elements,
    /// which means that some active objects are ready to run.
    uint8_t hasElements(void) {
        return (uint8_t)(bytes_ != (uint8_t)0);
    }

    /// \brief the macro evaluates to TRUE if the priority set has the
    /// element \a n.
    uint8_t hasElement(uint8_t n) {
        return (uint8_t)((bits_[QF::div8Lkup[n]]
                          & QF::pwr2Lkup[QF::div8Lkup[n] + 1]) != 0);
    }

    /// \brief insert element n_ into the set, n_= 1..64
    void insert(uint8_t n) {
        bits_[QF::div8Lkup[n]] |= QF::pwr2Lkup[n];
        bytes_ |= QF::pwr2Lkup[QF::div8Lkup[n] + 1];
    }

    /// \brief remove element n_ from the set, n_= 1..64
    void remove(uint8_t n) {
        bits_[QF::div8Lkup[n]] &= QF::invPwr2Lkup[n];
        if (bits_[QF::div8Lkup[n]] == (uint8_t)0) {
            bytes_ &= QF::invPwr2Lkup[QF::div8Lkup[n] + 1];
        }
    }

    /// \brief find the maximum element in the set,
    /// \note the set cannot be empty
    uint8_t findMax(void) {
        uint8_t n = (uint8_t)(QF::log2Lkup[bytes_] - 1);
        return (uint8_t)((n << 3) + QF::log2Lkup[bits_[n]]);
    }
};

#endif                                                              // qpset_h

