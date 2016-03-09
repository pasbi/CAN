#ifndef ATTACHMENTDATABASE_H
#define ATTACHMENTDATABASE_H

#include "Database/database.h"

class Attachment;
class Song;
class AttachmentDatabase : public Database<Attachment>
{
public:
    AttachmentDatabase(Song* song, Project *project);
    ~AttachmentDatabase();

    int columnCount(const QModelIndex &parent) const;
    QString itemName(int n) const;

    Song* song() const;

private:
    Song* m_song;
};

#endif // ATTACHMENTDATABASE_H
