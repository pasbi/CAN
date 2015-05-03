#ifndef UTIL_H
#define UTIL_H

#include <QAction>
#include <QMenu>
#include <functional>


namespace Util
{
QAction* addAction( QMenu* menu, const QString & caption, std::function<void ()> do_action );

QStringList findAllFiles(const QString & dirname, bool findHidden = false );
}

#endif // UTIL_H
