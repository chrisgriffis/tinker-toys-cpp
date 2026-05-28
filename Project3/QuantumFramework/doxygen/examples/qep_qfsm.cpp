class QBomb : public QFsm {
    uint8_t timeout_;                      // number of seconds till explosion
    uint8_t defuse_;                                 // the secret defuse code
    uint8_t code_;                            // the current defuse code entry

public:
    QBomb() : QFsm((QState)&QBomb::initial) {
    }

protected:
    static void initial(QBomb *me, QEvent const *e);
    static void setting(QBomb *me, QEvent const *e);
    static void timing(QBomb *me, QEvent const *e);
    static void blast(QBomb *me, QEvent const *e);
};
