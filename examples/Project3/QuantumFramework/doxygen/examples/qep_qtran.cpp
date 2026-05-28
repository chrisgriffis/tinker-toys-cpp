// state handler function for the QBomb FSM ..................................
void QBomb::setting(QBomb *me, QEvent const *e) {
    switch (e->sig) {
        . . .
        case ARM_SIG: {
            Q_TRAN(&QBomb::timing);
            break;
        }
    }
}

// state handler function for the QCalc HSM ..................................
QSTATE QCalc::begin(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        . . .
        case OPER_SIG: {
            if (((QCalcEvt *)e)->keyId == KEY_MINUS) {
                Q_TRAN(&QCalc::negated1);
                return (QSTATE)0;                             // event handled
            }
            break;
        }
    }
    return (QSTATE)&QCalc::ready;
}

