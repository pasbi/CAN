#ifndef PEOPLESDIALOG_H
#define PEOPLESDIALOG_H

#include <QDialog>
#include <QBitArray>

namespace Ui {
class PeoplesDialog;
}

class QTableWidgetItem;
class PeoplesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PeoplesDialog(const QStringList& allPeoples, const QStringList &activePeoples, QWidget *parent = 0);
    ~PeoplesDialog();
    QStringList activePeoples() const { return m_activePeoples; }

public slots:
    void accept();

private slots:
    void updateColumnSizes();
    void newRow(bool overwriteFormerWidgets = true);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    Ui::PeoplesDialog *ui;

    QStringList m_activePeoples;
    QStringList m_allPeoples;

    QPushButton* createLastRowButton();
    QPushButton* createRemoveButton(QWidget *cellWidget);
};

#endif // PEOPLESDIALOG_H
