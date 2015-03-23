#include "configurationwidgets.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QCheckBox>
#include <QSlider>
#include <QDoubleSpinBox>

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
        connect( m_edit, &TYPE::SIGNAL_NAME, [this]()                                                       \
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
                   toggled,
                   emit valueChanged( m_edit->isChecked() ),
                   m_edit->setChecked(   m_item->actualValue().toBool()   ),
                   )
DEFN_CONFIG_CLASS( QTextEdit,
                   textChanged,
                   emit valueChanged( m_edit->toPlainText() ),
                   m_edit->setPlainText( m_item->actualValue().toString() ),
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
    case ConfigurationItemOptions::TextEdit:
        return new ConfigQTextEdit( item, parent );
    case ConfigurationItemOptions::Checkbox:
        return new ConfigQCheckBox( item, parent );
    case ConfigurationItemOptions::AdvancedDoubleSlider:
        return new ConfigAdvancedDoubleSlider( item, parent );
    }
    return NULL;
}
