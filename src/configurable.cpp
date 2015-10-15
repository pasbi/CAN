#include "configurable.h"
#include "global.h"
#include <QVariant>
#include <QJsonDocument>
#include "application.h"

QSettings* Configurable::m_settings = nullptr;

Configurable::Configurable(const QString &prefix, const QString & caption ) :
    m_prefix(prefix),
    m_caption(caption)
{
    // do not restore Configurable here.
    // at the time this code is executed, QApplication does not exist.
    // Hence, QApplication cannot provide applicationName or
    // organizationName for QSettings.
    // Restore Configurable is better done in Application constructor.
    //restoreConfigurable();
    registerer()->registerConfigurable(this);
}

Configurable::~Configurable()
{
    qDeleteAll( m_items.values() );
    if (m_settings)
    {
        m_settings->sync();
    }
}

QSettings* Configurable::settings()
{
    if (!m_settings)
    {
        m_settings = new QSettings();
    }
    return m_settings;
}

void Configurable::addItem( const QString &                 key,
                            const QString &                 caption,
                            const QString &                 help,
                            const QVariant &                defaultValue,
                            const QString &                 switchItemKey,
                            const ConfigurableItemOptions & options)
{
    m_itemKeys << key;
    m_items.insert(key, new ConfigurableItem( this, caption, help, defaultValue, defaultValue, switchItemKey, options ));
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

void Configurable::saveConfigurable() const
{
    settings()->beginGroup("Configurables");
    settings()->beginGroup(m_prefix);
    qDebug() << "write " << m_prefix;
    for (const QString & key : m_items.keys() )
    {
        qDebug() << "    write " << key;
        if (m_items[key])
        {
            qDebug() << "             write " << settings() << key << m_items[key]->actualValue();
            settings()->setValue(key, m_items[key]->actualValue());
        }
        else
        {
            qWarning() << "cannot save " << key;
        }
    }
    settings()->endGroup();
    settings()->endGroup();
}

void Configurable::restoreConfigurable()
{
    QSettings* settings = Configurable::settings();
    settings->beginGroup("Configurables");
    settings->beginGroup(m_prefix);
    QStringList keys = settings->allKeys();
    qDebug() << "restore " << m_prefix;
    for (const QString & key : keys)
    {
        qDebug() << "      restore " << key;
        // only overwrite, dont create.
        if (item(key))
        {
            qDebug() << "             restore " << settings << key << settings->value(key);
            set( key, settings->value(key) );
        }
        else
        {
            qWarning() << "drop key " << key;
        }
    }
    apply();
    settings->endGroup();
    settings->endGroup();
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
        c->saveConfigurable();
    }
}

void Configurable::restoreAll()
{
    for (Configurable* c : registerer()->configs().values())
    {
        c->restoreConfigurable();
    }
}

void ConfigurableItem::setDefaultValue(const QVariant &value)
{
    assert( !m_defaultValue.isValid() );
    m_defaultValue = value;
}

void ConfigurableItem::set(const QVariant &value)
{
    if ( m_actualValue != value )
    {
        m_actualValue = value;
        emit valueChanged( value );
    }
}

void ConfigurableItem::apply()
{
    m_resetValue = m_actualValue;
    ConfigurableItem* switchItem = getSwitchItem();
    if (switchItem)
    {
        switchItem->apply();
    }
}

void ConfigurableItem::reset()
{
    set( m_resetValue );
    ConfigurableItem* switchItem = getSwitchItem();
    if (switchItem)
    {
        switchItem->reset();
    }
}

void ConfigurableItem::restore()
{
    set( defaultValue() );
    ConfigurableItem* switchItem = getSwitchItem();
    if (switchItem)
    {
        switchItem->restore();
    }
}

ConfigurableItem* ConfigurableItem::copy() const
{
    ConfigurableItem* c = new ConfigurableItem();
    c->m_actualValue = this->m_actualValue;
    c->m_defaultValue = this->m_defaultValue;
    c->m_resetValue = this->m_resetValue;

    c->m_caption = this->m_caption;
    c->m_configurable = this->m_configurable;
    c->m_help = this->m_help;
    c->m_options = this->m_options;
    c->m_switchItemKey = this->m_switchItemKey;

    return c;
}

void Configurable::set(const QString &key, const QVariant &value)
{
    item(key)->set(value);
}

ConfigurableItem* Configurable::item(const QString & key)
{
    if (m_items.contains(key))
    {
        return m_items.value( key );
    }
    else
    {
        return nullptr;
    }
}

const ConfigurableItem* Configurable::item(const QString & key) const
{
    return m_items.value( key, nullptr );
}

QString Configurable::caption() const
{
    return QApplication::translate("Configurable", m_caption.toStdString().c_str());
}

QString ConfigurableItem::caption() const
{
    return QApplication::translate("ConfigurableItem", m_caption.toStdString().c_str());
}

QString ConfigurableItem::help() const
{
    return QApplication::translate("ConfigurableItem", m_help.toStdString().c_str());
}

QVariant Configurable::value( const QString & key ) const
{
    return item(key)->actualValue();
}

QVariant Configurable::operator[] ( const QString & key ) const
{
    return item(key)->actualValue();
}

QVariant ConfigurableItem::defaultValue() const
{
    if (m_defaultValue.type() == QVariant::String)
    {
        return QApplication::translate("ConfigurableItem", m_defaultValue.toString().toStdString().c_str());
    }
    else
    {
        return m_defaultValue;
    }
}

ConfigurableItem* ConfigurableItem::getSwitchItem() const
{
    ConfigurableItem* switchItem = m_configurable->item( m_switchItemKey );
    return switchItem;
}

Configurable& Configurable::operator =(const Configurable &other)
{
    qDeleteAll( m_items.values() );
    m_items.clear();

    m_itemKeys = other.m_itemKeys;
    for (const QString& key : m_itemKeys)
    {
        ConfigurableItem* item = other.m_items[key]->copy();
        m_items.insert( key, item );
    }

    return *this;
}

Configurable::Configurable(const Configurable &other)
{
    *this = other;
}








