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
    void assembleCombination(Song *result);

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::CombineSongsDialog *ui;
    void init();

    QString m_title;
    QString m_artist;
    Song::Label m_label;
    Song::State m_state;
    Chord m_key;
    QStringList m_singers;
    QStringList m_soloPlayers;
    QString m_comments;
    void updateHeaderWidths();
    void addTableWidgetItem(const QString& master, const QString& slave, const QString &name);

};

#endif // COMBINESONGSDIALOG_H
