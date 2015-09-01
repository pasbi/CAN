#include "songeditprogramcommand.h"
#include "Database/SongDatabase/song.h"

SongEditProgramCommand::SongEditProgramCommand(Song *song, const Program &program) :
    ModelCommand<Song>( song ),
    m_newProgram( program ),
    m_oldProgram(song->program())
{
    setText( CommandTranslator::tr("set program") );
}

void SongEditProgramCommand::undo()
{
    model()->setProgram( m_oldProgram );
}

void SongEditProgramCommand::redo()
{
    model()->setProgram( m_newProgram );
}
