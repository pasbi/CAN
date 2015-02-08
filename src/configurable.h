#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <QString>
#include <QSettings>
#include <QList>
#include <QJsonObject>

#include "global.h"

class Configurable
{
public:
    Configurable( const QString & prefix, const QString & caption );
    ~Configurable();

    void addItem(const QString & key, const QString & actualValue, const QString & defaultValue );
    void setItem(const QString & key, const QString & newValue);
    bool contains(const QString & key) const;
    QString item(const QString & key) const;

    /**
     * @brief reset resets the value at key to reset value.
     * @param key
     */
    void reset(const QString & key);

    /**
     * @brief apply sets the reset value to actual value.
     * @param key
     */
    void apply(const QString & key);

    /**
     * @brief setDefault sets the actual value to defaultValue.
     * @param key
     */
    void setDefault(const QString & key);

    /**
     * @brief saveConfiguration saves the configuration with QSettings system.
     *  Is automatically called in destructor.
     */
    void saveConfiguration() const;

    /**
     * @brief restoreConfiguration restores the configuration with QSettings system.
     *  Is automatically called in constructor.
     */
    void restoreConfiguration();

    QString caption() const { return m_caption; }

    // Hidden items are not visible to users. Usually, the user will never see this items. So they have no default value.
    void setHiddenItem( const QString & key, const QVariant & data ) const;
    QVariant hiddenItem( const QString & key ) const;


private:
    typedef struct ConfigurationItem
    {
        ConfigurationItem() :
            m_actualValue(),
            m_defaultValue(),
            m_resetValue()
        {

        }

        ConfigurationItem(const QJsonObject & json)
        {
            if (!json["actual"].isString() || !json["default"].isString())
            {
                WARNING << "Invalid formatted json object";
            }

            m_actualValue  = json["actual"].toString();
            m_resetValue   = json["actual"].toString();
            m_defaultValue = json["default"].toString();
        }

        ConfigurationItem(const QString & actualValue, const QString & defaultValue, const QString & resetValue) :
            m_actualValue(actualValue),
            m_defaultValue(defaultValue),
            m_resetValue(resetValue)
        {

        }

        QJsonObject toJsonObject() const
        {
            QJsonObject json;
            json.insert("actual", m_actualValue);
            json.insert("default", m_defaultValue);
            return json;
        }

        QString m_actualValue;
        QString m_defaultValue;
        QString m_resetValue;
    } ConfigurationItem;

    QMap<QString, ConfigurationItem> m_items;


    const QString m_prefix;
    const QString m_caption;


private:    // helper methods
    bool expectKeyToExist(const QString & key, const bool exist) const;



    // utility to access all configuration objects at once
private:
    static QMap<QString, Configurable*> m_configurationObjects;
    static bool registerConfigurable(Configurable* conf);
    static bool unregisterConfigurable(Configurable* conf);

//    /**
//     * @brief getConfigurable returns a pointer to an existing object.
//     *  Does NOT transfer ownership!
//     * @param key
//     * @return
//     */
//    static Configurable* getConfigurable(const QString & key );
public:
    static QList<Configurable*> getKeys() { return m_configurationObjects.values(); }
    static void saveAll();
    static void restoreAll();




};

#endif // CONFIGURABLE_H
