#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QVariantMap>
#include <QDebug>

template<class Key, class T>
class SortedMap
{
public:
    SortedMap() {}

    void append(const Key& key, const T& value)
    {
        m_keys.append(key);
        m_map.insert(key, value);
    }

    QList<Key> keys() const
    {
        return m_keys;
    }

    T value(const QString& key) const
    {
        return m_map(key);
    }

private:
    QList<Key> m_keys;
    QMap<Key, T> m_map;
};


template<class Key, class T>
class PedanticMap
{

public:
    explicit PedanticMap(const QList<Key>& keys = QList<Key>()) :
        m_keys(keys)
    {

    }

    QList<Key> keys() const
    {
        return m_keys;
    }

    void addKey(const Key& key)
    {
        Q_ASSERT(!m_keys.contains(key));
        m_keys.append(key);
    }

    void set(const Key& key, const T& value)
    {
        Q_ASSERT(m_keys.contains(key));
        m_map.insert(key, value);
    }

    T operator[](const Key& key) const
    {
        Q_ASSERT(m_keys.contains(key));
        return m_map[key];
    }

    T& operator[](const Key& key)
    {
        Q_ASSERT(m_keys.contains(key));
        return m_map[key];
    }

    /**
     * @brief take takes the key from the map, but it will remain as key.
     * @param key
     */
    void take(const Key& key)
    {
        Q_ASSERT(m_keys.contains(key));
        m_map.remove(key);
    }

private:
    QMap<Key, T> m_map;
    QList<Key> m_keys;

    template<class FKey, class FT> friend QDataStream& operator << (QDataStream& out, const PedanticMap<FKey, FT>& map);
    template<class FKey, class FT> friend QDataStream& operator >> (QDataStream& in,        PedanticMap<FKey, FT>& map);


};

template<class Key, class T>
QDataStream& operator << (QDataStream& out, const PedanticMap<Key, T>& map)
{
    out << map.m_keys;
    out << map.m_map;
    return out;
}

template<class Key, class T>
QDataStream& operator >> (QDataStream& in, PedanticMap<Key, T>& map)
{
    QStringList keys;
    in >> keys; //TODO unused value
    in >> map.m_map;

    //TODO theoretically, this check is not required. See Q_ASSERT below.
    if (map.keys().toSet().size() != map.keys().length())
    {
        map.m_keys.clear();
        map.m_map.clear();
    }

    // map.m_keys shall be unique
    Q_ASSERT (map.keys().toSet().size() == map.keys().length());

    return in;
}

template <class Key, class T>
inline QDebug operator<<(QDebug debug, const PedanticMap<Key, T> &map)
{
    const bool oldSetting = debug.autoInsertSpaces();
    debug.nospace() << "PedanticMap(";
    for (const Key& key : map.keys())
    {
        debug << "(" << key << ", " << map[key] << ")";
    }
    debug << ')';
    debug.setAutoInsertSpaces(oldSetting);
    return debug.maybeSpace();
}

typedef PedanticMap<QString, QVariant> PedanticVariantMap;

#endif // MAP_H
