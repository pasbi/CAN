#ifndef PUSHWORKER_H
#define PUSHWORKER_H

#include "worker.h"

#ifdef HAVE_LIBGIT
#include "git2.h"


class PushWorker : public Worker
{
    Q_OBJECT

public:
    PushWorker(git_remote* remote, git_strarray* refspecs, const git_push_options* options);
    void run();

private:
    git_remote* m_remote;
    git_strarray* m_refspecs;
    const git_push_options* m_options;
};

#endif

#endif // PUSHWORKER_H
