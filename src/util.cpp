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
}
