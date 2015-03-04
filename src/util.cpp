#include "util.h"
#include <QDir>
#include "global.h"

namespace Util
{
QAction* addAction( QMenu* menu, const QString & caption, std::function<void ()> do_action )
{
    QAction* action = new QAction(caption, menu);
    menu->addAction(action);
    QObject::connect(action, &QAction::triggered, do_action );
    return action;
}

QStringList findAllFiles(const QString & dirname , bool findHidden)
{
    QStringList files;

    QDir::Filters fileFilter = QDir::Files;
    QDir::Filters dirFilter = QDir::Dirs | QDir::NoDotAndDotDot;

    if (findHidden)
    {
        fileFilter |= QDir::Hidden;
        dirFilter |= QDir::Hidden | QDir::NoDotAndDotDot;
    }

    QDir dir(dirname);
    for (const QString & filename : dir.entryList( fileFilter ))
    {
        files.append( dir.absoluteFilePath(filename) );
    }

    for (const QString & dirname : dir.entryList( dirFilter ))
    {
        QString absolute = dir.absoluteFilePath(dirname);
        files.append( findAllFiles( absolute, findHidden ) );
    }

    return files;
}
}
