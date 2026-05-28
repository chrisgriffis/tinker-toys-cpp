class QCalc : public QHsm {                // Quantum Calculator state machine
private:
    double operand1_;
    double operand2_;
    char display_[DISP_WIDTH + 1];
    uint8_t len_;
    uint8_t opKey_;

public:
    QCalc() : QHsm((QState)&QCalc::initial) {                          // ctor
    }

protected:
    static void initial(QCalc *me, QEvent const *e);
    static QSTATE on(QCalc *me, QEvent const *e);
    static QSTATE error(QCalc *me, QEvent const *e);
    static QSTATE ready(QCalc *me, QEvent const *e);
    static QSTATE result(QCalc *me, QEvent const *e);
    static QSTATE begin(QCalc *me, QEvent const *e);
    static QSTATE negated1(QCalc *me, QEvent const *e);
    static QSTATE operand1(QCalc *me, QEvent const *e);
    static QSTATE zero1(QCalc *me, QEvent const *e);
    static QSTATE int1(QCalc *me, QEvent const *e);
    static QSTATE frac1(QCalc *me, QEvent const *e);
    static QSTATE opEntered(QCalc *me, QEvent const *e);
    static QSTATE negated2(QCalc *me, QEvent const *e);
    static QSTATE operand2(QCalc *me, QEvent const *e);
    static QSTATE zero2(QCalc *me, QEvent const *e);
    static QSTATE int2(QCalc *me, QEvent const *e);
    static QSTATE frac2(QCalc *me, QEvent const *e);
};
