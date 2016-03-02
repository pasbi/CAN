#ifndef COMBINESONGSDIALOG_H
#define COMBINESONGSDIALOG_H

#include <QDialog>
#include "combineitems.h"
#include "Database/SongDatabase/song.h"
#include "QItemSelection"

namespace Ui {
class CombineSongsDialog;
}

class Song;
class Attachment;
class CombineSongsDialog : public QDialog, public CombineItems<Song>
{
    Q_OBJECT

public:
    explicit CombineSongsDialog(const Song* masterSong, const Song* slaveSong, QWidget *parent = 0);
    ~CombineSongsDialog();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::CombineSongsDialog *ui;
    void init();

    void updateHeaderWidths();
    void addTableWidgetItem(const QString& master, const QString& slave, const QString &name);
    bool useSlaveValue(const QString& key) const;

};

#endif // COMBINESONGSDIALOG_H
