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

    T operator[](int i) const
    {
        return m_map[m_keys[i]];
    }

    T& operator[](int i)
    {
        return m_map[m_keys[i]];
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
    in >> map.m_keys;
    in >> map.m_map;
    return in;
}

typedef PedanticMap<QString, QVariant> PedanticVariantMap;

#endif // PEDANTICMAP_H
