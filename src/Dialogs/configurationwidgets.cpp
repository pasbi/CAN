#include "configurationwidgets.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QCheckBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>


// magic configuration-class-template:
// most configuration widgets are set up the same.
// this defines a template to define a class based on a specific QWidget (as QLineEdit or QCheckBox, ...).
#define DEFN_CONFIG_CLASS( TYPE, SIGNAL_NAME, ACTION_ON_VALUE_CHANGED, UPDATE_ACTION, INITIALIZE )          \
class Config##TYPE : public ConfigurationWidget                                                             \
{                                                                                                           \
    TYPE* m_edit;                                                                                           \
public:                                                                                                     \
    Config##TYPE( ConfigurationItem *item, QWidget* parent ) : ConfigurationWidget(item, parent)            \
    {                                                                                                       \
        m_edit = new TYPE( this );                                                                          \
        QVBoxLayout* layout = new QVBoxLayout( this );                                                      \
        layout->setContentsMargins( 0, 0, 0, 0 );                                                           \
        layout->addWidget( m_edit );                                                                        \
        INITIALIZE;                                                                                         \
        connect( m_edit, SIGNAL_NAME, [this]()                                                              \
        {                                                                                                   \
            ACTION_ON_VALUE_CHANGED;                                                                        \
        });                                                                                                 \
        updateEdit();                                                                                       \
        setLayout( layout );                                                                                \
    }                                                                                                       \
public slots:                                                                                               \
    void updateEdit()                                                                                       \
    {                                                                                                       \
        UPDATE_ACTION;                                                                                      \
    }                                                                                                       \
};


DEFN_CONFIG_CLASS( QCheckBox,
                   &QCheckBox::toggled,
                   emit valueChanged( m_edit->isChecked() ),
                   m_edit->setChecked(   m_item->actualValue().toBool()   ),
                   )
DEFN_CONFIG_CLASS( QTextEdit,
                   &QTextEdit::textChanged,
                   emit valueChanged( m_edit->toPlainText() ),
                   m_edit->setPlainText( m_item->actualValue().toString() ),
                   m_edit->setPlaceholderText( item->options().placeHolderText()) )

DEFN_CONFIG_CLASS( QComboBox,
                   static_cast< void(QComboBox::*)(int) >(&QComboBox::currentIndexChanged),
                   emit valueChanged( m_edit->currentIndex() ),
                   m_edit->setCurrentIndex( m_item->actualValue().toInt() ),
                   m_edit->addItems( m_item->options().alternatives() ) )

DEFN_CONFIG_CLASS( QLineEdit,
                   &QLineEdit::textChanged,
                   emit valueChanged( m_edit->text() ),
                   m_edit->setText( m_item->actualValue().toString() ),
                   m_edit->setPlaceholderText( item->options().placeHolderText()) )

// some classes cannot be created with the fancy template above...
class ConfigAdvancedDoubleSlider : public ConfigurationWidget
{
    QSlider* m_slider;
    QDoubleSpinBox* m_spinBox;
public:
    ConfigAdvancedDoubleSlider( ConfigurationItem* item, QWidget* parent ) : ConfigurationWidget(item, parent)
    {
        m_slider = new QSlider(this);
        m_spinBox = new QDoubleSpinBox(this);
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->addWidget( m_slider );
        layout->addWidget( m_spinBox );
        setLayout(layout);

        double max = item->options().max_d();
        double min = item->options().min_d();
        double stepsize = item->options().step_d();
        int steps = (max - min) / item->options().step_d();

        m_spinBox->setMinimum(min);
        m_spinBox->setMaximum(max);
        m_spinBox->setSingleStep(item->options().step_d());
        m_spinBox->setSuffix(item->options().suffix());

        m_slider->setMinimum(0);
        m_slider->setMaximum(steps);
        m_slider->setOrientation(Qt::Horizontal);

        connect( m_spinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                 [min, stepsize, this](double value)
        {
            int sliderpos = (value - min) / stepsize;
            m_slider->setValue(sliderpos);
            emit valueChanged( m_spinBox->value() );
        });

        connect( m_slider, &QSlider::valueChanged,
                 [min, stepsize, this](int value)
        {
            double spinBoxValue = min + value * stepsize;
            m_spinBox->setValue( spinBoxValue );
        });

        m_spinBox->setValue( item->actualValue().toDouble() );
    }
public slots:
    void updateEdit()
    {
        m_spinBox->setValue( m_item->actualValue().toDouble() );
    }


};



ConfigurationWidget::ConfigurationWidget(ConfigurationItem *item, QWidget *parent) :
    QWidget(parent),
    m_item( item )
{
    connect( this, SIGNAL(valueChanged(QVariant)), m_item, SLOT(set(QVariant)) );
}

ConfigurationWidget* ConfigurationWidget::create( ConfigurationItem *item, QWidget* parent )
{
    switch (item->options().interface())
    {
    case ConfigurationItemOptions::Hidden:
    case ConfigurationItemOptions::Invalid:
        assert( false );
        break;
    case ConfigurationItemOptions::TextEdit:
        return new ConfigQTextEdit( item, parent );
    case ConfigurationItemOptions::Checkbox:
        return new ConfigQCheckBox( item, parent );
    case ConfigurationItemOptions::AdvancedDoubleSlider:
        return new ConfigAdvancedDoubleSlider( item, parent );
    case ConfigurationItemOptions::ComboBox:
        return new ConfigQComboBox( item, parent );
    case ConfigurationItemOptions::LineEdit:
        return new ConfigQLineEdit( item, parent );

    //TODO implementation missing:
    case ConfigurationItemOptions::PathEdit:
    case ConfigurationItemOptions::Slider:
    case ConfigurationItemOptions::DoubleSlider:
    case ConfigurationItemOptions::SpinBox:
    case ConfigurationItemOptions::DoubleSpinBox:
    case ConfigurationItemOptions::AdvancedSlider:
    case ConfigurationItemOptions::RadioButtons:
    case ConfigurationItemOptions::EditableComboBox:
    case ConfigurationItemOptions::ColorEditor:
    default:
        return NULL;
    }
}
