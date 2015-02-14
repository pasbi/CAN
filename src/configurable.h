#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <QString>
#include <QSettings>
#include <QList>
#include <QJsonObject>
#include <QObject>

#include "global.h"

#define REGISTER_DECL_CONFIG( CLASSNAME )    \
    public:                                  \
        static Configurable config


#define REGISTER_DEFN_CONFIG( CLASSNAME, Caption )    \
    Configurable CLASSNAME::config(#CLASSNAME, QObject::tr(Caption, "Configurable Caption"))

#define CONFIGURABLE_ADD_ITEM( CLASSNAME, KEY, DEFAULT_VALUE, OPTIONS )  \
    struct ConfigurableStaticInitializer_##CLASSNAME##_##KEY {    \
        ConfigurableStaticInitializer_##CLASSNAME##_##KEY() {     \
            CLASSNAME::config.addItem( #KEY, DEFAULT_VALUE, OPTIONS );   \
        }                                                       \
    } private__ConfigurableStaticInitializer_##CLASSNAME##_##KEY

class ConfigurationItemOptions
{
private:
    enum Interface { Invalid, Hidden,
                     LineEdit, PathEdit,
                     Slider, DoubleSlider,
                     SpinBox, DoubleSpinBox,
                     AdvancedSlider, AdvancedDoubleSlider,
                     RadioButtons, ComboBox, EditableComboBox,
                     ColorEditor, Checkbox                   };
public:
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


    // ConfigurationItemOptions factory
    static ConfigurationItemOptions HiddenInterface             (                                                                       )  { return ConfigurationItemOptions( Hidden,               "",                 "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
    static ConfigurationItemOptions LineEditOptions             ( const QString & placeHolderText                                       )  { return ConfigurationItemOptions( LineEdit,             placeHolderText,    "",         false,          false,          0,      0,      0,      0,      0,      0,      "",     QStringList()   ); }
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

struct ConfigurationItem
{
public:
    ConfigurationItem() :
        actualValue(),
        defaultValue(),
        resetValue(),
        options()
    {

    }

    ConfigurationItem( const ConfigurationItem& other ) :
        actualValue( other.actualValue ),
        defaultValue ( other.defaultValue ),
        resetValue( other.resetValue ),
        options( other.options )
    {

    }

    ConfigurationItem(const QVariant & actualValue, const QVariant & defaultValue, const ConfigurationItemOptions & options) :
        actualValue(actualValue),
        defaultValue(defaultValue),
        resetValue(actualValue),
        options(options)
    {

    }

    QVariant actualValue;
    QVariant defaultValue;
    QVariant resetValue;
    ConfigurationItemOptions options;

};





class Configurable
{
public:
    Configurable(const QString & prefix, const QString & caption);
    ~Configurable();

    void addItem(const QString & key, const QVariant & defaultValue, const ConfigurationItemOptions &options );
    void setItem(const QString & key, const QVariant & newValue);
    bool contains(const QString & key) const;
    QVariant item(const QString & key) const;

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

    QString prefix() const { return m_prefix; }

    void debug() const
    {
        for (const QString & key : m_items.keys())
        {
            qDebug() << key << m_items[key].actualValue << m_items[key].defaultValue << m_items[key].resetValue;
        }
    }


private:


    QMap<QString, ConfigurationItem> m_items;


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

protected:
    static ConfigurableRegisterer* registerer()
    {
        static ConfigurableRegisterer* singleton;
        if (!singleton)
        {
            singleton = new ConfigurableRegisterer();
        }
        return singleton;
    }

public:
    static void saveAll();
    static void restoreAll();





};

#endif // CONFIGURABLE_H
