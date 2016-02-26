#include "combinesongsdialog.h"
#include "ui_combinesongsdialog.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/attachment.h"

CombineSongsDialog::CombineSongsDialog(const Song *masterSong, const Song *slaveSong, QWidget *parent) :
    QDialog(parent),
    CombineItems(masterSong, slaveSong),
    ui(new Ui::CombineSongsDialog)
{
    ui->setupUi(this);
}

CombineSongsDialog::~CombineSongsDialog()
{
    delete ui;
}

void CombineSongsDialog::assembleCombination(const Song *master, const Song *slave, Song *result)
{
    Q_UNUSED(slave);

    // clone slave

    result->setTitle(master->title());
    result->setArtist(master->artist());
    result->setlabel(master->label());
    result->setState(master->state());
    result->setKey(master->key());
    result->setSingers(master->singers());
    result->setSoloPlayers(master->soloPlayers());
    result->setComments(master->comments());
    for (const Attachment* attachment : master->attachments())
    {
        result->addAttachment(attachment->copy());
    }
}
