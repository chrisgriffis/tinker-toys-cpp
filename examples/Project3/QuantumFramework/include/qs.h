//////////////////////////////////////////////////////////////////////////////
// Product: QS/C++ platform-independent public interface.
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
#ifndef qs_h
#define qs_h

/// \ingroup qep qf qk qs
/// \file qs.h
/// \brief QS/C++ platform-independent public interface.
/// This header file must be included directly or indirectly
/// in all modules (*.cpp files) that use QS/C++.

#ifdef Q_SPY

#include <stdint.h>                           // C99 exact-width integer types
#include <tchar.h>

#ifndef Q_ROM

/// \brief Macro to force allocating objects in ROM
///
/// The Q_ROM macro is used inside the QP source code to denote these constant
/// objects that can be allocated in ROM. On CPUs with the Harvard
/// architecture (e.g., the 8051 or AVR), the code and data spaces are
/// separate and are accessed through different CPU instructions. The
/// compilers often provide specific extended keywords to designate code or
/// data space, such as the __code extended keyword in the IAR 8051 compiler.
/// To use QP with a different MCU/compiler, you need to check how to allocate
/// constants to ROM. If you don not define the Q_ROM macro, it will default
/// to noting.

#define Q_ROM

#endif

//////////////////////////////////////////////////////////////////////////////

/// \brief Quantum Spy record types.
///
/// This enumeration specifies the record types used in the QP components.
/// You can specify your own record types starting from ::QS_USER offset.
/// Currently, the maximum of all records cannot exceed 256.
/// \sa QS::filterOn()/#QS_FILTER_ON and QS::filterOff()/#QS_FILTER_OFF
enum QSpyRecords {
    // QEP records
    QS_QEP_STATE_EMPTY,                                          // see NOTE01
    QS_QEP_STATE_ENTRY,                               ///< a state was entered
    QS_QEP_STATE_EXIT,                                 ///< a state was exited
    QS_QEP_STATE_INIT,          ///< an intial transition was taken in a state
    QS_QEP_INIT_TRAN,           ///< the top-most initial transition was taken
    QS_QEP_INTERN_TRAN,                  ///< an internal transition was taken
    QS_QEP_TRAN,                           ///< a regular transition was taken
    QS_QEP_IGNORED,             ///< an event was ignored (silently discarded)
    QS_QEP_RESERVED1,
    QS_QEP_RESERVED0,

    // QF records
    QS_QF_ACTIVE_ADD,                ///< an AO has been added to QF (started)
    QS_QF_ACTIVE_REMOVE,         ///< an AO has been removed from QF (stopped)
    QS_QF_ACTIVE_SUBSCRIBE,                  ///< an AO subscribed to an event
    QS_QF_ACTIVE_UNSUBSCRIBE,              ///< an AO unsubscribed to an event
    QS_QF_ACTIVE_POST_FIFO,  ///< an event was posted (FIFO) directly to an AO
    QS_QF_ACTIVE_POST_LIFO,  ///< an event was posted (LIFO) directly to an AO
    QS_QF_ACTIVE_GET, ///< an AO got an event and its queue is still not empty
    QS_QF_ACTIVE_GET_LAST,      ///< an AO got an event and its queue is empty
    QS_QF_EQUEUE_INIT,                     ///< an event queue was initialized
    QS_QF_EQUEUE_POST_FIFO,     ///< an event was posted (FIFO) to a raw queue
    QS_QF_EQUEUE_POST_LIFO,     ///< an event was posted (LIFO) to a raw queue
    QS_QF_EQUEUE_GET,              ///< get an event and queue still not empty
    QS_QF_EQUEUE_GET_LAST,              ///< get the last event from the queue
    QS_QF_MPOOL_INIT,                       ///< a memory pool was initialized
    QS_QF_MPOOL_GET,        ///< a memory block was removed from a memory pool
    QS_QF_MPOOL_PUT,         ///< a memory block was returned to a memory pool
    QS_QF_PUBLISH_ATTEMPT,            ///< an unsubscribed event was published
    QS_QF_PUBLISH,       ///< an event was truly published to some subscribers
    QS_QF_NEW,                                         ///< new event creation
    QS_QF_GC_ATTEMPT,                          ///< garbage collection attempt
    QS_QF_GC,                                          ///< garbage collection
    QS_QF_TICK,                                     ///< QF::tick() was called
    QS_QF_TIMEEVT_ARM,                             ///< a time event was armed
    QS_QF_TIMEEVT_AUTO_DISARM,      ///< a time event expired and was disarmed
    QS_QF_TIMEEVT_DISARM_ATTEMPT,///< an attempt to disarmed a disarmed tevent
    QS_QF_TIMEEVT_DISARM,           ///< true disarming of an armed time event
    QS_QF_TIMEEVT_REARM,                         ///< rearming of a time event
    QS_QF_TIMEEVT_POST,      ///< a time event posted itself directly to an AO
    QS_QF_TIMEEVT_PUBLISH,                  ///< a time event published itself
    QS_QF_INT_LOCK,                                ///< interrupts were locked
    QS_QF_INT_UNLOCK,                            ///< interrupts were unlocked
    QS_QF_ISR_ENTRY,                                   ///< an ISR was entered
    QS_QF_ISR_EXIT,                                     ///< an ISR was exited
    QS_QF_RESERVED6,
    QS_QF_RESERVED5,
    QS_QF_RESERVED4,
    QS_QF_RESERVED3,
    QS_QF_RESERVED2,
    QS_QF_RESERVED1,
    QS_QF_RESERVED0,

    // QK records
    QS_QK_MUTEX_LOCK,                             ///< the QK mutex was locked
    QS_QK_MUTEX_UNLOCK,                         ///< the QK mutex was unlocked
    QS_QK_SCHEDULE,      ///< the QK scheduler scheduled a new task to execute
    QS_QK_RESERVED6,
    QS_QK_RESERVED5,
    QS_QK_RESERVED4,
    QS_QK_RESERVED3,
    QS_QK_RESERVED2,
    QS_QK_RESERVED1,
    QS_QK_RESERVED0,

    // Miscallaneous QS records
    QS_SIG_DICTIONARY,                            ///< signal dictionary entry
    QS_OBJ_DICTIONARY,                            ///< object dictionary entry
    QS_FUN_DICTIONARY,                          ///< function dictionary entry
    QS_STR_DICTIONARY,                            ///< string dictionary entry
    QS_ASSERT,
    QS_RESERVED5,
    QS_RESERVED4,
    QS_RESERVED3,
    QS_RESERVED2,
    QS_RESERVED1,
    QS_RESERVED0,

    // User records
    QS_USER                ///< the first record available for user QS records
};

/// \brief Specification of all QS records for the QS::filterOn() and
/// QS::filterOff()
#define QS_ALL_RECORDS          ((uint8_t)0xFF)

/// \brief Constant representing End-Of-Data condition returned from the
/// QS::getByte() function.
#define QS_EOD                  ((uint16_t)0xFFFF)


#ifndef QS_TIME_SIZE

    /// \brief The size (in bytes) of the QS time stamp. Valid values: 1, 2,
    /// or 4; default 4.
    ///
    /// This macro can be defined in the QS port file (qs_port.h) to
    /// configure the ::QSTimeCtr type. Here the macro is not defined so the
    /// default of 4 byte is chosen.
    #define QS_TIME_SIZE 4
#endif
#if (QS_TIME_SIZE == 1)
    typedef uint8_t QSTimeCtr;
    #define QS_TIME_()   QS::u8_(QS::getTime())
#elif (QS_TIME_SIZE == 2)
    typedef uint16_t QSTimeCtr;
    #define QS_TIME_()   QS::u16_(QS::getTime())
#elif (QS_TIME_SIZE == 4)

    /// \brief The type of the QS time stamp
    ///
    /// This type determines the dynamic range of QS time stamps
    typedef uint32_t QSTimeCtr;

    /// \brief Internal macro to output time stamp to the QS record
    #define QS_TIME_()   QS::u32_(QS::getTime())
#else
    #error "QS_TIME_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/// \brief Quantum Spy logging facilities
///
/// This class groups together QS services. It has only static members and
/// should not be instantiated.
class QF_API QS {
public:

    /// \brief Get the current version of QS
    ///
    /// \return version of the QS as a constant 6-character string of the form
    /// x.y.zz, where x is a 1-digit major version number, y is a 1-digit
    /// minor version number, and zz is a 2-digit release number.
    static char const Q_ROM *getVersion(void);

    /// \brief Initialize the QS data buffer.
    ///
    /// This function should be called from QS_init() to provide QS with the
    /// data buffer. The first argument \a sto[] is the address of the memory
    /// block, and the second argument \a stoSize is the size of this block
    /// in bytes. Currently the size of the QS buffer cannot exceed 64KB.
    ///
    /// QS can work with quite small data buffers, but you will start losing
    /// data if the buffer is too small for the bursts of logging activity.
    /// The right size of the buffer depends on the data production rate and
    /// the data output rate. QS offers flexible filtering to reduce the data
    /// production rate.
    ///
    /// \note If the data output rate cannot keep up with the production rate,
    /// QS will start overwriting the older data with newer data. This is
    /// consistent with the "last-is-best" QS policy. The record sequence
    ///  counters and checksums on each record allow to easily detect data
    /// loss.
    static void initBuf(uint8_t sto[], uint32_t stoSize);

    /// \brief Turn the global Filter on for a given record type \a rec.
    ///
    /// This function sets up the QS filter to enable the record type \a rec.
    /// The argument #QS_ALL_RECORDS specifies to filter-on all records.
    /// This function should be called indirectly through the macro
    /// #QS_FILTER_ON.
    ///
    /// \note Filtering based on the record-type is only the first layer of
    /// filtering. The second layer is based on the object-type. Both filter
    /// layers must be enabled for the QS record to be inserted into the QS
    /// buffer.
    /// \sa QS_filterOff(), #QS_FILTER_SM_OBJ, #QS_FILTER_AO_OBJ,
    /// #QS_FILTER_MP_OBJ, #QS_FILTER_EQ_OBJ, and #QS_FILTER_TE_OBJ.
    static void filterOn(uint8_t rec);

    /// \brief Turn the global Filter off for a given record type \a rec.
    ///
    /// This function sets up the QS filter to disable the record type \a rec.
    /// The argument #QS_ALL_RECORDS specifies to suppress all records.
    /// This function should be called indirectly through the macro
    /// #QS_FILTER_OFF.
    ///
    /// \note Filtering records based on the record-type is only the first
    /// layer of filtering. The second layer is based on the object-type.
    /// Both filter layers must be enabled for the QS record to be inserted
    /// into the QS buffer.
    /// \sa
    static void filterOff(uint8_t rec);

    /// \brief Mark the begin of a QS record \a rec
    ///
    /// This function must be called at the beginning of each QS record.
    /// This function should be called indirectly through the macro #QS_BEGIN,
    /// or #QS_BEGIN_NOLOCK, depending if it's called in a normal code or from
    /// a critical section.
    static void begin(uint8_t rec);

    /// \brief Mark the end of a QS record \a rec
    ///
    /// This function must be called at the end of each QS record.
    /// This function should be called indirectly through the macro #QS_END,
    /// or #QS_END_NOLOCK, depending if it's called in a normal code or from
    /// a critical section.
    static void end(void);

    // unformatted data elements output ......................................

    /// \brief output uint8_t data element without format information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void u8_(uint8_t data);

    /// \brief Output uint16_t data element without format information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void u16_(uint16_t data);

    /// \brief Output uint32_t data element without format information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void u32_(uint32_t data);

    /// \brief Output zero-terminated ASCII string element without format
    /// information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void str_(char const *s);

    // formatted data elements output ........................................

    /// \brief Output uint8_t data element with format information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void u8(uint8_t format, uint8_t data);

    /// \brief output uint16_t data element with format information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void u16(uint8_t format, uint16_t data);

    /// \brief Output uint32_t data element with format information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void u32(uint8_t format, uint32_t data);

    /// \brief Output 32-bit floating point data element with format
    /// information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void f32(uint8_t format, float data);

    /// \brief Output 64-bit floating point data element with format
    /// information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void f64(uint8_t format, double data);

    /// \brief Output zero-terminated ASCII string element with format
    /// information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void str(TCHAR const* s);
#ifdef UNICODE
    static void str(char const* s);
#endif

    /// \brief Output memory block of up to 255-bytes with format information
    /// \note This function is only to be used through macros, never in the
    /// client code directly.
    static void mem(uint8_t const *blk, uint8_t size);

    static void hash_str(uint32_t hash);

    // QS buffer access ......................................................

    /// \brief Byte-oriented interface to the QS data buffer.
    ///
    /// This function delivers one byte at a time from the QS data buffer.
    /// The function returns the byte in the least-significant 8-bits of the
    /// 16-bit return value if the byte is available. If no more data is
    /// available at the time, the function returns QS_EOD (End-Of-Data).
    ///
    /// \note QS::getByte() is NOT protected with a critical section.
    static uint16_t getByte(void);

    /// \brief Block-oriented interface to the QS data buffer.
    ///
    /// This function delivers a contiguous block of data from the QS data
    /// buffer. The function returns the pointer to the beginning of the
    /// block, and writes the number of bytes in the block to the location
    /// pointed to by \a pNbytes. The argument \a pNbytes is also used as
    /// input to provide the maximum size of the data block that the caller
    /// can accept.
    ///
    /// If no bytes are available in the QS buffer when the function is
    /// called, the function returns a NULL pointer and sets the value
    /// pointed to by \a pNbytes to zero.
    ///
    /// \note Only the NULL return from QS::getBlock() indicates that the QS
    /// buffer is empty at the time of the call. The non-NULL return often
    /// means that the block is at the end of the buffer and you need to call
    /// QS::getBlock() again to obtain the rest of the data that "wrapped
    /// around" to the beginning of the QS data buffer.
    ///
    /// \note QS::getBlock() is NOT protected with a critical section.
    static uint8_t const *getBlock(uint16_t *pNbytes);

// platform-dependent callback functions, need to be implemented by clients
public:

    /// \brief Initialize the QS facility
    ///
    /// This is a platform-dependent "callback" function invoked through the
    /// macro #QS_INIT. You need to implement this function in your
    /// application. At a minimum, the function must configure the QS buffer
    /// by calling QS::initBuf(). Typically, you will also want to open/
    /// configure the QS output channel, such as a serial port, or a data
    /// file. The void* argument \a arg can be used to pass parameter(s)
    /// needed to configure the output channel.
    ///
    /// The function returns TRUE (1) if the QS initialization was successful,
    /// or FALSE (0) if it failed.
    ///
    /// The following example illustrates an implementation of QS::init():
    /// \include qs_init.cpp
    static uint8_t init(void const *arg);

    /// \brief Cleanup the QS facility
    ///
    /// This is a platform-dependent "callback" function invoked through the
    /// macro #QS_EXIT. You need to implement this function in your
    /// application. The main purpose of this function is to close the QS
    /// output channel, if necessary.
    static void exit(void);

    /// \brief Flush the QS trace data to the host
    ///
    /// This is a platform-dependent "callback" function to flush the QS
    /// trace buffer to the host. The function typically busy-waits until all
    /// the data in the buffer is sent to the host. This is acceptable only
    /// in the initial transient.
    static void flush(void);

    /// \brief Callback to obtain a timestamp for a QS record.
    ///
    /// This is a platform-dependent "callback" function invoked from the
    /// macro #QS_TIME_ to add the time stamp to the QS record.
    ///
    /// \note Some of the pre-defined QS records from QP do not output the
    /// time stamp. However, ALL user records do output the time stamp.
    /// \note QS_getTime() is called in a critical section and should not
    /// unlock interrupts.
    ///
    /// The following example shows using a system call to implement QS
    /// time stamping:
    /// \include qs_getTime.cpp
    static QSTimeCtr getTime(void);

// Global and Local QS filters ...............................................
public:
    static uint8_t glbFilter_[32];                ///< global on/off QS filter
    static void const *smObj_;         ///< state machine for QEP local filter
    static void const *aoObj_;       ///< active object for QF/QK local filter
    static void const *mpObj_;            ///<  event pool for QF local filter
    static void const *eqObj_;             ///<  raw queue for QF local filter
    static void const *teObj_;            ///<  time event for QF local filter
    static void const *apObj_;///<  generic object Application QF local filter
};


//////////////////////////////////////////////////////////////////////////////
// Macros for adding QS instrumentation to the client code

/// \brief Initialize the QS facility.
///
/// This macro provides an indirection layer to invoke the QS initialization
/// routine if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
/// \sa QS::init(), example of setting up a QS filter in #QS_FILTER_IN
#define QS_INIT(arg_)           QS::init(arg_)

/// \brief Cleanup the QS facility.
///
/// This macro provides an indirection layer to invoke the QS cleanup
/// routine if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
/// \sa QS::exit()
#define QS_EXIT()               QS::exit()

/// \brief Global Filter ON for a given record type \a rec.
///
/// This macro provides an indirection layer to call QS::filterOn() if #Q_SPY
/// is defined, or do nothing if #Q_SPY is not defined.
///
/// The following example shows how to use QS filters:
/// \include qs_filter.cpp
#define QS_FILTER_ON(rec_)      QS::filterOn(rec_)

/// \brief Global filter OFF for a given record type \a rec.
///
/// This macro provides an indirection layer to call QS::filterOff() if #Q_SPY
/// is defined, or do nothing if #Q_SPY is not defined.
///
/// \sa Example of using QS filters in #QS_FILTER_ON documentation
#define QS_FILTER_OFF(rec_)     QS::filterOff(rec_)

/// \brief Local Filter for a given state machine object \a obj_.
///
/// This macro sets up the state machine object local filter if #Q_SPY is
/// defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
/// is the pointer to the state machine object that you want to monitor.
///
/// The state machine object filter allows you to filter QS records pertaining
/// only to a given state machine object. With this filter disabled, QS will
/// output records from all state machines in your application. The object
/// filter is disabled by setting the state machine pointer to NULL.
///
/// The state machine filter affects the following QS records:
/// ::QS_QEP_STATE_ENTRY, ::QS_QEP_STATE_EXIT, ::QS_QEP_STATE_INIT,
/// ::QS_QEP_INIT_TRAN, ::QS_QEP_INTERN_TRAN, ::QS_QEP_TRAN,
/// and ::QS_QEP_IGNORED.
///
/// \note Because active objects are state machines at the same time,
/// the state machine filter (#QS_FILTER_SM_OBJ) pertains to active
/// objects as well. However, the state machine filter is more general,
/// because it can be used only for state machines that are not active
/// objects, such as "Orthogonal Components".
///
/// \sa Example of using QS filters in #QS_FILTER_ON documentation
#define QS_FILTER_SM_OBJ(obj_)  (QS::smObj_ = (obj_))

/// \brief Local Filter for a given active object \a obj_.
///
/// This macro sets up the active object local filter if #Q_SPY is defined,
/// or does nothing if #Q_SPY is not defined. The argument \a obj_ is the
/// pointer to the active object that you want to monitor.
///
/// The active object filter allows you to filter QS records pertaining
/// only to a given active object. With this filter disabled, QS will
/// output records from all active objects in your application. The object
/// filter is disabled by setting the active object pointer \a obj_ to NULL.
///
/// The active object filter affects the following QS records:
/// ::QS_QF_ACTIVE_ADD, ::QS_QF_ACTIVE_REMOVE, ::QS_QF_ACTIVE_SUBSCRIBE,
/// ::QS_QF_ACTIVE_UNSUBSCRIBE, ::QS_QF_ACTIVE_POST_FIFO,
/// ::QS_QF_ACTIVE_POST_LIFO, ::QS_QF_ACTIVE_GET, and ::QS_QF_ACTIVE_GET_LAST.
///
/// \sa Example of using QS filters in #QS_FILTER_ON documentation
#define QS_FILTER_AO_OBJ(obj_)  (QS::aoObj_ = (obj_))

/// \brief Local Filter for a given memory pool object \a obj_.
///
/// This macro sets up the memory pool object local filter if #Q_SPY is
/// defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
/// is the pointer to the memory buffer used during the initialization of the
/// event pool with QF::poolInit().
///
/// The memory pool filter allows you to filter QS records pertaining
/// only to a given memory pool. With this filter disabled, QS will
/// output records from all memory pools in your application. The object
/// filter is disabled by setting the memory pool pointer \a obj_ to NULL.
///
/// The memory pool filter affects the following QS records:
/// ::QS_QF_MPOOL_INIT, ::QS_QF_MPOOL_GET, and ::QS_QF_MPOOL_PUT.
///
/// \sa Example of using QS filters in #QS_FILTER_ON documentation
#define QS_FILTER_MP_OBJ(obj_)  (QS::mpObj_ = (obj_))

/// \brief Filter for a given event queue object \a obj_.
///
/// This macro sets up the event queue object filter if #Q_SPY is defined,
/// or does nothing if #Q_SPY is not defined. The argument \a obj_ is the
/// pointer to the "raw" thread-safe queue object you want to monitor.
///
/// The event queue filter allows you to filter QS records pertaining
/// only to a given event queue. With this filter disabled, QS will
/// output records from all event queues in your application. The object
/// filter is disabled by setting the event queue pointer \a obj_ to NULL.
///
/// The event queue filter affects the following QS records:
/// ::QS_QF_EQUEUE_INIT, ::QS_QF_EQUEUE_POST_FIFO, ::QS_QF_EQUEUE_POST_LIFO,
/// ::QS_QF_EQUEUE_GET, and ::QS_QF_EQUEUE_GET_LAST.
///
/// \sa Example of using QS filters in #QS_FILTER_IN documentation
#define QS_FILTER_EQ_OBJ(obj_)  (QS::eqObj_ = (obj_))

/// \brief Local Filter for a given time event object \a obj_.
///
/// This macro sets up the time event object local filter if #Q_SPY is
/// defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
/// is the pointer to the time event object you want to monitor.
///
/// The time event filter allows you to filter QS records pertaining
/// only to a given time event. With this filter disabled, QS will
/// output records from all time events in your application. The object
/// filter is disabled by setting the time event pointer \a obj_ to NULL.
///
/// The time event filter affects the following QS records:
/// ::QS_QF_TIMEEVT_ARM, ::QS_QF_TIMEEVT_AUTO_DISARM,
/// ::QS_QF_TIMEEVT_DISARM_ATTEMPT, ::QS_QF_TIMEEVT_DISARM,
/// ::QS_QF_TIMEEVT_REARM, ::QS_QF_TIMEEVT_POST, and ::QS_QF_TIMEEVT_PUBLISH.
///
/// \sa Example of using QS filters in #QS_FILTER_ON documentation
#define QS_FILTER_TE_OBJ(obj_)  (QS::teObj_ = (obj_))

/// \brief Local Filter for a generic application object \a obj_.
///
/// This macro sets up the local application object filter if #Q_SPY is
/// defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
/// is the pointer to the application object you want to monitor.
///
/// The application object filter allows you to filter QS records pertaining
/// only to a given application object. With this filter disabled, QS will
/// output records from all application-records enabled by the global filter.
/// The local filter is disabled by setting the time event pointer \a obj_
/// to NULL.
///
/// \sa Example of using QS filters in #QS_FILTER_ON documentation
#define QS_FILTER_AP_OBJ(obj_)  (QS_apObj_ = (obj_))


//////////////////////////////////////////////////////////////////////////////
// Macros to generate user QS records

/// \brief Begin a QS user record without locking interrupts.
#define QS_BEGIN_NOLOCK(rec_, obj_) \
    if (((QS::glbFilter_[(uint8_t)(rec_) >> 3U] \
          & (1U << ((uint8_t)(rec_) & 7U))) != 0) \
        && ((QS::apObj_ == (void *)0) || (QS::apObj_ == (obj_)))) \
    { \
        QS::begin((uint8_t)(rec_)); \
        QS_TIME_()

/// \brief End a QS user record without locking interrupts.
#define QS_END_NOLOCK() \
    QS_END_NOLOCK_()

                                    // QS-specific interrupt locking/unlocking
#ifndef QS_INT_KEY_TYPE
    /// \brief This is an internal macro for defining the interrupt lock key.
    ///
    /// The purpose of this macro is to enable writing the same code for the
    /// case when interrupt key is defined and when it is not. If the macro
    /// #QS_INT_KEY_TYPE is defined, this internal macro provides the
    /// definition of the lock key variable. Otherwise this macro is empty.
    /// \sa #QS_INT_KEY_TYPE, #QF_INT_KEY_TYPE, #QK_INT_KEY_TYPE
    #define QS_INT_LOCK_KEY_

    /// \brief This is an internal macro for locking interrupts.
    ///
    /// The purpose of this macro is to enable writing the same code for the
    /// case when interrupt key is defined and when it is not. If the macro
    /// #QS_INT_KEY_TYPE is defined, this internal macro invokes #QS_INT_LOCK
    /// passing the key variable as the parameter. Otherwise #QS_INT_LOCK
    /// is invoked with a dummy parameter.
    /// \sa #QS_INT_LOCK, #QF_INT_LOCK, #QK_INT_LOCK
    #define QS_INT_LOCK_()      QS_INT_LOCK(ignore_)

    /// \brief This is an internal macro for unlocking interrupts.
    ///
    /// The purpose of this macro is to enable writing the same code for the
    /// case when interrupt key is defined and when it is not. If the macro
    /// #QS_INT_KEY_TYPE is defined, this internal macro invokes
    /// #QS_INT_UNLOCK passing the key variable as the parameter. Otherwise
    /// #QS_INT_UNLOCK is invoked with a dummy parameter.
    /// \sa #QS_INT_UNLOCK, #QF_INT_UNLOCK, #QK_INT_UNLOCK
    #define QS_INT_UNLOCK_()    QS_INT_UNLOCK(ignore_)
#else
    #define QS_INT_LOCK_KEY_    QS_INT_KEY_TYPE intLockKey__;
    #define QS_INT_LOCK_()      QS_INT_LOCK(intLockKey__)
    #define QS_INT_UNLOCK_()    QS_INT_UNLOCK(intLockKey__)
#endif

/// \brief Begin a user QS record with locking interrupts.
///
/// The following example shows how to build a user QS record using the
/// macros #QS_BEGIN, #QS_END, and the formatted output macros: #QS_U8 and
/// #QS_STR.
/// \include qs_user.cpp
/// \note Must always be used in pair with #QS_END
#define QS_BEGIN_GUARD_(rec_, obj_) \
      ((QS::glbFilter_[(uint8_t)(rec_) >> 3U] \
          & (1U << ((uint8_t)(rec_) & 7U))) != 0) \
        && ((QS::apObj_ == (void *)0) || (QS::apObj_ == (obj_))) \

#define QS_BEGIN_BODY_(rec_, obj_)\
        QS_INT_LOCK_KEY_ \
        QS_INT_LOCK_(); \
        QS::begin((uint8_t)(rec_)); \
        QS_TIME_()

#define QS_BEGIN(rec_, obj_) \
    if(QS_BEGIN_GUARD_(rec_, obj_))\
    {\
        QS_BEGIN_BODY_(rec_, obj);\


/// \brief End a QS record with locking interrupts.
/// \sa example for #QS_BEGIN
/// \note Must always be used in pair with #QS_BEGIN
#define QS_END_BODY_()\
    QS::end(); \
    QS_INT_UNLOCK_();\

#define QS_END() \
        QS_END_BODY_();\
    }\

//////////////////////////////////////////////////////////////////////////////
// Macros for use inside other macros or internally in the QP code

/// \brief Internal QS macro to begin a QS record with locking the interrupts.
/// \note This macro is intended to use only inside QP components and NOT
/// at the application level. \sa #QS_BEGIN
#define QS_BEGIN_(rec_, objFilter_, obj_) \
    if (((QS::glbFilter_[(uint8_t)(rec_) >> 3U] \
          & (1U << ((uint8_t)(rec_) & 7U))) != 0) \
        && (((objFilter_) == (void *)0) || ((objFilter_) == (obj_)))) \
    { \
        QS_INT_LOCK_KEY_ \
        QS_INT_LOCK_(); \
        QS::begin((uint8_t)(rec_))

/// \brief  Internal QS macro to end a QS record with locking the interrupts.
/// \note This macro is intended to use only inside QP components and NOT
/// at the application level. \sa #QS_END
#define QS_END_() \
        QS::end(); \
        QS_INT_UNLOCK_(); \
    } else ((void)0)

/// \brief Internal QS macro to begin a QS record without locking the
/// interrupts.
/// \note This macro is intended to use only inside QP components and NOT
/// at the application level. \sa #QS_BEGIN_NOLOCK
#define QS_BEGIN_NOLOCK_(rec_, objFilter_, obj_) \
    if (((QS::glbFilter_[(uint8_t)(rec_) >> 3U] \
          & (1U << ((uint8_t)(rec_) & 7U))) != 0) \
        && (((objFilter_) == (void *)0) || ((objFilter_) == (obj_)))) \
    { \
        QS::begin((uint8_t)(rec_))

/// \brief Internal QS macro to end a QS record without locking
/// the interrupts.
/// \note This macro is intended to use only inside QP components and NOT
/// at the application level. \sa #QS_END_NOLOCK
#define QS_END_NOLOCK_() \
        QS::end(); \
    } else ((void)0)

/// \brief Internal OS macro to output an unformatted uint8_t data element
#define QS_U8_(data_)           QS::u8_(data_)

/// \brief Internal OS macro to output an unformatted uint16_t data element
#define QS_U16_(data_)          QS::u16_(data_)

/// \brief Internal OS macro to output an unformatted uint32_t data element
#define QS_U32_(data_)          QS::u32_(data_)

/// \brief Internal OS macro to output an unformatted HRESULT data element
#define QS_HRESULT_(data_)      QS::u32_(data_)

#if (QEP_SIGNAL_SIZE == 1)

    /// \brief Internal OS macro to output an unformatted event signal
    /// data element
    /// \note the size of the pointer depends on the macro #QEP_SIGNAL_SIZE.
    #define QS_SIG_(sig_)       QS::u8_(sig_)
#elif (QEP_SIGNAL_SIZE == 2)
    #define QS_SIG_(sig_)       QS::u16_(sig_)
#elif (QEP_SIGNAL_SIZE == 4)
    #define QS_SIG_(sig_)       QS::u32_(sig_)
#endif


#if (QF_EVENT_SIZ_SIZE == 1)

    /// \brief Internal OS macro to output an unformatted event size
    /// data element
    /// \note the size of the pointer depends on the macro #QF_EVENT_SIZ_SIZE.
    #define QS_EVS_(size_)      QS::u8_(size_)
#elif (QF_EVENT_SIZ_SIZE == 2)
    #define QS_EVS_(size_)      QS::u16_(size_)
#elif (QF_EVENT_SIZ_SIZE == 4)
    #define QS_EVS_(size_)      QS::u32_(size_)
#endif


#if (QF_EQUEUE_CTR_SIZE == 1)

    /// \brief Internal OS macro to output an unformatted event queue
    /// counter data element
    /// \note the size of the pointer depends on the macro #QF_EQUEUE_CTR_SIZE
    #define QS_EQC_(ctr_)       QS::u8_(ctr_)
#elif (QF_EQUEUE_CTR_SIZE == 2)
    #define QS_EQC_(ctr_)       QS::u16_(ctr_)
#elif (QF_EQUEUE_CTR_SIZE == 4)
    #define QS_EQC_(ctr_)       QS::u32_(ctr_)
#endif


#if (QF_MPOOL_SIZ_SIZE == 1)

    /// \brief Internal OS macro to output an unformatted memory pool
    /// block-size data element
    /// \note the size of the pointer depends on the macro #QF_MPOOL_SIZ_SIZE
    #define QS_MPS_(size_)      QS::u8_(size_)
#elif (QF_MPOOL_SIZ_SIZE == 2)
    #define QS_MPS_(size_)      QS::u16_(size_)
#elif (QF_MPOOL_SIZ_SIZE == 4)
    #define QS_MPS_(size_)      QS::u32_(size_)
#endif

#if (QF_MPOOL_CTR_SIZE == 1)

    /// \brief Internal OS macro to output an unformatted memory pool
    /// block-counter data element
    /// \note the size of the pointer depends on the macro #QF_MPOOL_CTR_SIZE
    #define QS_MPC_(ctr_)       QS::u8_(ctr_)
#elif (QF_MPOOL_CTR_SIZE == 2)
    #define QS_MPC_(ctr_)       QS::u16_(ctr_)
#elif (QF_MPOOL_CTR_SIZE == 4)
    #define QS_MPC_(ctr_)       QS::u32_(ctr_)
#endif


#if (QF_TIMEEVT_CTR_SIZE == 1)

    /// \brief Internal OS macro to output an unformatted time event
    /// tick-counter data element
    /// \note the size of the pointer depends on the macro
    /// #QF_TIMEEVT_CTR_SIZE.
    #define QS_TEC_(ctr_)       QS::u8_(ctr_)
#elif (QF_TIMEEVT_CTR_SIZE == 2)
    #define QS_TEC_(ctr_)       QS::u16_(ctr_)
#elif (QF_TIMEEVT_CTR_SIZE == 4)
    #define QS_TEC_(ctr_)       QS::u32_(ctr_)
#endif


#if (QS_OBJ_PTR_SIZE == 1)
    #define QS_OBJ_(obj_)       QS::u8_((uint8_t)(obj_))
#elif (QS_OBJ_PTR_SIZE == 2)
    #define QS_OBJ_(obj_)       QS::u16_((uint16_t)(obj_))
#elif (QS_OBJ_PTR_SIZE == 4)
    #define QS_OBJ_(obj_)       QS::u32_((uint32_t)(obj_))
#else

    /// \brief Internal OS macro to output an unformatted object pointer
    /// data element
    /// \note the size of the pointer depends on the macro #QS_OBJ_PTR_SIZE.
    /// If the size is not defined the size of pointer is assumed 4-bytes.
    #define QS_OBJ_(obj_)       QS::u32_((uint32_t)(obj_))
#endif


#if (QS_FUN_PTR_SIZE == 1)
    #define QS_FUN_(fun_)       QS::u8_((uint8_t)(fun_))
#elif (QS_FUN_PTR_SIZE == 2)
    #define QS_FUN_(fun_)       QS::u16_((uint16_t)(fun_))
#elif (QS_FUN_PTR_SIZE == 4)
    #define QS_FUN_(fun_)       QS::u32_((uint32_t)(fun_))
#else

    /// \brief Internal OS macro to output an unformatted function pointer
    /// data element
    /// \note the size of the pointer depends on the macro #QS_FUN_PTR_SIZE.
    /// If the size is not defined the size of pointer is assumed 4-bytes.
    #define QS_FUN_(fun_)       QS::u32_((uint32_t)(fun_))
#endif

/// \brief Internal OS macro to output a zero-terminated ASCII string
/// data element
#define QS_STR_(msg_)           QS::str_(msg_)

//////////////////////////////////////////////////////////////////////////////
// Macros for use in the client code

/// \brief Enumerates data formats recognized by QS
///
/// QS uses this enumeration is used only internally for the formatted user
/// data elements.
enum QSType {
    QS_I8_T,                                  ///< signed 8-bit integer format
    QS_U8_T,                                ///< unsigned 8-bit integer format
    QS_I16_T,                                ///< signed 16-bit integer format
    QS_U16_T,                              ///< unsigned 16-bit integer format
    QS_I32_T,                                ///< signed 32-bit integer format
    QS_U32_T,                              ///< unsigned 32-bit integer format
    QS_F32_T,                                ///< 32-bit floating point format
    QS_F64_T,                                ///< 64-bit floating point format
    QS_STR_T,                         ///< zero-terminated ASCII string format
    QS_MEM_T,                         ///< up to 255-bytes memory block format
    QS_SIG_T,                                         ///< event signal format
    QS_OBJ_T,                                       ///< object pointer format
    QS_FUN_T,                                     ///< function pointer format
    QS_HRESULT_T,                                          ///< HRESULT format
    QS_HASHED_STR_T,                                 ///< hashed string format
};

/// \brief Output formatted int8_t to the QS record
#define QS_I8(width_, data_) \
    QS::u8((uint8_t)(((width_) << 4)) | QS_I8_T, (data_))

/// \brief Output formatted uint8_t to the QS record
#define QS_U8(width_, data_) \
    QS::u8((uint8_t)(((width_) << 4)) | QS_U8_T, (data_))

/// \brief Output formatted int16_t to the QS record
#define QS_I16(width_, data_) \
    QS::u16((uint8_t)(((width_) << 4)) | QS_I16_T, (data_))

/// \brief Output formatted uint16_t to the QS record
#define QS_U16(width_, data_) \
    QS::u16((uint8_t)(((width_) << 4)) | QS_U16_T, (data_))

/// \brief Output formatted int32_t to the QS record
#define QS_I32(width_, data_) \
    QS::u32((uint8_t)(((width_) << 4)) | QS_I32_T, (data_))

/// \brief Output formatted uint32_t to the QS record
#define QS_U32(width_, data_) \
    QS::u32((uint8_t)(((width_) << 4)) | QS_U32_T, (data_))

/// \brief Output formatted 32-bit floating point number to the QS record
#define QS_F32(width_, data_) \
    QS::f32((uint8_t)(((width_) << 4)) | QS_F32_T, (data_))

/// \brief Output formatted 64-bit floating point number to the QS record
#define QS_F64(width_, data_) \
    QS::f64((uint8_t)(((width_) << 4)) | QS_F64_T, (data_))

/// \brief Output formatted zero-terminated ASCII string to the QS record
#define QS_STR(str_)            QS::str(str_)

/// \brief Output formatted HRESULT to QS record (0 is 'IE_FAIL', 1 is 'Unknown error', 2 is 'IE_FAIL - Unknown error?')
#define QS_HRESULT(kind_, data_) \
    QS::u32((uint8_t)(((kind_) << 4)) | QS_HRESULT_T, (data_))

#define QS_HASHED_STR(hash_) QS::u32(QS_HASHED_STR_T, hash_)
#define QS_HASHED_STR_(hash_) QS::u32_(hash_)
#define QS_HASH_DYNAMIC_STR(str_) QS_HASHED_STR(APHash(str_))
#define QS_HASH_DYNAMIC_STR_(str_) QS_HASHED_STR_(APHash(str_))
#define QS_HASH_STATIC_STR(str_) do { static uint32_t const hash_ = APHash(str_); QS_HASHED_STR(hash_); } while(0)
#define QS_HASH_STATIC_STR_(str_) do { static uint32_t const hash_ = APHash(str_); QS_HASHED_STR_(hash_); } while(0)

/// \brief Output formatted memory block of up to 255 bytes to the QS
/// record
#define QS_MEM(mem_, size_)     QS::mem((mem_), (size_))


#if (QS_OBJ_PTR_SIZE == 1)
    #define QS_OBJ(obj_)        QS::u8(QS_OBJ_T, (uint8_t)(obj_))
#elif (QS_OBJ_PTR_SIZE == 2)
    #define QS_OBJ(obj_)        QS::u16(QS_OBJ_T, (uint16_t)(obj_))
#elif (QS_OBJ_PTR_SIZE == 4)
    #define QS_OBJ(obj_)        QS::u32(QS_OBJ_T, (uint32_t)(obj_))
#else
    /// \brief Output formatted object pointer to the QS record
    #define QS_OBJ(obj_)        QS::u32(QS_OBJ_T, (uint32_t)(obj_))
#endif


#if (QS_FUN_PTR_SIZE == 1)
    #define QS_FUN(fun_)        QS::u8(QS_FUN_T, (uint8_t)(fun_))
#elif (QS_FUN_PTR_SIZE == 2)
    #define QS_FUN(fun_)        QS::u16(QS_FUN_T, (uint16_t)(fun_))
#elif (QS_FUN_PTR_SIZE == 4)
    #define QS_FUN(fun_)        QS::u32(QS_FUN_T, (uint32_t)(fun_))
#else
    /// \brief Output formatted function pointer to the QS record
    #define QS_FUN(fun_)        QS::u32(QS_FUN_T, (uint32_t)(fun_))
#endif


#if (QEP_SIGNAL_SIZE == 1)

    /// \brief Output formatted event signal (of type ::QSignal) and
    /// the state machine object to the user QS record
    #define QS_SIG(sig_, obj_) \
        QS::u8(QS_SIG_T, (sig_)); \
        QS_OBJ_(obj_)
#elif (QEP_SIGNAL_SIZE == 2)
    #define QS_SIG(sig_, obj_) \
        QS::u16(QS_SIG_T, (sig_)); \
        QS_OBJ_(obj_)
#elif (QEP_SIGNAL_SIZE == 4)
    #define QS_SIG(sig_, obj_) \
        QS::u32(QS_SIG_T, (sig_)); \
        QS_OBJ_(obj_)
#endif


/// \brief Output signal dictionary record
///
/// A signal dictionary record associates the numerical value of the signal
/// and the binary address of the state machine that consumes that signal
/// with the human-readable name of the signal.
///
/// Providing a signal dictionary QS record can vastly improve readability of
/// the QS log, because instead of dealing with cryptic machine addresses the
/// QSpy host utility can display human-readable names.
///
/// A signal dictionary entry is associated with both the signal value \a sig_
/// and the state machine \a obj_, because signals are required to be unique
/// only within a given state machine and therefore the same numerical values
/// can represent different signals in different state machines.
///
/// For the "global" signals that have the same meaning in all state machines
/// (such as globally published signals), you can specify a signal dictionary
/// entry with the \a obj_ parameter set to NULL.
///
/// The following example shows the definition of signal dictionary entries
/// in the initial transition of the Table active object. Please note that
/// signals HUNGRY_SIG and DONE_SIG are associated with the Table state
/// machine only ("me" \a obj_ pointer). The EAT_SIG signal, on the other
/// hand, is global (0 \a obj_ pointer):
/// \include qs_sigDic.cpp
///
/// \note The QSpy log utility must capture the signal dictionary record
/// in order to use the human-readable information. You need to connect to
/// the target before the dictionary entries have been transmitted.
///
/// The following QSpy log example shows the signal dictionary records
/// generated from the Table initial transition and subsequent records that
/// show human-readable names of the signals:
/// \include qs_sigLog.txt
///
/// The following QSpy log example shows the same sequence of records, but
/// with dictionary records removed. The human-readable signal names are not
/// available.
/// \include qs_sigLog0.txt
#define QS_SIG_DICTIONARY(sig_, obj_) QS_SIG_DICTIONARY_(sig_, #sig_, obj_)
#define QS_SIG_DICTIONARY_(sig_, sigstr_, obj_) \
    if (((QS::glbFilter_[(uint8_t)QS_SIG_DICTIONARY >> 3U] \
          & (1U << ((uint8_t)QS_SIG_DICTIONARY & 7U))) != 0)) \
    { \
        QS_INT_LOCK_KEY_ \
        QS_INT_LOCK_(); \
        QS::begin((uint8_t)QS_SIG_DICTIONARY); \
        QS_SIG_(sig_); \
        QS_OBJ_(obj_); \
        QS_STR_(sigstr_); \
        QS::end(); \
        QS_INT_UNLOCK_(); \
        QS::flush(); \
    } else ((void)0)

/// \brief Output object dictionary record
///
/// An object dictionary record associates the binary address of an object
/// in the target's memory with the human-readable name of the object.
///
/// Providing an object dictionary QS record can vastly improve readability of
/// the QS log, because instead of dealing with cryptic machine addresses the
/// QSpy host utility can display human-readable object names.
///
/// The following example shows the definition of object dictionary entry
/// for the Table active object:
/// \include qs_objDic.cpp
#define QS_OBJ_DICTIONARY(obj_) QS_OBJ_DICTIONARY_(obj_, #obj_)
#define QS_OBJ_DICTIONARY_(obj_, str_) \
    if (((QS::glbFilter_[(uint8_t)QS_OBJ_DICTIONARY >> 3U] \
          & (1U << ((uint8_t)QS_OBJ_DICTIONARY & 7U))) != 0)) \
    { \
        QS_INT_LOCK_KEY_ \
        QS_INT_LOCK_(); \
        QS::begin((uint8_t)QS_OBJ_DICTIONARY); \
        QS_OBJ_(obj_); \
        QS_STR_(str_); \
        QS::end(); \
        QS_INT_UNLOCK_(); \
        QS::flush(); \
    } else ((void)0)

/// \brief Output string dictionary record
///
/// A string dictionary record associates the hash of a string with the
/// actual string contents.
///
#define QS_STR_DICTIONARY_(hash_, str_) \
    if (((QS::glbFilter_[(uint8_t)QS_STR_DICTIONARY >> 3U] \
          & (1U << ((uint8_t)QS_STR_DICTIONARY & 7U))) != 0)) \
    { \
        QS_INT_LOCK_KEY_ \
        QS_INT_LOCK_(); \
        QS::begin((uint8_t)QS_STR_DICTIONARY); \
        QS_U32_(hash_); \
        QS_STR_(str_); \
        QS::end(); \
        QS_INT_UNLOCK_(); \
        QS::flush(); \
    } else ((void)0)

/// \brief Output function dictionary record
///
/// A function dictionary record associates the binary address of a function
/// in the target's memory with the human-readable name of the function.
///
/// Providing a function dictionary QS record can vastly improve readability
/// of the QS log, because instead of dealing with cryptic machine addresses
/// the QSpy host utility can display human-readable function names.
///
/// The example from #QS_SIG_DICTIONARY shows the definition of a function
/// dictionary.
#define QS_FUN_DICTIONARY(fun_) QS_FUN_DICTIONARY_(fun_, #fun_)
#define QS_FUN_DICTIONARY_(fun_, str_) \
    if (((QS::glbFilter_[(uint8_t)QS_FUN_DICTIONARY >> 3U] \
          & (1U << ((uint8_t)QS_FUN_DICTIONARY & 7U))) != 0)) \
    { \
        QS_INT_LOCK_KEY_ \
        QS_INT_LOCK_(); \
        QS::begin((uint8_t)QS_FUN_DICTIONARY); \
        QS_FUN_(fun_); \
        QS_STR_(str_); \
        QS::end(); \
        QS_INT_UNLOCK_(); \
        QS::flush(); \
    } else ((void)0)

#else                                  // Q_SPY undefined--QS tracing DISABLED

// Dummy definitions of the QS macros that avoid code generation from
// the QS instrumentation.

#define QS_INIT(arg_)                      ((uint8_t)1)
#define QS_EXIT()                          ((void)0)
#define QS_FILTER_ON(rec_)                 ((void)0)
#define QS_FILTER_OFF(rec_)                ((void)0)
#define QS_FILTER_SM_OBJ(obj_)             ((void)0)
#define QS_FILTER_AO_OBJ(obj_)             ((void)0)
#define QS_FILTER_MP_OBJ(obj_)             ((void)0)
#define QS_FILTER_EQ_OBJ(obj_)             ((void)0)
#define QS_FILTER_TE_OBJ(obj_)             ((void)0)

#define QS_GET_BYTE(pByte_)                ((uint8_t)0)
#define QS_GET_BLOCK(pSize_)               ((uint8_t const *)0)

#define QS_BEGIN(rec_, obj_)               if (0) { ((void)0)
#define QS_END()                           } else ((void)0)
#define QS_BEGIN_NOLOCK(rec_, obj_)        QS_BEGIN(rec_, obj_)
#define QS_END_NOLOCK()                    QS_END()

#define QS_I8(width_, data_)               ((void)0)
#define QS_U8(width_, data_)               ((void)0)
#define QS_I16(width_, data_)              ((void)0)
#define QS_U16(width_, data_)              ((void)0)
#define QS_I32(width_, data_)              ((void)0)
#define QS_U32(width_, data_)              ((void)0)
#define QS_F32(width_, data_)              ((void)0)
#define QS_F64(width_, data_)              ((void)0)
#define QS_STR(str_)                       ((void)0)
#define QS_MEM(mem_, size_)                ((void)0)
#define QS_SIG(sig_, obj_)                 ((void)0)
#define QS_OBJ(obj_)                       ((void)0)
#define QS_FUN(fun_)                       ((void)0)
#define QS_HRESULT(kind_, data_)           ((void)0)
#define QS_HASHED_STR(hash_)               ((void)0)
#define QS_HASHED_STR_(hash_)              ((void)0)
#define QS_HASH_DYNAMIC_STR(str_)          ((void)0)
#define QS_HASH_DYNAMIC_STR_(str_)         ((void)0)
#define QS_HASH_STATIC_STR(str_)           ((void)0)
#define QS_HASH_STATIC_STR_(str_)          ((void)0)

#define QS_SIG_DICTIONARY(sig_, obj_)      ((void)0)
#define QS_OBJ_DICTIONARY(obj_)            ((void)0)
#define QS_FUN_DICTIONARY(fun_)            ((void)0)

#endif                                                                // Q_SPY


//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// The first QS records have identical numerical values as the four QEP
// reserved signals.
//

//////////////////////////////////////////////////////////////////////////////
/// \page qs_rev QS/C++ Revision History
///
/// \section qs_3_2_00 Version 3.2.00 (Commercial Release)
/// Release date: Aug 11, 2006\n
///
/// -# In file qs.h replaced QS_QK_SCHED_LOCK/UNLOCK with QS_QK_MUTEX_LOCK/
/// QS_QK_MUTEX_UNLOCK. Also, changed data accompanying the trace records
/// QS_QK_MUTEX_LOCK/UNLOCK.
/// -# In file qs.h used the macro Q_ROM to allocate constant objects
/// to ROM (\sa qep.h).
/// -# in file qs.cpp changed the version number to 3.2.00
/// -# Updated "QS Programmer's Manual" in PDF.
///
///
/// \section qs_3_1_06 Version 3.1.06 (Commercial Release)
/// Release date: May 07, 2006\n
///
/// -# Updated licensing information.
/// -# Updated "QS/C++ Programmer's Manual" in PDF.
///
///
/// \section qs_3_1_05 Version 3.1.05 (Product Release)
/// Release date: Feb 13, 2006\n
///
/// -# Provided "QS/C++ Programmer's Manual" in PDF.
/// -# In file qs.h changed around the pre-defined records. Added records:
/// QS_QF_INT_LOCK, QS_QF_INT_UNLOCK, QS_QF_ISR_ENTRY, QS_QF_ISR_EXIT. Removed
/// records QS_QK_INT_LOCK, QS_QK_INT_UNLOCK, QS_QK_ISR_ENTRY, QS_QK_ISR_EXIT.
/// -# In file qs.h renamed functions QS::filterIn()/ QS::filerOut() to
/// QS::filterOn()/ QS::filerOff(). Correspondingly, changed macros
/// QS_FILTER_IN(), QS_FILTER_OUT(), to QS_FILTER_ON(), QS_FILTER_OFF().
/// -# In file qs.h changed the signature and semantics of QS::getByte() to
/// return QS_EOD (End-Of-Data).
/// -# In file qs.h changed the signature of QS::getBlock() to take a pointer
/// to uint16_t rather than uint32_t.
/// -# In file qs.h eliminated the callback QS::newRecord()
/// -# In file qs.h added new callback QS::flush()
/// -# In file qs.h added application-level local filter object QS::apObj_.
/// Consistently, added macro QS_FILTER_AP_OBJ() to set the new local filter.
/// Consistently, added object argument to macros QS_BEGIN() and
/// QS_BEGIN_NOLOCK().
/// -# In files qs.cpp, qs_.cpp, qs_blk.cpp, qs_byte.cpp, qs_pkg.h renamed
/// some variables and adjusted comments.
/// -# Ported the QSpy host applicatoin to Linux. Added TCP/IP input to QSpy.
/// Added new options.
/// -# Added redesigned QS port to Linux with TCP/IP data link.
///
///
/// \section qs_3_1_03 Version 3.1.03 (Beta Release)
/// Release date: Nov 21, 2005\n
///
/// -# Added Doxygen documentation to the source code
/// -# Renamed file qqs.cpp to qs_.cpp
///

//////////////////////////////////////////////////////////////////////////////
/// \defgroup qs Quantum Spy in C++ (QS/C++)
/// \image html logo_qs_TM.jpg
///
/// Quantum Spy (QS) is a real-time tracing facility built into QEP, QF, and
/// QK components and also available to the Application code. QS allows you
/// to gain unprecedented visibility into your application by selectively
/// logging almost all interesting events occurring within state machines,
/// the framework, the kernel, and your application code. QS event logging
/// is minimally intrusive, offers precise time-stamping, sophisticated
/// runtime filtering of events, and good data compression. QS can be
/// configured to send the real-time data out of the serial or Ethernet port
/// of the target device, or even write the data to a file. QS is available
/// starting from QP v3.1.
///
/// \sa <A HREF="file:../../qs/QS_Manual.pdf">
///      QS/C++ Programmer's Manual</A> \n
///      \ref qs_rev

#endif                                                                 // qs_h
