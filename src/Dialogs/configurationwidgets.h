#ifndef CONFIGURATIONWIDGETS_H
#define CONFIGURATIONWIDGETS_H

#include <QWidget>
#include "configurable.h"

class ConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationWidget(ConfigurableItem *item, QWidget *parent = 0 );
    static ConfigurationWidget* create(ConfigurableItem *item , QWidget *parent = 0, bool ignoreDefeatable = false );

signals:
    void valueChanged(QVariant);

public slots:
    virtual void updateEdit() = 0;

protected:
    ConfigurableItem* const m_item;

};

#endif // CONFIGURATIONWIDGETS_H
