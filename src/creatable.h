#ifndef CREATABLE_H
#define CREATABLE_H

#include <QString>
#include <QMap>

class Creatable;
template<typename T>
Creatable* createT()
{
    return new T();
}

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


private:
    static QMap<QString, Creatable* (*)()> m_constructorMap;
    static QMap<QString, QString> m_categoryMap;
    template<typename T> friend struct Registerer;

};

template<typename T>
struct Registerer
{
    Registerer(const QString & className, const QString & category)
    {
        Creatable::m_constructorMap.insert(className, &createT<T>);
        Creatable::m_categoryMap.insert(className, category);
    }
};

#define DECL_CREATABLE( CLASSNAME ) \
    static Registerer<CLASSNAME> reg

#define DEFN_CREATABLE( CLASSNAME, CATEGORY ) \
    Registerer<CLASSNAME> CLASSNAME::reg(#CLASSNAME, #CATEGORY)

#define CREATE(CLASSNAME, OBJECT) \
    Creatable::create(CLASSNAME, (Creatable*&) OBJECT)

#endif // CREATABLE_H
