#ifndef COMBINESONGSDIALOG_H
#define COMBINESONGSDIALOG_H

#include <QDialog>
#include "combineitems.h"

namespace Ui {
class CombineSongsDialog;
}

class Song;

class CombineSongsDialog : public QDialog, public CombineItems<Song>
{
    Q_OBJECT

public:
    explicit CombineSongsDialog(const Song* masterSong, const Song* slaveSong, QWidget *parent = 0);
    ~CombineSongsDialog();
    void assembleCombination(const Song *master, const Song *slave, Song *result);

private:
    Ui::CombineSongsDialog *ui;
};

#endif // COMBINESONGSDIALOG_H
