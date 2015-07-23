#include "attachmentcommand.h"


AttachmentCommand::AttachmentCommand(Attachment *attachment) :
    m_attachment( attachment )
{
    m_type = SongDatabaseRelated;
}
