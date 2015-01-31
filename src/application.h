#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "global.h"
#include "FileIndex/fileindex.h"

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char** argv);
    ~Application();

    void bark() { qDebug() << "bark"; }
    FileIndex & fileIndex() { return m_fileIndex; }

private:
    FileIndex m_fileIndex;

};

Application & app();

#endif // APPLICATION_H
