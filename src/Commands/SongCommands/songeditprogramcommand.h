#ifndef SONGEDITPROGRAMCOMMAND_H
#define SONGEDITPROGRAMCOMMAND_H

#include "Commands/modelcommand.h"
#include "program.h"

class Song;
class SongEditProgramCommand : public ModelCommand<Song>
{
public:
    SongEditProgramCommand(Song* song, const Program& program);

    void undo();
    void redo();

private:
    const Program m_newProgram;
    const Program m_oldProgram;
};

#endif // SONGEDITPROGRAMCOMMAND_H
