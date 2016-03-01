#ifndef PEDANTICMAP_H
#define PEDANTICMAP_H

#include <QObject>
#include <QVariantMap>

template<class Key, class T>
class PedanticMap
{

public:
    explicit PedanticMap(const QList<Key>& keys) :
        m_keys(keys)
    {

    }

    QList<Key> keys() const
    {
        return m_keys;
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
    qDebug() << "save keys: " << map.m_keys;
    out << map.m_keys;
    out << map.m_map;
    return out;
}

template<class Key, class T>
QDataStream& operator >> (QDataStream& in, PedanticMap<Key, T>& map)
{
    qDebug() << "load keys: " << map.m_keys;
    in >> map.m_keys;
    in >> map.m_map;
    return in;
}

typedef PedanticMap<QString, QVariant> PedanticVariantMap;

#endif // PEDANTICMAP_H
