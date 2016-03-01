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
    explicit MergeDialog(Merge* merge, QWidget *parent = 0);
    ~MergeDialog();

    QMap<Song*, MergeItemBase*> attachmentMergeItems() const { Q_UNIMPLEMENTED(); return QMap<Song*, MergeItemBase*>(); }

private slots:
    void on_buttonNext_clicked();
    void on_buttonPrevious_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::MergeDialog *ui;
    Merge* m_merger;
    void setMerger(Merge* merger);
    void updateButtonText();

    static const QStringList BUTTON_TEXT;
    int buttonWidth();
};

#endif // MERGEDIALOG_H
