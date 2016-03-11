#ifndef COMBINEDATABASEITEMSDIALOG_H
#define COMBINEDATABASEITEMSDIALOG_H

#include <QDialog>
#include "QItemSelection"
#include "mergeitem.h"
#include "mergelistwidget.h"

namespace Ui {
class CombineDatabaseItemsDialog;
}

class QTableWidgetItem;
class CombineDatabaseItemsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CombineDatabaseItemsDialog(DatabaseMergerBase *mergeBase, MergeItem* mergeItem, QWidget* parent = nullptr);
    ~CombineDatabaseItemsDialog();

    void accept();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);
    MergeListWidget* detailsMergeListWidget() const;

private slots:
    void onSelectionChange();

private:
    Ui::CombineDatabaseItemsDialog *ui;
    void initItems();

    void updateHeaderWidths();

    MergeItem* m_mergeItem;
    QList<MergeItem::ModifyDetail> m_modifyDetails;
    QStringList m_keys;


};

#endif // COMBINEDATABASEITEMSDIALOG_H
