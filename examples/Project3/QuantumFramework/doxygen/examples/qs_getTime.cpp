#ifdef Q_SPY

QSTimeCtr QS::getTime(void) {
    return (QSTimeCtr)clock();
}

. . .

#endif // Q_SPY
