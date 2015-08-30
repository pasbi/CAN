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
    template<typename T>
    static T* create(const QString & classname)
    {
        if (!m_constructorMap->contains(classname))
        {
            return NULL;
        }
        else
        {
            return static_cast<T*>(m_constructorMap->value(classname)());
        }
    }
    static QString category(const QString & classname);
    static QStringList classnamesInCategory( const QString & category );
    static QString name(const QString &classname );

private:
    // pointers to hashmaps since they are required in a static
    // initialization and maybe constructor would not have been called
    // when creating them statically on stack.
    static QHash<QString, Creatable* (*)()>* m_constructorMap;
    static QHash<QString, QString>* m_categoryMap;
    static QHash<QString, QString>* m_inverseCategoryMap;
    static QHash<QString, const char*>* m_nameMap;
    template<typename T> friend struct Registerer;

};

template<typename T>
struct Registerer
{
    Registerer( const QString& className, const QString & category, const char* name )
    {
        if (!Creatable::m_categoryMap)          Creatable::m_categoryMap        = new QHash<QString, QString>();
        if (!Creatable::m_inverseCategoryMap)   Creatable::m_inverseCategoryMap = new QHash<QString, QString>();
        if (!Creatable::m_constructorMap)       Creatable::m_constructorMap     = new QHash<QString, Creatable* (*)()>();
        if (!Creatable::m_nameMap)              Creatable::m_nameMap            = new QHash<QString, const char*>();
        Creatable::m_constructorMap->insert( className, &createT<T> );
        Creatable::m_categoryMap->insert( className, category );
        Creatable::m_inverseCategoryMap->insertMulti( category, className );
        Creatable::m_nameMap->insertMulti( className, name );
    }
};

#define DECL_CREATABLE( CLASSNAME )         \
    static Registerer<CLASSNAME> reg;       \
public:                                     \
    static const QString TYPE;              \
    QString type() const { return TYPE; }   /* this will override `virtual QString type() const = 0` from base classes */

#define DEFN_CREATABLE_NAME( CLASSNAME, CATEGORY, NAME )                            \
    Registerer<CLASSNAME> CLASSNAME::reg(#CLASSNAME, #CATEGORY, NAME); \
    const QString CLASSNAME::TYPE = #CLASSNAME;

#define DEFN_CREATABLE( CLASSNAME, CATEGORY ) \
    Registerer<CLASSNAME> CLASSNAME::reg(#CLASSNAME, #CATEGORY, #CLASSNAME)

#endif // CREATABLE_H
