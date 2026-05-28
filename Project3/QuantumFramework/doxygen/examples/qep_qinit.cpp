// initial pseudostate of the QBomb FSM ......................................
void QBomb::initial(QBomb *me, QEvent const *e) {
    Q_REQUIRE(e != (QEvent const *)0);        // initialization event expected
    me->updateState("top-INIT");
    me->timeout_ = INIT_TIMEOUT;
    me->defuse_ = ((QBombInitEvt const *)e)->defuse;
    Q_INIT(&QBomb::setting);
}

// state handler function for the QCalc HSM ..................................
QSTATE QCalc::on(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        . . .
        case Q_INIT_SIG: {
            me->updateState("on-INIT");
            Q_INIT(&QCalc::ready);
            return (QSTATE)0;
        }
        . . .
    }
    return (QSTATE)&QHsm::top;
}
