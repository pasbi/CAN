#include "attachmentcreatecommand.h"
#include "global.h"

AttachmentCreateCommand::AttachmentCreateCommand(Song *song, const QString & classname) :
    AttachmentCommand( song ),
    m_classname( classname )
{
    assert( Creatable::category(classname) == "Attachment" );
}

AttachmentCreateCommand::~AttachmentCreateCommand()
{
    if (m_ownsAttachment)
    {
        delete m_attachment;
    }
}

void AttachmentCreateCommand::undo()
{
    song()->removeAttachment( m_attachment );
    m_ownsAttachment = true;
}

void AttachmentCreateCommand::redo()
{
    if (!m_attachment)
    {
        CREATE( m_classname, m_attachment );
        m_attachment->setSong( song() );
        m_attachment->makeNameUnique();
    }

    song()->addAttachment( m_attachment );
}
