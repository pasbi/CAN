#include "configurationwidgets.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QCheckBox>

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
    }
    return NULL;
}
