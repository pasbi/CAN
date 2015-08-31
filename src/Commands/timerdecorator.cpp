#include "timerdecorator.h"

#include <QTimer>

QTimer* TimerDecorator::m_timer = nullptr;

TimerDecorator::TimerDecorator()
{
    if (!m_timer)
    {
        m_timer = new QTimer();
        m_timer->setInterval(1000);
        m_timer->setSingleShot(true);
        m_timer->start();
    }
}

bool TimerDecorator::spanExpired()
{
    if (m_timer->isActive())
    {
        return false;
    }
    else
    {
        m_timer->start();
        return true;
    }
}
