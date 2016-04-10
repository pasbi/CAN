#include "pushworker.h"
#include "githandler.h"
#include "global.h"

PushWorker::PushWorker(git_remote *remote, git_strarray *refspecs, const git_push_options *options) :
    m_remote(remote),
    m_refspecs(refspecs),
    m_options(options)
{

}

void PushWorker::run()
{
    m_error = (git_remote_push( m_remote, m_refspecs, m_options) != 0);
    m_isFinished = true;
}
