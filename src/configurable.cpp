#include "configurable.h"
#include "global.h"
#include <QVariant>
#include <QJsonDocument>
#include "application.h"

Configurable::Configurable(const QString &prefix, const char* caption ) :
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

void Configurable::addItem(const QString & key, const char *caption, const QVariant & defaultValue, const ConfigurationItemOptions & options)
{
    m_itemKeys << key;
    m_items.insert(key, new ConfigurationItem(caption, defaultValue, defaultValue, options));
}

bool Configurable::contains(const QString & key) const
{
    return m_items.contains(key);
}

void Configurable::reset()
{
    for (const QString & key : m_items.keys())
    {
        if (item(key))
        {
            m_items[key]->reset();
        }
        else
        {
            qWarning() << "cannot reset " << key;
        }
    }
}

void Configurable::apply()
{
    for (const QString & key : m_items.keys())
    {
        if (item(key))
        {
            m_items[key]->apply();
        }
        else
        {
            qWarning() << "cannot apply " << key;
        }
    }
}

void Configurable::restore()
{
    for (const QString & key : m_items.keys())
    {
        if (item(key))
        {
            m_items[key]->restore();
        }
        else
        {
            qWarning() << "cannot restore " << key;
        }
    }
}

void Configurable::saveConfiguration() const
{
    QSettings settings;
    settings.beginGroup("Configurables");
    settings.beginGroup(m_prefix);
    for (const QString & key : m_items.keys() )
    {
        if (m_items[key])
        {
            settings.setValue(key, m_items[key]->actualValue());
        }
        else
        {
            qWarning() << "cannot save " << key;
        }
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
        if (item(key))
        {
            set( key, settings.value(key) );
        }
        else
        {
            qWarning() << "drop key " << key;
        }
    }
    apply();
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

void ConfigurationItem::setDefaultValue(const QVariant &value)
{
    assert( !m_defaultValue.isValid() );
    m_defaultValue = value;
}

void ConfigurationItem::set(const QVariant &value)
{
    if ( m_actualValue != value )
    {
        m_actualValue = value;
        emit valueChanged( value );
    }
}

void ConfigurationItem::reset()
{
    set( m_resetValue );
}

void Configurable::set(const QString &key, const QVariant &value)
{
    item(key)->set(value);
}

ConfigurationItem* Configurable::item(const QString & key)
{
    if (m_items.contains(key))
    {
        return m_items.value( key );
    }
    else
    {
        return NULL;
    }
}

const ConfigurationItem* Configurable::item(const QString & key) const
{
    return m_items.value( key, NULL );
}

QString Configurable::caption() const
{
    return QApplication::translate("Configurable", m_caption);
}

QString ConfigurationItem::caption() const
{
    return QApplication::translate("ConfigurableItem", m_caption);
}

QVariant Configurable::value( const QString & key ) const
{
    return item(key)->actualValue();
}

QVariant Configurable::operator[] ( const QString & key ) const
{
    return item(key)->actualValue();
}








