QSTATE Philosopher::eating(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            me->timeEvt_.postIn(me, EAT_TIME);      // arm one-shot time event
            return 0;
        }
        case TIMEOUT_SIG: {
            Q_TRAN(&Philosopher::thinking);
            return 0;
        }
        case Q_EXIT_SIG: {
            busyDelay();
            pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philNum = me->num_;
            QF::publish(pe);
            return 0;
        }
    }
    return (QSTATE)&QHsm::top;
}
