#ifndef COPYINDEXEDFILESDIALOG_H
#define COPYINDEXEDFILESDIALOG_H

#include <QDialog>
#include <QList>

namespace Ui {
class CopyIndexedFilesDialog;
}

class Song;
class CopyIndexedFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyIndexedFilesDialog(QList<Song*> songs, QWidget *parent = 0);
    ~CopyIndexedFilesDialog();

    QStringList sources() const;

private:
    Ui::CopyIndexedFilesDialog *ui;
    void setupTable(QList<Song *> songs);

    enum SelectMode { All, None, Toggle };
    void select(SelectMode mode);
};

#endif // COPYINDEXEDFILESDIALOG_H
