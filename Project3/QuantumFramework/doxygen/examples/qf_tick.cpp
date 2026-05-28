static void interrupt tmrISR(...) {
    QF_INT_UNLOCK(ignore);                                // unlock interrupts
    QF::tick();                                //<-- process the QF clock tick
    QF_INT_LOCK(ignore);                              // lock interrupts again
    geninterrupt(SPARE_VECTOR);        // invoke the DOS timer ISR, see NOTE01
}
