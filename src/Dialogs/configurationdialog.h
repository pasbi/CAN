#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include "configurable.h"

namespace Ui {
class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent = 0);
    ~ConfigurationDialog();

public slots:
    void accept();
    void reject();

private:
    Ui::ConfigurationDialog *ui;
    void buildPage(const QString & key);
    QList<Configurable*> m_configurables;
};

#endif // CONFIGURATIONDIALOG_H
