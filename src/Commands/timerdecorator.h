#ifndef TIMERDECORATOR_H
#define TIMERDECORATOR_H


class QTimer;
class TimerDecorator
{
protected:
    TimerDecorator();

    /**
     * @brief spanExpired returns whether the span has expired, starts the timer if it was expired.
     * @return
     */
    bool spanExpired();

private:
    static QTimer* m_timer;
};

#endif // TIMERDECORATOR_H
