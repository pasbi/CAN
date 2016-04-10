//#include "githandlerdetached.h"

//#include <QThread>

//#include "global.h"
//#include "pushworker.h"
//#include "cloneworker.h"

//#define FATAL_SHUTDOWN_WORKER_FIRST qCritical() << "Shut down other worker (" << m_worker << ") first."; abort();

//GitHandlerDetached::GitHandlerDetached() :
//    m_thread(new QThread(this)),
//    m_worker(nullptr),
//    m_isFinished(true)
//{
//}

//GitHandlerDetached::~GitHandlerDetached()
//{
//    if (m_worker)
//    {
//        FATAL_SHUTDOWN_WORKER_FIRST
//    }
//}

//bool GitHandlerDetached::push(git_repository* repository, const QString &username, const QString &password)
//{
////    qDebug() << "detached push";
////    if (m_worker)
////    {
////        FATAL_SHUTDOWN_WORKER_FIRST
////        return false;
////    }

////    m_worker = new PushWorker(this, repository, username, password);
////    m_worker->moveToThread(m_thread);
////    connect(m_worker, SIGNAL(done()), this, SLOT(onWorkerFinished()));
////    m_isFinished = false;

////    qDebug() << "start push...";
////    m_worker->run();

////    return true;
////}

//bool GitHandlerDetached::clone(git_repository *&repository, const QString& url, const QString& path, const QString &username, const QString &password)
//{
////    if (m_worker)
////    {
////        FATAL_SHUTDOWN_WORKER_FIRST
////        return false;
////    }

////    m_worker = new CloneWorker(this, repository, url, path, username, password);
////    m_worker->moveToThread(m_thread);
////    connect(m_worker, SIGNAL(done()), this, SLOT(onWorkerFinished()));
////    m_isFinished = false;

////    m_worker->run();

////    return true;
//}

//bool GitHandlerDetached::isFinished() const
//{
//    return m_isFinished;
//}

//void GitHandlerDetached::onWorkerFinished()
//{
//    m_isFinished = true;
//    delete m_worker;
//    m_worker = nullptr;
//}

//void GitHandlerDetached::abort()
//{
//    if (m_worker)
//    {
//        m_worker-
//        m_error = true;
//    }
//}
