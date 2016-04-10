#ifndef CLONEWORKER_H
#define CLONEWORKER_H

#include "worker.h"
#include "git2.h"

class CloneWorker : public Worker
{
public:
    CloneWorker(git_repository*& repository, const QString& url, const QString& path, const git_clone_options* options );

public slots:
    void run();

private:
    git_repository* & m_repository;
    QString m_url;
    QString m_path;
    const git_clone_options* m_options;
};

#endif // CLONEWORKER_H
