#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker();
    bool isFinished() const;
    bool error() const;

public slots:
    virtual void run() = 0;

protected:
    bool m_isFinished;
    bool m_error;
};

#endif // WORKER_H
