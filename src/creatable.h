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
    static bool create(const QString & classname, void* &object);


private:
    static QMap<QString, Creatable* (*)()> m_constructorMap;
    template<typename T> friend struct Registerer;

};

template<typename T>
struct Registerer
{
    Registerer(QString className)
    {
        Creatable::m_constructorMap.insert(className, &createT<T>);
    }
};

#define DECL_CREATABLE( CLASSNAME ) \
    static Registerer<CLASSNAME> reg

#define DEFN_CREATABLE( CLASSNAME ) \
    Registerer<CLASSNAME> CLASSNAME::reg(#CLASSNAME)

#define CREATE(CLASSNAME, OBJECT) \
    Creatable::create(CLASSNAME, (void*&) OBJECT)

#endif // CREATABLE_H
