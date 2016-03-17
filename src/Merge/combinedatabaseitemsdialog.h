#ifndef COMBINEDATABASEITEMSDIALOG_H
#define COMBINEDATABASEITEMSDIALOG_H

#include <QDialog>

#include <QTableWidget>

#include "mergeitem.h"

namespace Ui {
    class CombineDatabaseItemsDialog;
}

class CombineDatabaseItemsDialog : public QDialog
{
    Q_OBJECT
public:
    CombineDatabaseItemsDialog(DatabaseMergerBase *merger, MergeItem* mergeItem, QWidget *parent = 0);
    virtual ~CombineDatabaseItemsDialog();

    void accept();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);
    QWidget *detailsPage() const;
    DatabaseMergerBase* databaseMerger() const;
    void setDetailsPage(QWidget* page, const QString& title);
    MergeItem* mergeItem() const;

private:
    void initItems();

private slots:
    void updateHeaderWidths();
    void onSelectionChange();

private:
    Ui::CombineDatabaseItemsDialog* ui;
    MergeItem* m_mergeItem;
    DatabaseMergerBase* m_databaseMerger;
    QList<MergeItem::ModifyDetail> m_modifyDetails;


signals:

public slots:

};

#endif // COMBINEDATABASEITEMSDIALOG_H
