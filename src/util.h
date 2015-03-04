#ifndef UTIL_H
#define UTIL_H

#include <QAction>
#include <QMenu>
#include <functional>


// declares an enum and a function for stringificatoin.b
#define ENUM( NAME, X... ) \
    enum NAME { X };     \
    auto getNameOf_##NAME = [](NAME i) { return QString( #X ).split(QRegExp("\\s*,\\s*"))[i]; }

namespace Util
{
QAction* addAction( QMenu* menu, const QString & caption, std::function<void ()> do_action );

QStringList findAllFiles(const QString & dirname, bool findHidden = false );

}

#endif // UTIL_H
