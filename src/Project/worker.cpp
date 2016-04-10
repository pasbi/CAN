
#include "worker.h"

Worker::Worker() :
    QObject(nullptr),
    m_error(false)
{
    m_isFinished = false;
}

bool Worker::isFinished() const
{
    return m_isFinished;
}

bool Worker::error() const
{
    return m_error;
}
