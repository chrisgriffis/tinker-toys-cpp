// allocate storage for active objects, event queues, event pools,
// subscriber lists, and stacks.
static Philosopher l_philo[N];                 // N Philosopher active objects
static Table l_table;                               // the Table active object
static KbdMgr l_kbdMgr;                            // the KbdMgr active object

static QEvent const *l_kbdMgrQueueSto[2];
static QEvent const *l_tableQueueSto[N];
static QEvent const *l_philQueueSto[N][10];
static TableEvt l_regPoolSto[N + 20];
static QSubscrList l_subscrSto[MAX_PUB_SIG];

static int l_philosopherStk[N][256];            // stacks for the philosophers
static int l_tableStk[256];               // stack for the Table active object
static int l_kbdMgrStk[256];             // stack for the KbdMgr active object

//............................................................................
int main(int argc, char *argv[]) {
    QF::init();       // initialize the framework and the underlying RT kernel

    QF::psInit(l_subscrSto, Q_DIM(l_subscrSto));     // init publish-subscribe
                                                  // initialize event pools...
    QF::poolInit((QEvent *)l_regPoolSto, sizeof(l_regPoolSto),
                 sizeof(l_regPoolSto[0]));
                                                    // start active objects...
    uint8_t n;
    for (n = 0; n < N; ++n) {
        l_philo[n].start((uint8_t)(n*10 + 1),
                         l_philQueueSto[n], Q_DIM(l_philQueueSto[n]),
                         l_philosopherStk[n], sizeof(l_philosopherStk[n]));
    }

    l_table.start((uint8_t)(N*10 + 1),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  l_tableStk, sizeof(l_tableStk));

    l_kbdMgr.start((uint8_t)((N + 1)*10 + 1),
                   l_kbdMgrQueueSto, Q_DIM(l_kbdMgrQueueSto),
                   l_kbdMgrStk, sizeof(l_kbdMgrStk));

    QF::run();                                       // run the QF application

    return 0;
}
