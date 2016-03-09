#include "attachmentdatabase.h"

AttachmentDatabase::AttachmentDatabase(Song *song, Project* project) :
    Database<Attachment>(project),
    m_song(song)
{
}

AttachmentDatabase::~AttachmentDatabase()
{

}

QString AttachmentDatabase::itemName(int n) const
{
    return tr("Attachment(s)", "", n);
}

int AttachmentDatabase::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_ASSERT(!parent.isValid());
    return 1;
}

Song* AttachmentDatabase::song() const
{
    return m_song;
}

