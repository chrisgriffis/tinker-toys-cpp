class Philosopher : public QActive {
private:
    uint8_t num_;                               // number of this philosopher
    QTimeEvt timeEvt_;                        // to timeout thining or eating

public:
    Philosopher::Philosopher()
    : QActive((QState)&Philosopher::initial),
      timeEvt_(TIMEOUT_SIG)
    {}

protected:
    static void initial(Philosopher *me, QEvent const *e);
    static QSTATE thinking(Philosopher *me, QEvent const *e);
    static QSTATE hungry(Philosopher *me, QEvent const *e);
    static QSTATE eating(Philosopher *me, QEvent const *e);
};
