#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <QString>
#include <QSettings>
#include <QList>
#include <QJsonObject>
#include <QObject>


/**
 * @brief The ConfigurableTranslator class is used to be able to write ConfigurableTranslator::tr("foobar").
 *  This is better than writing QObject::tr("foobar") since there is a distinct namespace for Configurable-related
 *  translations.
 */
class ConfigurableTranslator : public QObject { };
#define TR(arg) ConfigurableTranslator::tr(arg)

#include "global.h"

#define DECL_CONFIG( CLASSNAME )    \
    public:                                  \
        static Configurable config


#define DEFN_CONFIG( CLASSNAME, Caption )    \
    Configurable CLASSNAME::config(#CLASSNAME, QObject::tr(Caption, "Configurable Caption"))


#define CONFIGURABLE_ADD_ITEM( CLASSNAME, KEY, CAPTION, DEFAULT_VALUE, OPTIONS )  \
    struct ConfigurableStaticInitializer_##CLASSNAME##_##KEY {    \
        ConfigurableStaticInitializer_##CLASSNAME##_##KEY() {     \
            CLASSNAME::config.addItem( #KEY, QObject::tr(CAPTION, "Configurable Item Caption"), DEFAULT_VALUE, OPTIONS );   \
        }                                                       \
    } private__ConfigurableStaticInitializer_##CLASSNAME##_##KEY

#define CONFIGURABLE_ADD_ITEM_HIDDEN( CLASSNAME, KEY, DEFAULT_VALUE )  \
    CONFIGURABLE_ADD_ITEM( CLASSNAME, KEY, #KEY, DEFAULT_VALUE, ConfigurationItemOptions::HiddenInterface() )

class ConfigurationItemOptions
{
public:
    enum Interface { Invalid, Hidden,
                     LineEdit, TextEdit, PathEdit,
                     Slider, DoubleSlider,
                     SpinBox, DoubleSpinBox,
                     AdvancedSlider, AdvancedDoubleSlider,
                     RadioButtons, ComboBox, EditableComboBox,
                     ColorEditor, Checkbox                   };
    ConfigurationItemOptions() :
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
    ConfigurationItemOptions( const Interface       interface,
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


    // ConfigurationItemOptions factory
    static ConfigurationItemOptions HiddenInterface             (                                                                       )  { return ConfigurationItemOptions( Hidden,               "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurationItemOptions LineEditOptions             ( const QString & placeHolderText                                       )  { return ConfigurationItemOptions( LineEdit,             placeHolderText,    "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurationItemOptions TextEditOptions             ( const QString & placeHolderText                                       )  { return ConfigurationItemOptions( TextEdit,             placeHolderText,    "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurationItemOptions PathEditOptions             ( const QString fileFilter, bool isOpenDialog, bool isFileDialog        )  { return ConfigurationItemOptions( PathEdit,             "",                 fileFilter, isOpenDialog,   isFileDialog,   0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurationItemOptions SliderOptions               ( int min, int max, int step = 1                                        )  { return ConfigurationItemOptions( Slider,               "",                 "",         false,          false,          min,    max,    step,   0,      0,      0,      "",     QStringList()   ); }
    static ConfigurationItemOptions SpinBoxOptions              ( int min, int max, int step = 1, const QString & suffix = ""           )  { return ConfigurationItemOptions( SpinBox,              "",                 "",         false,          false,          min,    max,    step,   0,      0,      0,      suffix, QStringList()   ); }
    static ConfigurationItemOptions AdvancedSliderOptions       ( int min, int max, int step = 1, const QString & suffix = ""           )  { return ConfigurationItemOptions( AdvancedSlider,       "",                 "",         false,          false,          min,    max,    step,   0,      0,      0,      suffix, QStringList()   ); }
    static ConfigurationItemOptions DoubleSliderOptions         ( double min, double max, double step = 1                               )  { return ConfigurationItemOptions( DoubleSlider,         "",                 "",         false,          false,          0,      0,      0,      min,    max,    step,   "",     QStringList()   ); }
    static ConfigurationItemOptions DoubleSpinBoxOptions        ( double min, double max, double step = 1, const QString & suffix = ""  )  { return ConfigurationItemOptions( DoubleSpinBox,        "",                 "",         false,          false,          0,      0,      0,      min,    max,    step,   suffix, QStringList()   ); }
    static ConfigurationItemOptions AdvancedDoubleSliderOptions ( double min, double max, double step = 1, const QString & suffix = ""  )  { return ConfigurationItemOptions( AdvancedDoubleSlider, "",                 "",         false,          false,          0,      0,      0,      min,    max,    step,   suffix, QStringList()   ); }
    static ConfigurationItemOptions RadioButtonsOptions         ( const QStringList & alternatives                                      )  { return ConfigurationItemOptions( RadioButtons,         "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     alternatives    ); }
    static ConfigurationItemOptions ComboBoxOptions             ( const QStringList & alternatives                                      )  { return ConfigurationItemOptions( ComboBox,             "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     alternatives    ); }
    static ConfigurationItemOptions EditableComboBoxOptions     ( const QStringList & alternatives                                      )  { return ConfigurationItemOptions( EditableComboBox,     "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     alternatives    ); }
    static ConfigurationItemOptions ColorEditorOptions          (                                                                       )  { return ConfigurationItemOptions( ColorEditor,          "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurationItemOptions CheckboxOptions             (                                                                       )  { return ConfigurationItemOptions( Checkbox,             "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }

};

class ConfigurationItem : public QObject
{
    Q_OBJECT
public:
    ConfigurationItem() :
        m_actualValue(),
        m_defaultValue(),
        m_resetValue(),
        m_options(),
        m_caption()
    {

    }

    ConfigurationItem( const QString &                  caption,
                       const QVariant &                 actualValue,
                       const QVariant &                 defaultValue,
                       const ConfigurationItemOptions & options) :
        m_actualValue(actualValue),
        m_defaultValue(defaultValue),
        m_resetValue(actualValue),
        m_options(options),
        m_caption( caption )
    {

    }

    QString caption() const { return m_caption; }
    ConfigurationItemOptions options() const { return m_options; }
    QVariant actualValue() const { return m_actualValue; }
    QVariant defaultValue() const { return m_defaultValue; }

    /**
     * @brief setDefaultValue normally, default value is set in constructor and should not be set
     *  manually. Sometimes, default value is not accessible at this time, then default value
     *  can be set this way. Note that default value can be set only once (including possible
     *  assignment in ctor). Pass QVariant() when you do not want to set default value in ctor.
     * @param value
     */
    void setDefaultValue( const QVariant & value );

public slots:
    void apply() { m_resetValue = m_actualValue; }
    void reset();
    void restore() { set( m_defaultValue ); }
    void set( const QVariant & value );


private:
    QVariant m_actualValue;
    QVariant m_defaultValue;
    QVariant m_resetValue;
    ConfigurationItemOptions m_options;

signals:
    void valueChanged( QVariant );

private:
    const QString m_caption;

};





class Configurable
{
public:
    Configurable(const QString & prefix, const QString & caption );
    ~Configurable();

    void addItem(const QString &                    key,
                 const QString &                    caption,
                 const QVariant &                   defaultValue,
                 const ConfigurationItemOptions &   options );

    bool contains(const QString & key) const;
    ConfigurationItem* item(const QString & key) { return m_items[key]; }
    const ConfigurationItem* item(const QString & key) const { return m_items[key]; }
    QVariant value( const QString & key ) const { return item(key)->actualValue(); }
    QVariant operator[] ( const QString & key ) const { return item(key)->actualValue(); }

    // do never return QVariant& (note the lacking const). always call ConfigurationItem::set.
    void set( const QString & key, const QVariant & value ) { item(key)->set(value); }

    void reset();
    void apply();
    void restore();

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

    QString prefix() const { return m_prefix; }
    QStringList itemKeys() const { return m_items.keys(); }

private:
    QMap<QString, ConfigurationItem*> m_items;
    const QString m_prefix;
    const QString m_caption;

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
