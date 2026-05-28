//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++ platform-independent public interface
// Last Updated for Version: 3.2.00
// Date of the Last Update:  Aug 08, 2006
//
// Copyright (C) 2002-2006 Quantum Leaps, LLC. All rights reserved.
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
#ifndef qf_h
#define qf_h

////INSITU DLL STUFF////
#ifndef QF_NODLLIMPORT
    #define QF_API __declspec(dllimport)
#else
    #define QF_API
#endif
////END INSITU DLL STUFF////

/// \ingroup qf qk
/// \file qf.h
/// \brief QF/C++ platform-independent public interface.
///
/// This header file must be included directly or indirectly
/// in all modules (*.cpp files) that use QF/C++.

//////////////////////////////////////////////////////////////////////////////
#ifndef QF_MAX_ACTIVE

    /// \brief Default value of the macro configurable value in qf_port.h
    #define QF_MAX_ACTIVE  63
#elif (QF_MAX_ACTIVE < 1) || (63 < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE out of range. The valid range is 1..63"
#endif

//////////////////////////////////////////////////////////////////////////////
#ifndef QF_EVENT_SIZ_SIZE

    /// \brief Default value of the macro configurable value in qf_port.h
    #define QF_EVENT_SIZ_SIZE 2
#endif
#if (QF_EVENT_SIZ_SIZE == 1)

    /// \brief The data type to store the block-size defined based on
    /// the macro #QF_EVENT_SIZ_SIZE.
    ///
    /// The dynamic range of this data type determines the maximum block
    /// size that can be managed by the pool.
    typedef uint8_t QEventSize;
#elif (QF_EVENT_SIZ_SIZE == 2)
    typedef uint16_t QEventSize;
#elif (QF_EVENT_SIZ_SIZE == 4)
    typedef uint32_t QEventSize;
#else
    #error "QF_EVENT_SIZ_SIZE defined incorrectly, expected 1, 2, or 4"
#endif


//////////////////////////////////////////////////////////////////////////////
#ifndef QF_FSM_ACTIVE

    //////////////////////////////////////////////////////////////////////////
    /// \brief The macro defining the base class for QActive.
    ///
    /// By default, the ::QActive class is derived from ::QHsm. However,
    /// if the macro QF_FSM_ACTIVE is defined, QActive is derived from ::QFsm.
    /// Defining QF_FSM_ACTIVE can be advantageous in resource-constraint
    /// applications because avoiding QHsm functions saves about 1KB of code
    /// space (typically ROM), and QFsm functions are slightly faster than
    /// the more general QHsm functions.
    #define QF_ACTIVE_SUPER_  QHsm
#else
    #define QF_ACTIVE_SUPER_  QFsm
#endif

/// \brief Base class for derivation of application-level active object
/// classes.
///
/// QActive is the base class for derivation of active objects. Active objects
/// in QF are encapsulated tasks (each embedding a state machine and an event
/// queue) that communicate with one another asynchronously by sending and
/// receiving events. Within an active object, events are processed
/// sequentially in a run-to-completion (RTC) fashion, while QF encapsulates
/// all the details of thread-safe event exchange and queuing.
///
/// \note QActive is not intended to be instantiated directly, but rather
/// serves as the base class for derivation of active objects in the
/// application code.
///
/// The following example illustrates how to derive an active object from
/// QActive.
/// \include qf_qactive.cpp
///
/// \sa #QF_ACTIVE_SUPER_ defines the base class (either QHsm and QFsm)
class QF_API QActive : public QF_ACTIVE_SUPER_ {
private:

    /// \brief OS-dependent event-queue type.
    ///
    /// The type of the queue depends on the underlying operating system or
    /// a kernel. Many kernels support "message queues" that can be adapted
    /// to deliver QF events to the active object. Alternatively, QF provides
    /// a native event queue implementation that can be used as well.
    ///
    /// The native QF event queue is configured by defining the macro
    /// #QF_EQUEUE_TYPE as ::QEQueue.
    QF_EQUEUE_TYPE eQueue_;

    /// \brief OS-dependent object to block the native QF queue.
    ///
    /// This data member is only used with the native QF event queue.
    /// (A native QF queue is selected by setting #QF_EQUEUE_TYPE to
    /// ::QEQueue.)
    ///
    /// The osObject_ attribute determines the mechanism used to block the
    /// task of the active object when the event queue is empty. The
    /// applicable mechanisms are: a binary semaphore, a POSIX condition
    /// variable, a Win32 object to be used with WaitForSingleObject()/
    /// SetEvent() APIs, or others such OS-dependent mechanisms.
    QF_OS_OBJECT_TYPE osObject_;

    /// \brief OS-dependent representation of the thread of the active
    /// object.
    QF_THREAD_TYPE thread_;

    /// \brief QF priority associated with the active object.
    /// \sa QActive::start()
    uint8_t prio_;

    /// \brief The Boolean loop variable determining if the thread routine
    /// of the active object is running.
    ///
    /// This flag is only used with the traditional loop-structured thread
    /// routines. Clearing this flag breaks out of the thread loop, which is
    /// often the cleanest way to terminate the thread. The following example
    /// illustrates the thread routine for Win32:
    /// \include qf_run.cpp
    uint8_t running_;

public:

    /// \brief Starts execution of an active object and registers the object
    /// with the framework.
    ///
    /// The function takes six arguments.
    /// \a prio is the priority of the active object. QF allows you to start
    /// up to 63 active objects, each one having a unique priority number
    /// between 1 and 63 inclusive, where higher numerical values correspond
    /// to higher priority (urgency) of the active object relative to the
    /// others.
    /// \a qSto[] and \a qLen arguments are the storage and size of the event
    /// queue used by this active object.
    /// \a stkSto and \a stkSize are the stack storage and size in bytes.
    /// Please note that a per-active object stack is used only when the
    /// underlying OS requies it. If the stack is not required, or the
    /// underlying OS allocates the stack internally, the \a stkSto should be
    /// NULL and/or \a stkSize should be 0.
    /// \a ie is an optional initialization event that can be used to pass
    /// additional startup data to the active object. (Pass NULL if your
    /// active object does not expect the initialization event).
    ///
    /// \note This function is strongly OS-dependent and must be defined in
    /// the QF port to a particular platform.
    ///
    /// The following example shows starting of the Philosopher object when a
    /// per-task stack is required:
    /// \include qf_start.cpp
    void start(uint8_t prio,
               QEvent const *qSto[], uint32_t qLen,
               void *stkSto, uint32_t stkSize,
               QEvent const *ie);

    /// \brief Posts an event \a e directly to the event queue of the acitve
    /// object \a me using the First-In-First-Out (FIFO) policy.
    ///
    /// Direct event posting is the simplest asynchronous communication method
    /// available in QF. The following example illustrates how the Philosopher
    /// active obejct posts directly the HUNGRY event to the Table active
    /// object. \include qf_post.cpp
    ///
    /// \note The producer of the event (Philosopher in this case) must only
    /// "know" the recipient (Table) by a generic (QActive *QDPP_table)
    /// pointer, but the specific definition of the Table class is not
    /// required.
    ///
    /// \note Direct event posting should not be confused with direct event
    /// dispatching. In contrast to asynchronous event posting through event
    /// queues, direct event dispatching is synchronous. Direct event
    /// dispatching occurs when you call QHsm::dispatch(), or QFsm::dispatch()
    /// function.
    void postFIFO(QEvent const *e);

    /// \brief Posts an event directly to the event queue of the active object
    /// \a me using the Last-In-First-Out (LIFO) policy.
    ///
    /// \note The LIFO policy should be used only with great caution because
    /// it alters order of events in the queue.
    /// \sa QActive::postFIFO()
    void postLIFO(QEvent const *e);

    /// \brief Traditional loop-structured thread routine for active objects
    ///
    /// This function is only used when QF is ported to a traditional
    /// RTOS/Kernel. QActive::run() is structured as a typical endless loop,
    /// which blocks on the event queue get() operation. When an event
    /// becomes available, it's dispatched to the active object's state
    /// machine and after this recycled with QF::gc(). The loop might
    /// optionally use the QActive::running_ flag to terminate and cause
    /// QActive::run() to return which is often the cleanest way to terminate
    /// the thread.
    void run(void);

protected:

    /// \brief protected constructor
    ///
    /// Performs the first step of active object initialization by assigning
    /// the initial pseudostate to the currently active state of the state
    /// machine.
    ///
    /// \note The constructor is protected to prevent direct instantiation
    /// of QActive objects. This class is intended only for derivation
    /// (abstract class).
    QActive(QState initial);
    ~QActive();

    /// \brief Stops execution of an active object and removes it from the
    /// framework's supervision.
    ///
    /// The preferred way of calling this method is from within the active
    /// object that needs to stop (that's why this method is protected).
    /// In other words, an active object should stop itself rather than being
    /// stopped by some other entity. This policy works best, because only
    /// the active object itself "knows" when it has reached the appropriate
    /// state for the shutdown.
    ///
    /// \note This method is strongly OS-dependent and should be defined in
    /// the QF port to a particular platform. This function is optional in
    /// embedded systems where active objects never need to be stopped.
    void stop(void);

    /// \brief Subscribes for delivery of signal \a sig to the active object
    ///
    /// This function is part of the Publish-Subscribe event delivery
    /// mechanism available in QF. Subscribing to an event means that the
    /// framework will start posting all published events with a given signal
    /// \a sig to the event queue of the active object.
    ///
    /// The following example shows how the Table active object subscribes
    /// to three signals in the initial transition:
    /// \include qf_subscribe.cpp
    ///
    /// \sa QF::publish(), QActive::unsubscribe(), and
    /// QActive::unsubscribeAll()
    void subscribe(QSignal sig) const;

    /// \brief Un-subscribes from the delivery of signal \a sig to the
    /// active object.
    ///
    /// This function is part of the Publish-Subscribe event delivery
    /// mechanism available in QF. Un-subscribing from an event means that
    /// the framework will stop posting published events with a given signal
    /// \a sig to the event queue of the active object.
    ///
    /// \note Due to the latency of event queues, an active object should NOT
    /// assume that a given signal \a sig will never be dispatched to the
    /// state machine of the active object after un-subscribing from that
    /// signal. The event might be already in the queue, or just about to be
    /// posted and the un-subscribe operation will not flush such events.
    ///
    /// \note Un-subscribing from a signal that has never been subscribed in
    /// the first place is considered an error and QF will rise an assertion.
    ///
    /// \sa QF::publish(), QActive::subscribe(), and QActive::unsubscribeAll()
    void unsubscribe(QSignal sig) const;

    /// \brief Un-subscribes from the delivery of all signals to the active
    /// object.
    ///
    /// This function is part of the Publish-Subscribe event delivery
    /// mechanism available in QF. Un-subscribing from all events means that
    /// the framework will stop posting any published events to the event
    /// queue of the active object.
    ///
    /// \note Due to the latency of event queues, an active object should NOT
    /// assume that no events will ever be dispatched to the state machine of
    /// the active object after un-subscribing from all events.
    /// The events might be already in the queue, or just about to be posted
    /// and the un-subscribe operation will not flush such events. Also, the
    /// alternative event-delivery mechanisms, such as direct event posting or
    /// time events, can be still delivered to the event queue of the active
    /// object.
    ///
    /// \sa QF::publish(), QActive::subscribe(), and QActive::unsubscribe()
    void unsubscribeAll(void) const;

private:

    /// \brief Native QF event queue implementation for the First-In-First-Out
    /// (FIFO) event posting.
    ///
    /// This is an internal QF function used only when the native QF queue is
    /// selected by defining the macro #QF_EQUEUE_TYPE as ::QEQueue.
    /// The function raises an assertion if the native QF queue becomes full
    /// and cannot accept the event.
    ///
    /// \note This private function is called through the indirection layer
    /// of the macro #QACTIVE_POST_FIFO_ from QActive_postFIFO() function
    /// only when the native QF event queue is selected and the internal
    /// macro #QACTIVE_POST_FIFO_ is defined as follows:
    /// \include qf_postFIFO_.cpp
    ///
    /// \sa QActive::postFIFO(), QActive::postLIFO_()
    void postFIFO_(QEvent const *e);

    /// \brief Native QF event queue implementation for the Last-In-First-Out
    /// (LIFO) event posting.
    ///
    /// This is an internal QF function used only when the native QF queue is
    /// selected by defining the macro #QF_EQUEUE_TYPE as ::QEQueue.
    /// The function raises an assertion if the native QF queue becomes full
    /// and cannot accept the event.
    ///
    /// \note The LIFO policy should be used only with great caution because
    /// it alters order of events in the queue.
    /// \note This private function is called through the indirection layer of
    /// the macro #QACTIVE_POST_LIFO_ from QActive_postLIFO() function only
    /// when the native QF event queue is selected and the internal macro
    /// #QACTIVE_POST_LIFO_ is defined as follows:
    /// \include qf_postLIFO_.cpp
    ///
    /// \sa QActive::postLIFO(), QActive::postFIFO_(),
    void postLIFO_(QEvent const *e);

    /// \brief Native QF event queue implementation for the get operation.
    ///
    /// This is an internal QF function used only when the native QF queue is
    /// selected by defining the macro #QF_EQUEUE_TYPE as ::QEQueue.
    /// Depending on the underlying OS or kernel, the function might block the
    /// calling thread when no events are available.
    ///
    /// \note This private function is called through the indirection layer of
    /// the macro #QACTIVE_GET_ defined as follows:
    /// \include qf_get_.cpp
    ///
    /// \sa QActive::postFIFO_(), QActive::postLIFO_()
    QEvent const *get_(void);

    friend class QF;
    friend class QTimeEvt;
    #ifndef QK_INT_KEY_TYPE
        friend void QK_schedule_(void);
    #else
        friend void QK_schedule_(QK_INT_KEY_TYPE intLockKey);
    #endif
};


//////////////////////////////////////////////////////////////////////////////
#ifndef QF_TIMEEVT_CTR_SIZE
    /// \brief macro to override the default QTimeEvtCtr size.
    /// Valid values 1, 2, or 4; default 2
    #define QF_TIMEEVT_CTR_SIZE 2
#endif
#if (QF_TIMEEVT_CTR_SIZE == 1)

    /// \brief type of the Time Event counter, which determines the dynamic
    /// range of the time delays measured in clock ticks.
    ///
    /// This typedef is configurable via the preprocessor switch
    /// #QF_TIMEEVT_CTR_SIZE. The other possible values of this type are
    /// as follows: \n
    /// uint8_t when (QF_TIMEEVT_CTR_SIZE == 1), and \n
    /// uint32_t when (QF_TIMEEVT_CTR_SIZE == 4).
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2)
    typedef uint16_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 4)
    typedef uint32_t QTimeEvtCtr;
#else
    #error "QF_TIMEEVT_CTR_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

//////////////////////////////////////////////////////////////////////////////
/// \brief Time Event class
///
/// Time events are special QF events equipped with the notion of time
/// passage. The basic usage model of the time events is as follows. An
/// active object allocates one or more QTimeEvt objects (provides the
/// storage for them). When the active object needs to arrange for a timeout,
/// it arms one of its time events to fire either just once (one-shot) or
/// periodically. Each time event times out independently from the others,
/// so a QF application can make multiple parallel timeout requests (from the
/// same or different active objects). When QF detects that the appropriate
/// moment has arrived, it inserts the time event directly into the
/// recipient's event queue. The recipient then processes the time event just
/// like any other event.
///
/// Time events, as any other QF events derive from the ::QEvent base
/// class. Typically, you will use a time event as-is, but you can also
/// further derive more specialized time events from it by adding some more
/// data members and/or specialized functions that operate on the specialized
/// time events.
///
/// Internally, the armed time events are organized into a bi-directional
/// linked list. This linked list is scanned in every invocation of the
/// QF::tick() function. Only armed (timing out) time events are in the list,
/// so only armed time events consume CPU cycles.
///
/// \note QF manages the time events in the function QF::tick(), which
/// must be called periodically, preferably from the clock tick ISR.
/// \note In this version of QF QTimeEvt objects should be allocated
/// statically rather than dynamically from event pools. Currently, QF will
/// not correctly recycle the dynamically allocated Time Events.
class QF_API QTimeEvt : public QEvent {
private:

    //// link to the previous time event in the list
    QTimeEvt *prev_;

    /// link to the next time event in the list
    QTimeEvt *next_;

    /// the active object that receives the time events. If this pointer is
    /// NULL, the event is published globally rather than posted directly.
    QActive *act_;

    /// the internal down-counter of the time event. The down-counter
    /// is decremented by 1 in every QF_tick() invocation. The time event
    /// fires (gets posted or published) when the down-counter reaches zero.
    QTimeEvtCtr ctr_;

    /// the interval for the periodic time event (zero for the one-shot
    /// time event). The value of the interval is re-loaded to the internal
    /// down-counter when the time event expires, so that the time event
    /// keeps timing out periodically.
    QTimeEvtCtr interval_;

public:

    /// \brief The Time Event constructor.
    ///
    /// The most important initialization performed in  the constructor is
    /// assigning a signal to the Time Event. You can reuse the Time Event
    /// any number of times, but you cannot change the signal.
    /// This is because pointers to Time Events might still be held in event
    /// queues and changing signal could to hard-to-detect errors.
    ///
    /// The following example shows the use of QTimeEvt::QTimeEvt()
    /// constructor in the constructor initializer list of the Philosopher
    /// active object constructor that owns the time event
    /// \include qf_ctor.cpp
    QTimeEvt(QSignal s);

    /// \brief Arm a one-shot time event for direct event posting.
    ///
    /// Arms a time event to fire in \a nTicks clock ticks (one-shot time
    /// event). The time event gets directly posted (using the FIFO policy)
    /// into the event queue of the active object \a act.
    ///
    /// After posting, the time event gets automatically disarmed and can be
    /// reused for a one-shot or periodic timeout requests.
    ///
    /// A one-shot time event can be disarmed at any time by calling the
    /// QTimeEvt::disarm() function. Also, a one-shot time event can be
    /// re-armed to fire in a different number of clock ticks by calling the
    /// QTimeEvt::rearm() function.
    ///
    /// The following example shows how to arm a one-shot time event from a
    /// state machine of an active object:
    /// \include qf_state.cpp
    void postIn(QActive *act, QTimeEvtCtr nTicks) {
        interval_ = (uint16_t)0;
        arm_(act, nTicks);
    }

    /// \brief Arm a one-shot time event for global event publishing.
    ///
    /// Arms a time event to fire in \a nTicks clock ticks (one-shot time
    /// event). The time event gets globally published to the framework
    /// to be delivered to all subscriber active objects.
    ///
    /// After posting, the time event gets automatically disarmed and can be
    /// reused for a one-shot or periodic timeout requests.
    ///
    /// A one-shot time event can be disarmed at any time by calling the
    /// QTimeEvt_disarm() function. Also, a one-shot time event can be
    /// re-armed to fire in a different number of clock ticks by calling the
    /// QTimeEvt::rearm() function.
    void publishIn(QTimeEvtCtr nTicks) {
        interval_ = (uint16_t)0;
        arm_((QActive *)0, nTicks);
    }

    /// \brief Arm a periodic time event for direct event posting.
    ///
    /// Arms a time event to fire every \a nTicks clock ticks (periodic time
    /// event). The time event gets directly posted (using the FIFO policy)
    /// into the event queue of the active object \a act.
    ///
    /// After posting, the time event gets automatically re-armed to fire
    /// again in the specified \a nTicks clock ticks.
    ///
    /// A periodic time event can be disarmed only by calling the
    /// QTimeEvt::disarm() function. After disarming, the time event can be
    /// reused for a one-shot or periodic timeout requests.
    ///
    /// \note An attempt to reuse (arm again) a running periodic time event
    /// raises an assertion.
    ///
    /// Also, a periodic time event can be re-armed to shorten or extend the
    /// current period by calling the QTimeEvt_rearm() function. After
    /// adjusting the current period, the periodic time event goes back
    /// timing out at the original rate.
    void postEvery(QActive *act, QTimeEvtCtr nTicks) {
        interval_ = nTicks;
        arm_(act, nTicks);
    }

    /// \brief Arm a periodic time event for global event publishing.
    ///
    /// Arms a time event to fire every \a nTicks clock ticks (periodic time
    /// event). The time event gets globally published to the framework to
    /// be delivered to all subscriber active objects.
    ///
    /// After posting, the time event gets automatically re-armed to fire
    /// again in the specified \a nTicks clock ticks.
    ///
    /// A periodic time event can be disarmed only by calling the
    /// QTimeEvt::disarm() function. After disarming, the time event can be
    /// reused for a one-shot or periodic timeout requests.
    ///
    /// \note An attempt to reuse (arm again) a running periodic time event
    /// raises an assertion.
    ///
    /// Also, a periodic time event can be re-armed to shorten or extend the
    /// current period by calling the QTimeEvt_rearm() function. After
    /// adjusting the current period, the periodic time event goes back
    /// timing out at the original rate.
    void publishEvery(QTimeEvtCtr nTicks) {
        interval_ = nTicks;
        arm_((QActive *)0, nTicks);
    }

    /// \brief Disarm a time event.
    ///
    /// The time event gets disarmed and can be reused. The function
    /// returns 1 (TRUE) if the time event was truly disarmed, that is, it
    /// was running. The return of 0 (FALSE) means that the time event was
    /// not truly disarmed because it was not running. The FALSE return is
    /// only possible for one-shot time events that have been automatically
    /// disarmed upon expiration. In this case the FALSE return means that
    /// the time event has already been posted or published and should be
    /// expected in the active object's state machine.
    uint8_t disarm(void);

    /// \brief Rearm a time event.
    ///
    /// The time event gets rearmed with a new number of clock ticks
    /// \a nTicks. This facility can be used to prevent a one-shot time event
    /// from expiring (e.g., a watchdog time event), or to adjusts the
    /// current period of a periodic time event. Rearming a periodic timer
    /// leaves the interval unchanged and is a convenient method to adjust the
    /// phasing of the periodic time event.
    ///
    /// The function returns 1 (TRUE) if the time event was running as it
    /// was re-armed. The return of 0 (FALSE) means that the time event was
    /// not truly rearmed because it was not running. The FALSE return is only
    /// possible for one-shot time events that have been automatically
    /// disarmed upon expiration. In this case the FALSE return means that
    /// the time event has already been posted or published and should be
    /// expected in the active object's state machine.
    uint8_t rearm(QTimeEvtCtr nTicks);

    // for backwards compatibility

    /// \brief Arm a one-shot time event for direct event posting (obsolete).
    ///
    /// This facility is now obsolete, please use \sa QTimeEvt::postIn().
    void fireIn(QActive *act, QTimeEvtCtr nTicks) {
        postIn(act, nTicks);
    }

    /// \brief Arm a periodic time event for direct event posting (obsolete).
    ///
    /// This facility is now obsolete, please use \sa QTimeEvt::postEvery().
    void fireEvery(QActive *act, QTimeEvtCtr nTicks) {
        postEvery(act, nTicks);
    }

private:

    /// \brief Arm a time event (internal function to be used through macros
    /// only).
    ///
    /// \sa QTimeEvt::postIn(), QTimeEvt::postEvery(),
    /// \sa QTimeEvt::publishIn(), QTimeEvt::publishEvery()
    void arm_(QActive *act, QTimeEvtCtr nTicks);

    friend class QF;
};


#if (QF_MAX_ACTIVE > 63)
    #error "QF_MAX_ACTIVE exceeds 63"
#endif

//////////////////////////////////////////////////////////////////////////////
/// \brief Subscriber List class
///
/// This data type represents a set of active objects that subscribe to
/// a given signal. The set is represented as an array of bits, where each
/// bit corresponds to the unique priority of an active object.
class QF_API QSubscrList {
private:

    /// An array of bits representing subscriber active objects. Each bit
    /// in the array corresponds to the unique priority of the active object.
    /// The size of the array is determined of the maximum number of active
    /// objects in the application configured by the #QF_MAX_ACTIVE macro.
    /// For example, an active object of priority p is a subscriber if the
    /// following is true: ((bits[QF::div8Lkup[p]] & QF::pwr2Lkup[p]) != 0)
    ///
    /// \sa QF::psInit(), QF::div8Lkup, QF::pwr2Lkup, #QF_MAX_ACTIVE
    uint8_t bits[((QF_MAX_ACTIVE - 1) / 8) + 1];

    friend class QF;
    friend class QActive;
};

//////////////////////////////////////////////////////////////////////////////
/// \brief QF services.
///
/// This class groups together QF services. It has only static members and
/// should not be instantiated.
class QF_API QF {
public:

    /// \brief QF initialization.
    ///
    /// This function initializes QF and must be called exactly once before
    /// any other QF function.
    static void init(void);

    /// \brief Publish-subscribe initialization.
    ///
    /// This function initializes the publish-subscribe facilities of QF and
    /// must be called exactly once before any subscriptions/publications
    /// occur in the application. The arguments are as follows: \a subscrSto
    /// is a pointer to the array of subscriber-lists. \a maxSignal is the
    /// dimension of this array and at the same time the maximum signal that
    /// can be published or subscribed.
    ///
    /// The array of subscriber-lists is indexed by signals and provides
    /// mapping between the signals and subscirber-lists. The subscriber-
    /// lists are bitmasks of type ::QSubscrList, each bit in the bitmask
    /// corresponding to the unique priority of an active object. The size
    /// of the ::QSubscrList bitmask depends on the value of the
    /// #QF_MAX_ACTIVE macro.
    ///
    /// \note The publish-subscribe facilities are optional, meaning that
    /// you might choose not to use publish-subscribe. In that case calling
    /// QF::psInit() and using up memory for the subscriber-lists is
    /// unnecessary.
    ///
    /// \sa ::QSubscrList
    ///
    /// The following example shows the typical initialization sequence of
    /// QF: \include qf_main.cpp
    static void psInit(QSubscrList *subscrSto, QSignal maxSignal);

    /// \brief Event pool initialization for dynamic allocation of events.
    ///
    /// This function initializes one event pool at a time and must be called
    /// exactly once for each event pool before the pool can be used.
    /// The arguments are as follows: \a poolSto is a pointer to the memory
    /// block for the events. \a poolSize is the size of the memory block in
    /// bytes. \a evtSize is the block-size of the pool in bytes, which
    /// determines the maximum size of events that can be allocated from the
    /// pool.
    ///
    /// You might initialize one, two, and up to three event pools by making
    /// one, two, or three calls to the QF_poolInit() function. However,
    /// for the simplicity of the internal implementation, you must initialize
    /// event pools in the ascending order of the event size.
    ///
    /// Many RTOSes provide fixed block-size heaps, a.k.a. memory pools that
    /// can be used for QF event pools. In case such support is missing, QF
    /// provides a native QF event pool implementation. The macro
    /// #QF_EPOOL_TYPE_ determines the type of event pool used by a
    /// particular QF port. See class ::QMPool for more information.
    ///
    /// \note The actual number of events available in the pool might be
    /// actually less than (\a poolSize / \a evtSize) due to the internal
    /// alignment of the blocks that the pool might perform. You can always
    /// check the capacity of the pool by calling QF::getPoolMargin().
    ///
    /// \note The dynamic allocation of events is optional, meaning that you
    /// might choose not to use dynamic events. In that case calling
    /// QF::poolInit() and using up memory for the memory blocks is
    /// unnecessary.
    ///
    /// \sa QF initialization example for QF::init()
    static void poolInit(void *poolSto, uint32_t poolSize, uint16_t evtSize);

    /// \brief Starts the interrupts and initializes other critical resources
    /// that might interact with the QF application.
    ///
    /// The timeline for calling QF::start() depends on the particular port
    /// of QF. In general, this function must be called after QF::init(), but
    /// before QF::run(). Often it is actually called from QF::run(), right
    /// before starting any multitasking kernel or the background loop.
    ///
    /// \note This function is strongly platform-dependent and is not
    /// implemented in the QF, but either in the QF port or in the Board
    /// Support Package (BSP) for the given application. Some QF ports might
    /// not require implementing QF::start() at all.
    ///
    /// \sa QF initialization example for QF::init()
    static void start(void);

    /// \brief Transfers control to QF to run the application.
    ///
    /// QF::run() is typically called from your startup code after you
    /// initialize the QF and start at least one active object with
    /// QActive::start(). Also, QF::start() call must precede the transfer
    /// of control to QF::run(), but some QF ports might call QF::start()
    /// from QF::run(). QF::run() typically never returns to the caller.
    ///
    /// \note This function is strongly platform-dependent and is not
    /// implemented in the QF, but either in the QF port or in the
    /// Board Support Package (BSP) for the given application. All QF ports
    /// must implement QF::run().
    ///
    /// \note When the Quantum Kernel (QK) is used as the underlying real-time
    /// kernel for the QF, all platfrom dependencies are handled in the QK, so
    /// no porting of QF is necessary. In other words, you only need to
    /// recompile the QF platform-independent code with the compiler for your
    /// platform, but you don't need to provide any platform-specific
    /// implementation (so, no qf_port.cpp file is necessary). Moreover, QK
    /// implements the function QF::run() in a platform-independent way,
    /// in the modile qk.cpp.
    static void run(void);

#ifndef QF_INT_KEY_TYPE
    static void onIdle(void);                // interrupt lock key NOT defined

#else

    /// \brief QF idle callback (customized in BSPs for QF)
    ///
    /// QF::onIdle() is called by the non-preemptive scheduler built into QF
    /// when the framework detects that no events are available for active
    /// objects (the idle condition). This callback gives the application an
    /// opportunity to enter a power-saving CPU mode, or perform some other
    /// idle processing (such as Q-Spy output).
    ///
    /// \note QF::onIdle() is invoked with interrupts LOCKED because the idle
    /// condition can be asynchronously changed at any time by an interrupt.
    /// QF::onIdle() MUST unlock the interrupts internally, but not before
    /// putting the CPU into the low-power mode. (Ideally, unlocking
    /// interrupts and low-power mode should happen atomically). At the very
    /// least, the function MUST unlock interrupts, otherwise interrups will
    /// be locked permanently.
    ///
    /// \note QF::onIdle() is only used by the non-preemptive scheduler built
    /// into QF in the "bare metal" port, and is NOT used in any other ports.
    /// When QF is combined with QK, the QK idle loop calls a different
    /// function QK::onIdle(), with different semantics than QF::onIdle().
    /// When QF is combined with a 3rd-party RTOS or kernel, the idle
    /// processing mechanism of the RTOS or kernal is used instead of
    /// QF::onIdle().
    static void onIdle(QF_INT_KEY_TYPE intLockKey);   // int. lock key defined

#endif                                                      // QF_INT_KEY_TYPE

    /// \brief Function invoked by the application layer to stop the QF
    /// application and return control to the OS/Kernel.
    ///
    /// This function stops the QF application. After calling this function,
    /// QF attempts to gracefully stop the  application. This graceful
    /// shutdown might take some time to complete. The typical use of this
    /// funcition is for terminating the QF application to return back to the
    /// operating system or for handling fatal errors that require shutting
    /// down (and possibly re-setting) the system.
    ///
    /// This function is strongly platform-specific and is not implemented in
    /// the QF but either in the QF port or in the Board Support Package (BSP)
    /// for the given application. Some QF ports might not require
    /// implementing QF::stop() at all, because many embedded application
    /// don't have anything to exit to.
    ///
    /// \sa QF::stop() and QF::cleanup()
    static void stop(void);

    /// \brief QF cleanup callback (customized in QF ports or BSPs for QF)
    ///
    /// QF::cleanup() is called in some QF ports before QF returns to the
    /// unerlying operating system or RTOS.
    ///
    /// This function is strongly platform-specific and is not implemented in
    /// the QF but either in the QF port or in the Board Support Package (BSP)
    /// for the given application. Some QF ports might not require
    /// implementing QF::cleanup() at all, because many embedded application
    /// don't have anything to exit to.
    ///
    /// \sa QF::init() and QF::stop()
    static void cleanup(void);

    /// \brief Exits the QF application and returns control to the OS/Kernel.
    /// (Deprecated: use QF::stop() in new designs)
    ///
    /// This function exits the framework. After calling this function, QF is
    /// no longer in control of the application. The typical use of this
    /// method is for exiting the QF application to return back to the
    /// operating system or for handling fatal errors that require resetting
    /// the system.
    ///
    /// This function is strongly platform-dependent and is not implemented in
    /// the QF, but either in the QF port or in the Board Support Package
    /// (BSP) for the given application. Some QF ports might not require
    /// implementing QF::exit() at all, because many embedded application
    /// don't have anything to exit to.
    ///
    /// \sa QF::stop() and QF::cleanup()
    static void exit(void);

    /// \brief Publish event to the framework.
    ///
    /// This function posts (using the FIFO policy) the event \a e it to ALL
    /// active object that have subscribed to the signal \a e->sig.
    /// This function is designed to be callable from any part of the system,
    /// including ISRs, device drivers, and active objects.
    ///
    /// In the general case, event publishing requires multi-casting the
    /// event to multiple subscribers. This happens in the caller's thread
    /// with the scheduler locked to prevent preemptions during the multi-
    /// casting process. (Please note that the interrupts are not locked.)
    static void publish(QEvent const *e);

    /// \brief Processes all armed time events at every clock tick.
    ///
    /// This function must be called periodically from a time-tick ISR or from
    /// the highest-priority task so that QF can manage the timeout events.
    ///
    /// \note The QF::tick() function is not reentrant meaning that it must
    /// run to completion before it is called again. Also, QF::tick() assumes
    /// that it never will get preempted by a task, which is always the case
    /// when it is called from an ISR or the highest-priority task.
    ///
    /// \sa ::QTimeEvt.
    ///
    /// The following example illustrates the call to QF::tick():
    /// \include qf_tick.cpp
    static void tick(void);

    /// \brief Return the running tick counter.
    ///
    /// The value returned by QF::getTime() has no meaningful absolute value
    /// but is meaningful only relatively to the values returned by
    /// QF::getTime() at other time instances. The internal QF_tickCtr_
    /// counter wraps around through 0xFFFFFFFF to 0, which is transparent
    /// to any time difference calculations.
    ///
    /// For example, a time difference calculated across the wrap-around
    /// might look as follows: \n
    /// t1 == 0xFFFFFFFB (QF::getTime() value returned at instance 1) \n
    /// t2 == 0x00000003 (QF::getTime() value returned 8 ticks later) \n
    ///
    /// in the 2-complement unsigned arithmetic the difference is still
    /// 8 ticks, even across the wrap-around point: \n
    /// dt = t2 - t1 == 0x00000003 - 0xFFFFFFFB == 8 \n
    ///
    /// \note This function uses internally a critical section, so that
    /// it is reentrant on any CPU (even 8-bit machines that cannot handle
    /// 32-bit numbers atomically). However, you should be careful not to
    /// call this function from within a critical section in case nesting of
    /// critical sections is not supported.
    static QTimeEvtCtr getTime(void);

    /// \brief Returns the QF version.
    ///
    /// This function returns constant version string in the format x.y.zz,
    /// where x (one digit) is the major version, y (one digit) is the minor
    /// version, and zz (two digits) is the maintenance release version.
    /// An example of the version string is "3.1.03".
    ///
    /// The following example illustrates the usage of this function:
    /// \include qf_version.cpp
    static char const Q_ROM *getVersion(void);

    /// \brief Returns the QF-port version.
    ///
    /// This function returns constant version string in the format x.y.zz,
    /// where x (one digit) is the major version, y (one digit) is the minor
    /// version, and zz (two digits) is the maintenance release version.
    /// An example of the QF-port version string is "1.1.03".
    ///
    /// \sa QF::getVersion()
    static char const Q_ROM *getPortVersion(void);

    /// \brief This function returns the margin of the given event pool.
    ///
    /// This function returns the margin of the given event pool \a poolId,
    /// where poolId is the ID of the pool initialized by the call to
    /// QF::poolInit(). The poolId of the first initialized pool is 1, the
    /// second 2, and so on.
    ///
    /// The returned pool margin is the minimal number of free blocks
    /// encountered in the given pool since system startup.
    ///
    /// \note Requesting the margin of an un-initialized pool raises an
    /// assertion in the QF.
    static uint32_t getPoolMargin(uint8_t poolId);

    /// \brief This function returns the margin of the given event queue.
    ///
    /// This function returns the margin of the given event queue of an active
    /// object with priority \a prio. (QF priorities start with 1 and go up to
    /// #QF_MAX_ACTIVE.) The margin is the minimal number of free events
    /// encountered in the given queue since system startup.
    ///
    /// \note QF::getQueueMargin() is available only when the native QF event
    /// queue implementation is used. Requesting the queue margin of an unused
    /// priority level raises an assertion in the QF. (A priority level
    /// becomes used in QF after the call to the QF::add_() function.)
    static uint32_t getQueueMargin(uint8_t prio);

    /// \brief Internal QF implementation of the dynamic event allocator.
    ///
    /// \note The application code should not call this function directly.
    /// Please use the macro #Q_NEW.
    static QEvent *new_(uint16_t evtSize, QSignal sig);

    /// \brief Allocate a dynamic event.
    ///
    /// This macro returns an event pointer cast to the type \a evtT_. The
    /// event is initialized with the signal \a sig. Internally, the macro
    /// calls the internal QF function QF::new_(), which always returns a
    /// valid event pointer.
    ///
    /// \note The internal QF function QF::new_() raises an assertion when
    /// the allocation of the event turns out to be impossible due to event
    /// pool depletion, or incorrect (too big) size of the requested event.
    ///
    /// The following example illustrates dynamic allocation of an event:
    /// \include qf_post.cpp
    #define Q_NEW(evtT_, sig_) ((evtT_ *)QF::new_(sizeof(evtT_), (sig_)))

    /// \brief Recycle a dynamic event.
    ///
    /// This function implements a simple garbage collector for the dynamic
    /// events. Only dynamic events are candidates for recycling. (A dynamic
    /// event is one that is allocated from an event pool, which is
    /// determined as non-zero e->attrQF__ attribute.) Next, the function
    /// decrements the reference counter of the event, and recycles the event
    /// only if the counter drops to zero (meaning that no more references
    /// are outstanding for this event). The dynamic event is recycled by
    /// returning it to the pool from which it was originally allocated.
    /// The pool-of-origin information is stored in the upper 2-MSBs of the
    /// e->attrQF__ attribute.)
    ///
    /// \note QF invokes the garbage collector at all appropriate contexts,
    /// when an event can become garbage (automatic garbage collection),
    /// so the application code should have NO need to call QF::gc() directly.
    /// The QF::gc() function is exposed only for special cases when your
    /// application sends dynamic events to the "raw" thread-safe queues
    /// (see ::QEQueue). Such queues are processed outside of QF and the
    /// automatic garbage collection CANNOT be performed for these events.
    /// In this case you need to call QF::gc() explicitly.
    static void gc(QEvent const *e);

    /// \brief array of registered active objects
    ///
    /// \note Not to be used by Clients directly, only in ports of QF
    static QActive *active_[];

    // useful lookup tables

    /// \brief Lookup table for (log2(n) + 1), where n is the index
    /// into the table.
    ///
    /// This lookup delivers the 1-based number of the most significant 1-bit
    /// of a byte.
    ///
    /// \note Index range n = 0..255. The first index (n == 0) should never
    /// be used.
    ///
    static uint8_t const Q_ROM log2Lkup[256];

    /// \brief Lookup table for (1 << ((n-1) % 8)), where n is the index
    /// into the table.
    ///
    /// \note Index range n = 0..64. The first index (n == 0) should never
    /// be used.
    static uint8_t const Q_ROM pwr2Lkup[65];

    /// \brief Lookup table for ~(1 << ((n-1) % 8)), where n is the index
    /// into the table.
    ///
    /// \note Index range n = 0..64. The first index (n == 0) should never
    /// be used.
    static uint8_t const Q_ROM invPwr2Lkup[65];

    /// \brief Lookup table for (n-1)/8
    ///
    /// \note Index range n = 0..64. The first index (n == 0) should never
    /// be used.
    static uint8_t const Q_ROM div8Lkup[65];

private:                                // methods to be used in QF ports only

    /// \brief Register an active object to be managed by the framework
    ///
    /// This function should not be called by the application directly, only
    /// through the function QActive::start(). The priority of the active
    /// object \a a should be set before calling this function.
    ///
    /// \note This function raises an assertion if the priority of the active
    /// object exceeds the maximum value #QF_MAX_ACTIVE. Also, this function
    /// raises an assertion if the priority of the active object is already in
    /// use. (QF requires each active object to have a UNIQUE priority.)
    static void add_(QActive *a);

    /// \brief Remove the active object from the framework.
    ///
    /// This function should not be called by the application directly, only
    /// inside the QF port. The priority level occupied by the active object
    /// is freed-up and can be reused for another active object.
    ///
    /// The active object that is removed from the framework can no longer
    /// participate in the publish-subscribe event exchange.
    ///
    /// \note This function raises an assertion if the priority of the active
    /// object exceeds the maximum value #QF_MAX_ACTIVE or is not used.
    static void remove_(QActive const *a);

    friend class QActive;
};

/// \brief counter incremented every tick
///
/// \note Not to be used by Clients directly, only in ports of QF
extern QTimeEvtCtr QF_tickCtr_;

//////////////////////////////////////////////////////////////////////////////
/// Macros for QS instrumentation of interrupts locking/unlocking and
/// ISR entry/exit

#ifdef Q_SPY

/// \brief interrupt-lock nesting level
///
/// \note Not to be used by Clients directly, only in ports of QF
extern QF_API uint8_t QF_intLockNest_;

/// \brief ISR-call nesting level
///
/// \note Not to be used by Clients directly, only in ports of QF
extern QF_API uint8_t QF_isrNest_;

#define QF_QS_INT_LOCK() \
    QS_BEGIN_NOLOCK_(QS_QF_INT_LOCK, 0, 0); \
        QS_TIME_(); \
        QS_U8_((uint8_t)(++QF_intLockNest_)); \
    QS_END_NOLOCK_()

#define QF_QS_INT_UNLOCK() \
    QS_BEGIN_NOLOCK_(QS_QF_INT_UNLOCK, 0, 0); \
        QS_TIME_(); \
        QS_U8_((uint8_t)(QF_intLockNest_--)); \
    QS_END_NOLOCK_()

#define QF_QS_ISR_ENTRY(prio_) \
    QS_BEGIN_NOLOCK_(QS_QF_ISR_ENTRY, 0, 0); \
        QS_TIME_(); \
        QS_U8_((uint8_t)(++QF_isrNest_)); \
        QS_U8_(prio_); \
    QS_END_NOLOCK_()

#define QF_QS_ISR_EXIT(prio_) \
    QS_BEGIN_NOLOCK_(QS_QF_ISR_EXIT, 0, 0); \
        QS_TIME_(); \
        QS_U8_((uint8_t)(QF_isrNest_--)); \
        QS_U8_(prio_); \
    QS_END_NOLOCK_()

#define QF_QS_ACTION(act_)      (act_)

#else

#define QF_QS_INT_LOCK()        ((void)0)
#define QF_QS_INT_UNLOCK()      ((void)0)
#define QF_QS_ISR_ENTRY(prio_)  ((void)0)
#define QF_QS_ISR_EXIT(prio_)   ((void)0)
#define QF_QS_ACTION(act_)      ((void)0)

#endif                                                                // Q_SPY

//////////////////////////////////////////////////////////////////////////////
/// \page qf_rev QF/C++ Revision History
///
/// \section qf_3_2_00 Version 3.2.00 (Product Release)
/// Release date: Aug 08, 2006\n
///
/// -# In file qf.h changed the semantics of the QF::onIdle() callback.
/// This callback is now invoked with interrupts LOCKED from the
/// non-preemptive scheduler used in the "vanilla" QF ports to "bare metal"
/// target boards.<br>
/// <br>
/// The modification changes the responsibilities of QF::onIdle(), which now
/// MUST at least unlock interrupts. A failure to unlock interrupts in
/// QF::onIdle() will leave the interrupts locked all the time and would
/// prevent the application from running.<br>
/// <br>
/// Also, the signature of QF::onIdle() now depends on the interrupt locking
/// policy. In case of the "save and restore interrupt status" policy, the
/// QF::onIdle() callback takes the interrupt lock key as parameter (to be
/// able to unlock the interrups correctly).
/// -# In file qf.h used the macro Q_ROM to allocate constant objects
/// to ROM (\sa qep.h). Objects allocated to ROM are: the version strings, and
/// the lookup tables (QF::log2Lkup[], QF::pwr2Lkup[], QF::invPwr2Lkup[], and
/// QF::div8Lkup[].
/// -# Added new platform-independent header file qsched.h to provide the
/// interface to the simple non-preemptive scheduler used in the "vanilla"
/// ports of QF to "bare metal" targets. This header file is only applicable
/// to the "vanilla" ports.
/// -# Added new platform-independent implementation file qf_run.cpp to
/// implement the simple non-preemptive scheduler used in the "vanilla" ports
/// of QF to "bare metal" targets. This implementation file eliminates the
/// need for qf_port.cpp file in the "vanilla" ports of QF. Also, the
/// qf_run.cpp module should only be placed in the QF library in the vanilla
/// QF ports.
/// -# Simplified all "vanilla" ports of QF to use the common platform-
/// independent implementation provided in qf_run.cpp.
/// -# Updated QF::onIdle() callback in all examples of "vanilla" ports of QF
/// to unlock interrupts.
/// -# Updated the "QF/C++ Programmer's Manual".
///
///
/// \section qf_3_1_06 Version 3.1.06 (Product Release)
/// Release date: Jul 14, 2006\n
///
/// -# In file qf.h added function QF_stop() to be called from the application
/// code to stop the framework.
/// -# In file qf.h added callback function QF_cleanup() to be called from the
/// QF port to cleanup before exiting to the OS.
/// -# In file qf.h deprecated the function QF_exit().
///
///
/// \section qf_3_1_05 Version 3.1.05 (Product Release)
/// Release date: Feb 10, 2006\n
///
/// -# In file qf_act.cpp added the Revision History Doxygen comment, which
/// was previously in doxygen/qp.h
/// -# In file qf.h augmented comment for QF_run() to cover the case when QF
/// is used with QK.
/// -# In file qf.h added the extern declarations of ::QF_tickCtr_,
/// ::QF_intLockNest, and ::QF_isrNest_, which were previously declared
/// in qf_pkg.h.
/// -# In file qf.h added macros #QF_QS_INT_LOCK, #QF_QS_INT_UNLOCK(),
/// #QF_QS_ISR_ENTRY, and #QF_QS_ISR_EXIT, which were previously declared
/// in qs_port.h.
/// -# In file ports/linux/gcc/qf_port.h added extern uint8_t QF_running_.
/// -# In file qf/80x86/dos/tcpp101/l/qf_port.cpp replaced deprecated
/// QPSet::hasElements() to QPSet::isEmpty().
/// -# In file qf/80x86/linux/gcc/qf_port.cpp added QF::run()
/// -# In file qeq_init.cpp changed QS_OBJ(this) to QS_OBJ(qSto) to
/// consistently refer to a queue by the ring buffer object
/// -# In file qf_pkg.h removed extern ::QF_tickCtr_.
///
/// \section qf_3_1_04 Version 3.1.04 (Product Release)
/// Release date: Dec 11, 2005\n
///
/// -# In file qmpool.h changed the definition of the #QF_MPOOL_SIZ_SIZE macro
/// to remove the dependency on the #QF_EVENT_SIZ_SIZE. Macro
/// #QF_EVENT_SIZ_SIZE might not be defined by the time qmpool.h is included.
/// -# Added explicit definition of the configuration macro QF_EVENT_SIZ_SIZE
/// to all qf_port.h files.
/// -# Fixed a bug in function QMPool::init() (file qmp_init.cpp) by changing
/// the type of variable n from uint8_t to QMPoolCtr. The uint8_t data type
/// was failing for bigger block sizes.
/// -# Added the QF::onIdle() callback to qf.h
/// -# Improved comments in qpset.h
/// -# Corrected dependencies in the Makefile for QDPP example
/// (directory 80x86\dos\tcpp101\l and 80x86\dos\tcpp101\s)
/// -# Added Linux QF port to the standard QF/C++ distribution.
/// -# Released the "QF/C++ Programmer's Manual"
///
///
/// \section qf_3_1_03 Version 3.1.03 (Beta Release)
/// Release date: Nov 21, 2005\n
///
/// -# Added Doxygen documentation to the source code
/// -# Added running__ member to the QActive structure
/// -# Added QF_EVENT_SIZ_SIZE configuration macro and related data type
/// QEventSize. Made the following changes to the signatures:\n
/// void QF::poolInit(void *poolSto, uint32_t poolSize, QEventSize evtSize);\n
/// QEvent *QF::new_(QEventSize evtSize, QSignal sig);
///
///
/// \section qf_3_1_00 Version 3.1.00 (Snapshot Release)
/// Release date: Oct 10, 2005\n
///
/// -# Applied new directory structure desribed in
/// <A HREF="http://www.quantum-leaps.com/doc/AN_QP_Directory_Structure.pdf">
/// Application Note: QP Directory Structure</A>
/// -# Added <A HREF="http://www.quantum-leaps.com/products/qs.htm">
/// Quantum Spy</A> instrumentation.
///
///

//////////////////////////////////////////////////////////////////////////////
/// \defgroup qf Quantum Framework in C++ (QF/C++)
/// \image html logo_qf_TM.jpg
///
/// Quantum Frameowrk (QF) is a reusable event-driven application framework
/// for executing concurrent state machines specifically designed for real-
/// time embedded (RTE) systems. The use of QF generally simplifies the
/// design of event-driven software by allowing the application to be divided
/// into multiple active objects  that the framework manages. Active objects
// in QF are encapsulated tasks (each embedding a state machine and an event
/// queue) that communicate with one another asynchronously by sending and
/// receiving events. Within an active object, events are processed
/// sequentially in a run-to-completion (RTC) fashion, while QF encapsulates
/// all the details of thread-safe event exchange and queuing..
///
/// Most of QF/C++ is written in portable ANSI-C, with microprocessor-
/// specific, compiler-specific, or op-erating system-specific code kept to
/// a minimum for ease of portability. QF is designed to work with Quantum
/// Event Processor (QEP) and a Real Time Operating System (RTOS) of your
/// choice, or even with just "main+ISRs" configuration. The framework is
/// very compact, typically taking up about 4KB of code and data (including
/// the QEP). QF has been used in hundreds of event-driven applications
/// worldwide and has been originally described in Part 2 of the book
/// <A HREF="http://www.quantum-leaps.com/writings/book.htm">Practical
/// Statecharts in C/C++</A> by Miro Samek, CMP Books 2002.
///
/// \sa <A HREF="http://www.quantum-leaps.com/doc/QF_Cpp_Manual.pdf">
///      QF/C++ Programmer's Manual</A> \n
///      \ref qf_rev

#endif                                                                 // qf_h
