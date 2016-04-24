#include "worker.h"
#include "git2.h"

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

void Worker::lookForErrors()
{
    const git_error* e = giterr_last();
    if (e)
    {
        emit error(e->klass, e->message);
        giterr_clear();
        giterr_set_oom();
    }
}
