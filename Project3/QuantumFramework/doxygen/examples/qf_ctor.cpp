class Philosopher : public QActive {
    . . .
public:
    Philosopher::Philosopher()                   // public default constructor
    : QActive((QState)&Philosopher::initial),
      timeEvt_(TIMEOUT_SIG)
    {}
    . . .
};
