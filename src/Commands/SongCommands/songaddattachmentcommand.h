#ifndef SONGADDATTACHMENTCOMMAND_H
#define SONGADDATTACHMENTCOMMAND_H

#include "songcommand.h"

class SongAddAttachmentCommand : public SongCommand
{
public:
    SongAddAttachmentCommand(Song* song, const QString & classname);
    ~SongAddAttachmentCommand();

    void undo();
    void redo();

private:
    QString m_classname;
    Attachment* m_attachment = 0;
    bool m_ownsAttachment = false;
};

#endif // SONGADDATTACHMENTCOMMAND_H
