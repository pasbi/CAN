#ifndef UTIL_H
#define UTIL_H

#include <QObject>

// since I experienced some problems on windows with Q_ASSERT, it is turned of there.
#ifdef assert   // do not use third party asserts.
#undef assert
#endif

#if defined __linux && defined QT_DEBUG
#define assert( assertion ) Q_ASSERT( assertion )
#else   // if we are not on linux or not in debug mode, pass the expression through check function.
#define assert( assertion ) check(assertion, #assertion, __FILE__, __LINE__)
#endif


// cast and assert that the cast succeeded.
template< class T, class S > T qobject_assert_cast( S s )
{
    T t = qobject_cast<T>(s);
    assert( s == t );
    return t;
}
template< class T, class S > T dynamic_assert_cast( S s )
{
    T t = dynamic_cast<T>( s );
    assert( s == t );
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
    QString replaceDiacritics(QString string );
}


#endif // UTIL_H
