#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include <QDialog>
#include "merge.h"

namespace Ui {
class MergeDialog;
}

class EventMergeWidget;
class SongMergeWidget;
class MergeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeDialog(QWidget *parent = 0);
    ~MergeDialog();
    void setMerger(Merge* merger);

    QList<MergeInfo> songMergeItems() const;
    QList<MergeInfo> eventMergeItems() const;
    QMap<Song*, MergeInfo> attachmentMergeItems() const { Q_UNIMPLEMENTED(); return QMap<Song*, MergeInfo>(); }

private slots:
    void on_buttonBack_clicked();
    void on_buttonNext_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::MergeDialog *ui;
    Merge* m_merger;
    SongMergeWidget* songMergeWidget() const;
    EventMergeWidget* eventMergeWidget() const;
};

#endif // MERGEDIALOG_H
