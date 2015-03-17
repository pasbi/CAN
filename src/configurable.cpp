#include "configurable.h"
#include "global.h"
#include <QVariant>
#include <QJsonDocument>
#include "application.h"

Configurable::Configurable(const QString &prefix, const QString & caption ) :
    m_prefix(prefix),
    m_caption(caption)
{
    // do not restore configuration here.
    // at the time this code is executed, QApplication does not exist.
    // Hence, QApplication cannot provide applicationName or
    // organizationName for QSettings.
    // Restore Configuration is better done in Application constructor.
    //restoreConfiguration();
    registerer()->registerConfigurable(this);
}

Configurable::~Configurable()
{
}

void Configurable::addItem(const QString & key, const QVariant & defaultValue, const ConfigurationItemOptions & options)
{
    m_items.insert(key, ConfigurationItem(defaultValue, defaultValue, options));
}

void Configurable::setItem(const QString & key, const QVariant & newValue)
{
    assert( m_items.contains(key) );
    m_items[key].actualValue = newValue;
}

bool Configurable::contains(const QString & key) const
{
    return m_items.contains(key);
}

QVariant Configurable::item(const QString & key) const
{
    assert( m_items.contains(key) );
    return m_items[key].actualValue;
}

void Configurable::reset(const QString & key)
{
    assert( m_items.contains(key) );
    m_items[key].actualValue = m_items[key].resetValue;
}

/**
 * @brief apply sets the reset value to actual value.
 * @param key
 */
void Configurable::apply(const QString & key)
{
    assert( m_items.contains(key) );
    m_items[key].resetValue = m_items[key].actualValue;
}

/**
 * @brief setDefault sets the actual value to defaultValue.
 * @param key
 */
void Configurable::setDefault(const QString & key)
{
    assert( m_items.contains(key) );
    m_items[key].actualValue = m_items[key].defaultValue;
}

void Configurable::saveConfiguration() const
{
    QSettings settings;
    settings.beginGroup("Configurables");
    settings.beginGroup(m_prefix);
    for (const QString & key : m_items.keys() )
    {
        settings.setValue(key, m_items[key].actualValue);
    }
    settings.endGroup();
    settings.endGroup();
}

void Configurable::restoreConfiguration()
{
    QSettings settings;
    settings.beginGroup("Configurables");
    settings.beginGroup(m_prefix);
    QStringList keys = settings.allKeys();
    for (const QString & key : keys)
    {
        // only overwrite, dont create.
        setItem(key, settings.value(key));
        apply(key);
    }
    settings.endGroup();
    settings.endGroup();
}

void Configurable::ConfigurableRegisterer::registerConfigurable(Configurable *config)
{
    assert( !m_configurables.contains( config->prefix() ) );
    m_configurables.insert( config->prefix(), config );
}


void Configurable::saveAll()
{
    for (const Configurable* c : registerer()->configs().values())
    {
        c->saveConfiguration();
    }
}

void Configurable::restoreAll()
{
    for (Configurable* c : registerer()->configs().values())
    {
        c->restoreConfiguration();
    }
}






