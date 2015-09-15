#ifndef ADDFILESTOINDEXDIALOG_H
#define ADDFILESTOINDEXDIALOG_H

#include <QDialog>
#include "configurable.h"

namespace Ui {
class AddFilesToIndexDialog;
}

class QCheckBox;
class AddFilesToIndexDialog : public QDialog
{
    Q_OBJECT
    DECL_CONFIG( AddFilesToIndexDialog )


public:
    explicit AddFilesToIndexDialog(QWidget *parent = 0);
    ~AddFilesToIndexDialog();
    QMap<QString, bool> acceptedEndings() const;

private slots:
    void on_buttonOpenFileDialog_clicked();

public:
    QString path() const;
private:
    void setAcceptEnding(const QString& ending, bool accept);
    bool acceptEnding(const QString& ending) const;
    void setEndings(const QStringList& endings);

    QMap<QString, QCheckBox*> m_checkBoxes;
    Ui::AddFilesToIndexDialog *ui;
};

#endif // ADDFILESTOINDEXDIALOG_H
