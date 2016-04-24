#include "cloneworker.h"
#include "githandler.h"
#include "global.h"

CloneWorker::CloneWorker(git_repository*& repository, const QString& url, const QString& path, const git_clone_options *options ) :
    m_repository(repository),
    m_url(url),
    m_path(path),
    m_options(options)
{

}

#define CSTR(qstring) (qstring.toStdString().c_str())

void CloneWorker::run()
{
    int error = git_clone( &m_repository, CSTR(m_url), CSTR(m_path), m_options);
    m_error = (error != 0);
    m_isFinished = true;
}

