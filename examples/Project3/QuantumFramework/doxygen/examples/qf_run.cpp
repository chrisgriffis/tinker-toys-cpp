void QActive::run(void) {
    do {
        QEvent const *e;
        QACTIVE_GET_(this, e);                               // wait for event
        QF_ACTIVE_SUPER_::dispatch(e);   // dispatch to the active object's SM
        QF::gc(e);      // check if the event is garbage, and collect it if so
    } while (running_);

    unsubscribeAll();                          // unsubscribe from all signals
    QF::remove_(this);            // remove this object from any subscriptions
}
