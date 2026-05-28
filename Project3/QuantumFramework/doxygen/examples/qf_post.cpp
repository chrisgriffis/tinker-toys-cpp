extern QActive *QDPP_table;

QSTATE Philosopher::hungry(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            pe = Q_NEW(TableEvt, HUNGRY_SIG);    // dynamically allocate event
            pe->philNum = me->num_;
            QDPP_table->postFIFO(pe);               // post the event directly
            return 0;
        }
        . . .
    }
    return (QSTATE)&QHsm::top;
}
