#ifndef ATTACHMENTHISTORY_H
#define ATTACHMENTHISTORY_H

#include <QMap>

class Attachment;
class Song;
class AttachmentHistory
{
public:
    AttachmentHistory();

    void removeAttachment(Attachment* a);
    void appendAttachment(Attachment* a);

    void loadHistory(Song* song);
    Attachment* nextAttachment();
    Attachment* previousAttachment();
    Attachment* currentAttachment() const;

    bool hasNextAttachment() const;
    bool hasPreviousAttachment() const;

    void removeObsoleteAttachments();

private:
    QMap<Song*, QList<Attachment*>> m_histories;
    QList<Attachment*>* m_history;
    int m_index;
    Song* m_currentSong;
};

void testHist();
#endif // ATTACHMENTHISTORY_H
