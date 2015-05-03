#ifndef CONFIGURATIONWIDGETS_H
#define CONFIGURATIONWIDGETS_H

#include <QWidget>
#include "configurable.h"

class ConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationWidget(ConfigurationItem *item, QWidget *parent = 0 );
    static ConfigurationWidget* create(ConfigurationItem *item , QWidget *parent = 0);

signals:
    void valueChanged(QVariant);

public slots:
    virtual void updateEdit() = 0;

protected:
    ConfigurationItem* const m_item;

};

#endif // CONFIGURATIONWIDGETS_H
