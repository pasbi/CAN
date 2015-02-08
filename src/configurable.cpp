#include "configurable.h"
#include "global.h"
#include <QVariant>
#include <QJsonDocument>

QMap<QString, Configurable*> Configurable::m_configurationObjects;

Configurable::Configurable(const QString &prefix, const QString &caption) :
    m_prefix(prefix),
    m_caption(caption)
{
    restoreConfiguration();
    registerConfigurable(this);
}

Configurable::~Configurable()
{
    saveConfiguration();
}

void Configurable::addItem(const QString & key, const QString & actualValue, const QString & defaultValue )
{
    if (expectKeyToExist(key, false))
        return;

    m_items.insert(key, ConfigurationItem(actualValue, actualValue, defaultValue));
}

void Configurable::setItem(const QString & key, const QString & newValue)
{
    if (expectKeyToExist(key, true))
        return;

    m_items[key].m_actualValue = newValue;
}

bool Configurable::contains(const QString & key) const
{
    if (expectKeyToExist(key, true))
        return false;

    return m_items.contains(key);
}

QString Configurable::item(const QString & key) const
{
    if (expectKeyToExist(key, true))
        return QString();

    return m_items[key].m_actualValue;
}

void Configurable::reset(const QString & key)
{
    if (expectKeyToExist(key, true))
        return;

    m_items[key].m_actualValue = m_items[key].m_resetValue;
}

/**
 * @brief apply sets the reset value to actual value.
 * @param key
 */
void Configurable::apply(const QString & key)
{
    if (expectKeyToExist(key, true))
        return;

    m_items[key].m_resetValue = m_items[key].m_actualValue;
}

/**
 * @brief setDefault sets the actual value to defaultValue.
 * @param key
 */
void Configurable::setDefault(const QString & key)
{
    if (expectKeyToExist(key, true))
        return;

    m_items[key].m_actualValue = m_items[key].m_defaultValue;
}

bool Configurable::expectKeyToExist(const QString & key, const bool exist) const
{
    if (contains(key) == exist)
    {
        return true;
    }
    else
    {
        if (exist)
        {
            WARNING << "Configuration " << this << " does not contain key " << key << ". Do nothing now.";
        }
        else
        {
            WARNING << "Configuration " << this << " already contains a key " << key << ". Overwite it now.";
        }
        return false;
    }
}

void Configurable::saveConfiguration() const
{
    QJsonObject json;

    for (const QString & key : m_items.keys() )
    {
        json.insert(key, m_items[key].toJsonObject());
    }

    QSettings settings;
    settings.beginGroup("Configurables");
    settings.setValue(m_prefix, QJsonDocument(json).toBinaryData());
    settings.endGroup();
}

void Configurable::restoreConfiguration()
{
    QSettings settings;
    settings.beginGroup("Configurables");
    QVariant variant = settings.value(m_prefix);
    settings.endGroup();

    if (variant.canConvert<QByteArray>())
    {
        QJsonObject json = QJsonDocument::fromBinaryData( variant.toByteArray() ).object();
        for (const QString & key : json.keys())
        {
            m_items[key] = ConfigurationItem( json[key].toObject() );
        }
    }
    else
    {
        WARNING << "Invalid variant type " << variant.type();
    }
}

bool Configurable::registerConfigurable(Configurable* conf)
{
    if (m_configurationObjects.keys().contains(conf->m_prefix))
    {
        WARNING << "Prefix collision <" << conf->m_prefix << "]";
        return false;
    }
    if (m_configurationObjects.values().contains(conf))
    {
        WARNING << "ConfigurableList already contains " << conf;
        return false;
    }

    m_configurationObjects.insert(conf->m_prefix, conf);
    return true;
}

bool Configurable::unregisterConfigurable(Configurable* conf)
{
    if (!m_configurationObjects.keys().contains(conf->m_prefix))
    {
        WARNING << "Cannot remove " << conf << " from ConfigurableList since it is not contained.";
        return false;
    }

    m_configurationObjects.remove(conf->m_prefix);
    return true;
}

void Configurable::saveAll()
{
    for (const Configurable* c : m_configurationObjects.values())
    {
        c->saveConfiguration();
    }
}

void Configurable::restoreAll()
{
    for (Configurable* c : m_configurationObjects.values())
    {
        c->restoreConfiguration();
    }
}

void Configurable::setHiddenItem( const QString & key, const QVariant & data ) const
{
    QSettings settings;
    settings.beginGroup("Configurable");
    settings.beginGroup(m_prefix);
    settings.setValue(key, data);
    settings.endGroup();
    settings.endGroup();
}

QVariant Configurable::hiddenItem( const QString & key ) const
{
    QSettings settings;
    settings.beginGroup("Configurable");
    settings.beginGroup(m_prefix);
    QVariant v = settings.value(key);
    settings.endGroup();
    settings.endGroup();
    return v;
}















