#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include "global.h"

// cast and Q_ASSERT that the cast succeeded.
template< class T, class S > T qobject_assert_cast( S s )
{
    T t = qobject_cast<T>(s);
    Q_ASSERT( s == t );
    return t;
}
template< class T, class S > T dynamic_assert_cast( S s )
{
    T t = dynamic_cast<T>( s );
    Q_ASSERT( s == t );
    return t;
}

#define initAction( name, associated_widget, text, tooltip, shortcut, menu, icon )                          \
    {                                                                                                       \
    m_##name = new QAction( associated_widget );                                                            \
    associated_widget->addAction( m_##name );                                                               \
    m_##name->setIcon(QIcon(icon));                                                                         \
    m_##name->setText(text);                                                                                \
    m_##name->setShortcut( QKeySequence(shortcut) );                                                        \
    m_##name->setShortcutContext( Qt::WidgetShortcut );                                                     \
    m_##name->setToolTip(tooltip);                                                                          \
    connect( m_##name, SIGNAL(triggered()), this, SLOT(my_on_##name##_triggered()) );                       \
    if (menu)                                                                                               \
    {                                                                                                       \
        ((QMenu*) menu)->addAction( m_##name );                                                             \
    }                                                                                                       \
    }

namespace Util
{
    QString diacriticLetters();
    QStringList nonDiacriticLetters();
    QString replaceDiacritics(QString string);
}


#endif // UTIL_H
