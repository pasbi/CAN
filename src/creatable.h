#ifndef CREATABLE_H
#define CREATABLE_H

#include <QString>
#include <QHash>
#include <QStringList>

class Creatable;
template<typename T>
Creatable* createT()
{
    return new T();
}

/**
 * @brief The Creatable class is the base class for all Classes, that shall be dynamically
 *  creatable by classname. note that Creatable does not need to derive from QObject
 */
class Creatable
{
public:
    Creatable();

    /**
     * @brief create create an object with given classname with default constructor.
     * @param classname
     * @param object
     * @return success
     */
    static bool create(const QString & classname, Creatable *&object);
    static QString category(const QString & classname);
    static QStringList classnamesInCategory( const QString & category );
    static QString name( const QString & classname );


private:
    static QHash<QString, Creatable* (*)()> m_constructorMap;
    static QHash<QString, QString> m_categoryMap;
    static QHash<QString, QString> m_inverseCategoryMap;
    static QHash<QString, QString> m_nameMap;
    template<typename T> friend struct Registerer;

};

template<typename T>
struct Registerer
{
    Registerer( const QString & className, const QString & category, const QString & name )
    {
        Creatable::m_constructorMap.insert( className, &createT<T> );
        Creatable::m_categoryMap.insert( className, category );
        Creatable::m_inverseCategoryMap.insertMulti( category, className );
        Creatable::m_inverseCategoryMap.insertMulti( className, name );
    }
};

#define DECL_CREATABLE( CLASSNAME ) \
    static Registerer<CLASSNAME> reg

#define DEFN_CREATABLE_NAME( CLASSNAME, CATEGORY, NAME ) \
    Registerer<CLASSNAME> CLASSNAME::reg(#CLASSNAME, #CATEGORY, QObject::tr(NAME))

#define DEFN_CREATABLE( CLASSNAME, CATEGORY ) \
    Registerer<CLASSNAME> CLASSNAME::reg(#CLASSNAME, #CATEGORY, #CLASSNAME)

#define CREATE(CLASSNAME, OBJECT) \
    Creatable::create(CLASSNAME, (Creatable*&) OBJECT)

#endif // CREATABLE_H
