#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <QString>
#include <QSettings>
#include <QList>
#include <QJsonObject>
#include <QObject>

#include "global.h"

#define DECL_CONFIG( CLASSNAME )    \
    public:                                  \
        static Configurable config;


#define DEFN_CONFIG( CLASSNAME, Caption )    \
    Configurable CLASSNAME::config(#CLASSNAME, Caption)


#define CONFIGURABLE_ADD_ITEM_PRIVATE( CLASSNAME, KEY, CAPTION, HELP, DEFAULT_VALUE, SWITCH_ITEM_KEY, OPTIONS )  \
    struct ConfigurableStaticInitializer_##CLASSNAME##_##KEY {    \
        ConfigurableStaticInitializer_##CLASSNAME##_##KEY() {     \
            CLASSNAME::config.addItem( #KEY, CAPTION, HELP, DEFAULT_VALUE, SWITCH_ITEM_KEY, OPTIONS );   \
        }                                                       \
    } private__ConfigurableStaticInitializer_##CLASSNAME##_##KEY    \

#define CONFIGURABLE_ADD_ITEM( CLASSNAME, KEY, CAPTION, HELP, DEFAULT_VALUE, OPTIONS )  \
    CONFIGURABLE_ADD_ITEM_PRIVATE( CLASSNAME, KEY, CAPTION, HELP, DEFAULT_VALUE, "", OPTIONS )

#define CONFIGURABLE_ADD_ITEM_HIDDEN( CLASSNAME, KEY, DEFAULT_VALUE )  \
    CONFIGURABLE_ADD_ITEM( CLASSNAME, KEY, #KEY, "", DEFAULT_VALUE, ConfigurableItemOptions::HiddenInterface() )

#define CONFIGURABLE_ADD_DEFEATABLE_ITEM( CLASSNAME, KEY, CAPTION, HELP, DEFAULT_VALUE, DEFAULT_ENABLED, OPTIONS )  \
    CONFIGURABLE_ADD_ITEM_PRIVATE( CLASSNAME, KEY, CAPTION, HELP, DEFAULT_VALUE, "enable_"#KEY, OPTIONS ); \
    CONFIGURABLE_ADD_ITEM_HIDDEN( CLASSNAME, enable_##KEY, DEFAULT_ENABLED)


class Configurable;
class ConfigurableItemOptions
{
public:
    enum Interface { Invalid, Hidden,
                     LineEdit, TextEdit, PathEdit,
                     Slider, DoubleSlider,
                     SpinBox, DoubleSpinBox,
                     AdvancedSlider, AdvancedDoubleSlider,
                     RadioButtons, ComboBox, EditableComboBox,
                     ColorEditor, Checkbox                   };
    ConfigurableItemOptions() :
        m_interface( Invalid ),
        m_placeHolderText(),
        m_fileFilter(),
        m_isOpenDialog(),
        m_isFileDialog(),
        m_min_i(),
        m_max_i(),
        m_step_i(),
        m_min_d(),
        m_max_d(),
        m_step_d(),
        m_suffix(),
        m_alternatives()
    {

    }

private:
    ConfigurableItemOptions( const Interface       interface,
                              const QString &       placeHolderText,
                              const QString &       fileFilter,
                              const bool            isOpenDialog,
                              const bool            isFileDialog,
                              const int             min_i,
                              const int             max_i,
                              const int             step_i,
                              const double          min_d,
                              const double          max_d,
                              const double          step_d,
                              const QString &       suffix,
                              const QStringList &   alternatives ) :
        m_interface( interface ),
        m_placeHolderText( placeHolderText ),
        m_fileFilter( fileFilter ),
        m_isOpenDialog( isOpenDialog ),
        m_isFileDialog( isFileDialog ),
        m_min_i(min_i),
        m_max_i(max_i),
        m_step_i(step_i),
        m_min_d(min_d),
        m_max_d(max_d),
        m_step_d(step_d),
        m_suffix(suffix),
        m_alternatives(alternatives)
    {

    }

#define MEMBER(TYPE, NAME)                      \
    private:                                    \
        TYPE m_##NAME;                          \
    public:                                     \
        TYPE NAME() const { return m_##NAME; }

    MEMBER(Interface, interface)

    // line Edit options
    MEMBER(QString, placeHolderText)

    // PathEdit options
    MEMBER(QString, fileFilter)
    MEMBER(bool, isOpenDialog)  // else save dialog
    MEMBER(bool, isFileDialog)  // else dir dialog

    // int slider/spinbox options
    MEMBER(int, min_i)
    MEMBER(int, max_i)
    MEMBER(int, step_i)

    // double slider/spinbox options
    MEMBER(double, min_d)
    MEMBER(double, max_d)
    MEMBER(double, step_d)

    // spin boxes can have suffix
    MEMBER(QString, suffix)

    // (editable) combobox options, radiobutton options
    MEMBER(QStringList, alternatives)

    // color editor options
    // checkbox options
#undef MEMBER


public:

    // ConfigurableItemOptions factory
    static ConfigurableItemOptions HiddenInterface             (                                                                       )  { return ConfigurableItemOptions( Hidden,               "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurableItemOptions LineEditOptions             ( const QString & placeHolderText                                       )  { return ConfigurableItemOptions( LineEdit,             placeHolderText,    "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurableItemOptions TextEditOptions             ( const QString & placeHolderText                                       )  { return ConfigurableItemOptions( TextEdit,             placeHolderText,    "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurableItemOptions PathEditOptions             ( const QString fileFilter, bool isOpenDialog, bool isFileDialog        )  { return ConfigurableItemOptions( PathEdit,             "",                 fileFilter, isOpenDialog,   isFileDialog,   0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurableItemOptions SliderOptions               ( int min, int max, int step = 1                                        )  { return ConfigurableItemOptions( Slider,               "",                 "",         false,          false,          min,    max,    step,   0,      0,      0,      "",     QStringList()   ); }
    static ConfigurableItemOptions SpinBoxOptions              ( int min, int max, int step = 1, const QString & suffix = ""           )  { return ConfigurableItemOptions( SpinBox,              "",                 "",         false,          false,          min,    max,    step,   0,      0,      0,      suffix, QStringList()   ); }
    static ConfigurableItemOptions AdvancedSliderOptions       ( int min, int max, int step = 1, const QString & suffix = ""           )  { return ConfigurableItemOptions( AdvancedSlider,       "",                 "",         false,          false,          min,    max,    step,   0,      0,      0,      suffix, QStringList()   ); }
    static ConfigurableItemOptions DoubleSliderOptions         ( double min, double max, double step = 1                               )  { return ConfigurableItemOptions( DoubleSlider,         "",                 "",         false,          false,          0,      0,      0,      min,    max,    step,   "",     QStringList()   ); }
    static ConfigurableItemOptions DoubleSpinBoxOptions        ( double min, double max, double step = 1, const QString & suffix = ""  )  { return ConfigurableItemOptions( DoubleSpinBox,        "",                 "",         false,          false,          0,      0,      0,      min,    max,    step,   suffix, QStringList()   ); }
    static ConfigurableItemOptions AdvancedDoubleSliderOptions ( double min, double max, double step = 1, const QString & suffix = ""  )  { return ConfigurableItemOptions( AdvancedDoubleSlider, "",                 "",         false,          false,          0,      0,      0,      min,    max,    step,   suffix, QStringList()   ); }
    static ConfigurableItemOptions RadioButtonsOptions         ( const QStringList & alternatives                                      )  { return ConfigurableItemOptions( RadioButtons,         "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     alternatives    ); }
    static ConfigurableItemOptions ComboBoxOptions             ( const QStringList & alternatives                                      )  { return ConfigurableItemOptions( ComboBox,             "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     alternatives    ); }
    static ConfigurableItemOptions EditableComboBoxOptions     ( const QStringList & alternatives                                      )  { return ConfigurableItemOptions( EditableComboBox,     "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     alternatives    ); }
    static ConfigurableItemOptions ColorEditorOptions          (                                                                       )  { return ConfigurableItemOptions( ColorEditor,          "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurableItemOptions CheckboxOptions             (                                                                       )  { return ConfigurableItemOptions( Checkbox,             "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
};

class ConfigurableItem : public QObject
{
    Q_OBJECT
public:
    ConfigurableItem() :
        m_actualValue(),
        m_defaultValue(),
        m_resetValue(),
        m_options(),
        m_caption()
    {

    }

    ConfigurableItem(  Configurable*                    configurable,
                       const QString &                  caption,
                       const QString &                  help,
                       const QVariant &                 actualValue,
                       const QVariant &                 defaultValue,
                       const QString &                  switchItemKey,
                       const ConfigurableItemOptions &  options) :
        m_configurable( configurable ),
        m_actualValue(actualValue),
        m_defaultValue(defaultValue),
        m_resetValue(actualValue),
        m_options(options),
        m_caption( caption ),
        m_help( help ),
        m_switchItemKey( switchItemKey )
    {

    }

    QString caption() const;
    ConfigurableItemOptions options() const { return m_options; }
    QVariant actualValue() const { return m_actualValue; }
    QVariant defaultValue() const;
    QString help() const;

    /**
     * @brief setDefaultValue normally, default value is set in constructor and should not be set
     *  manually. Sometimes, default value is not accessible at this time, then default value
     *  can be set this way. Note that default value can be set only once (including possible
     *  assignment in ctor). Pass QVariant() when you do not want to set default value in ctor.
     * @param value
     */
    void setDefaultValue( const QVariant & value );


    // some general options
    void makeDefeatable( const QString& switchItemKey )
    {
        m_switchItemKey = switchItemKey;
    }

    void makeNotDefeatable( )
    {
        m_switchItemKey = "";
    }

    bool isDefeatable() const { return !m_switchItemKey.isEmpty(); }

    ConfigurableItem* getSwitchItem() const;

    ConfigurableItem* copy() const;


public slots:
    void apply();
    void reset();
    void restore();
    void set( const QVariant & value );



private:
    Configurable* m_configurable;
    QVariant m_actualValue;
    QVariant m_defaultValue;
    QVariant m_resetValue;
    ConfigurableItemOptions m_options;
    QString m_caption;
    QString m_help;

    // some items are defeatable. Store a link to the corresponding checkbox-item
    QString m_switchItemKey;

signals:
    void valueChanged( QVariant );

};





class Configurable
{
public:
    Configurable(const QString & prefix, const QString& caption );
    ~Configurable();

    void addItem(const QString &                    key,
                 const QString &                    caption,
                 const QString &                    help,
                 const QVariant &                   defaultValue,
                 const QString &                    switchItemKey,
                 const ConfigurableItemOptions &    options );

    bool contains(const QString & key) const;
    ConfigurableItem* item(const QString & key);
    const ConfigurableItem* item(const QString & key) const;
    QVariant value( const QString & key ) const;
    QVariant operator[] ( const QString & key ) const;

    // do never return QVariant& (note the lacking const). always call ConfigurableItem::set.
    void set( const QString & key, const QVariant & value );

    void reset();
    void apply();
    void restore();

    /**
     * @brief saveConfigurable saves the Configurable with QSettings system.
     *  Is automatically called in destructor.
     */
    void saveConfigurable() const;

    /**
     * @brief restoreConfigurable restores the Configurable with QSettings system.
     *  Is automatically called in constructor.
     */
    void restoreConfigurable();

    QString caption() const;

    QString prefix() const { return m_prefix; }
    QStringList itemKeys() const { return m_itemKeys; }

    Configurable& operator= (const Configurable& other);
    Configurable( const Configurable& other );

    static void deinit();

private:
    QMap<QString, ConfigurableItem*> m_items;
    QStringList m_itemKeys; // is needed to keep order. QMap::keys is arbitrary order.

    const QString m_prefix;
    const QString m_caption;

    static QSettings* m_settings;
    static QSettings* settings();

    // we want to know all classes that have a static Configurable member
    // i.e. we want to know the Configurable member in particular
private:
    class ConfigurableRegisterer
    {
    public:
        ConfigurableRegisterer() { }
        void registerConfigurable( Configurable* config );
        QMap<QString, Configurable*> configs() const { return m_configurables; }

    private:
        QMap<QString, Configurable*> m_configurables;
    };

public:
    static ConfigurableRegisterer* registerer()
    {
        static ConfigurableRegisterer* singleton;
        if (!singleton)
        {
            singleton = new ConfigurableRegisterer();
        }
        return singleton;
    }
    static QMap<QString, Configurable*> configurables() { return registerer()->configs(); }


public:
    static void saveAll();
    static void restoreAll();
};

#endif // CONFIGURABLE_H
