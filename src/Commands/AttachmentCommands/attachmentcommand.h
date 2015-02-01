#ifndef ATTACHMENTCOMMAND_H
#define ATTACHMENTCOMMAND_H

#include "Commands/command.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/attachment.h"

class AttachmentCommand : public Command
{
public:
    AttachmentCommand( Song* song ) __attribute__ ((deprecated));

    Song* song() const { return m_song; }
private:
    Song* m_song;
};

#endif // ATTACHMENTCOMMAND_H
