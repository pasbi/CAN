#include "songeditprogramcommand.h"

SongEditProgramCommand::SongEditProgramCommand(Song *song, const Program &program) :
    SongCommand( song ),
    m_newProgram( program )
{
    m_oldProgram = song->program();
    setText( CommandTranslator::tr("set program") );
}

void SongEditProgramCommand::undo()
{
    song()->setProgram( m_oldProgram );
}

void SongEditProgramCommand::redo()
{
    song()->setProgram( m_newProgram );
}
