#ifndef UTIL_H
#define UTIL_H

#include <QAction>
#include <QMenu>
#include <functional>

#define initAction( name, associated_widget, text, tooltip, shortcut, menu, icon )                          \
    {                                                                                                       \
    name = new QAction( associated_widget );                                                                \
    associated_widget->addAction( name );                                                                   \
    name->setIcon(QIcon(icon));                                                                             \
    name->setText(text);                                                                                    \
    name->setShortcut( QKeySequence(shortcut) );                                                            \
    name->setShortcutContext( Qt::WidgetShortcut );                                                         \
    name->setToolTip(tooltip);                                                                              \
    connect( name, SIGNAL(triggered()), this, SLOT(my_on_##name##_triggered()) );                           \
    if (menu)                                                                                               \
    {                                                                                                       \
        ((QMenu*) menu)->addAction( name );                                                                 \
    }                                                                                                       \
    }

#define newAction( name, associated_widget, text, tooltip, shortcut, menu, icon )                           \
    {                                                                                                       \
    QAction* name = new QAction( associated_widget );                                                       \
    associated_widget->addAction( name );                                                                   \
    name->setIcon(QIcon(icon));                                                                             \
    name->setText(text);                                                                                    \
    name->setShortcut( QKeySequence(shortcut) );                                                            \
    name->setShortcutContext( Qt::WidgetShortcut );                                                         \
    name->setToolTip(tooltip);                                                                              \
    connect( name, SIGNAL(triggered()), this, SLOT(my_on_##name##_triggered()) );                           \
    if (menu)                                                                                               \
    {                                                                                                       \
        ((QMenu*) menu)->addAction( name );                                                                 \
    }                                                                                                       \
    }


namespace Util
{
QAction* addAction( QMenu* menu, const QString & caption, std::function<void ()> do_action );

QStringList findAllFiles(const QString & dirname, bool findHidden = false );
}

#endif // UTIL_H
