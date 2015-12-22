#ifndef PEOPLESDIALOG_H
#define PEOPLESDIALOG_H

#include <QDialog>
#include <QBitArray>

namespace Ui {
class PeoplesDialog;
}

class PeoplesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PeoplesDialog(const QStringList& peoples, const QBitArray &actives, QWidget *parent = 0);
    ~PeoplesDialog();
    QBitArray activePeoples() const { return m_actives; }
public slots:
    void accept();

private:
    Ui::PeoplesDialog *ui;

    QBitArray m_actives;
};

#endif // PEOPLESDIALOG_H
