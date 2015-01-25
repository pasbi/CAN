#ifndef UTIL_H
#define UTIL_H

#include <QAction>
#include <QMenu>
#include <functional>

namespace Util
{
QAction* addAction( QMenu* menu, const QString & caption, std::function<void ()> do_action );

}

#endif // UTIL_H
