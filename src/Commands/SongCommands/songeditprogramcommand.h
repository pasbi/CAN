#ifndef SONGEDITPROGRAMCOMMAND_H
#define SONGEDITPROGRAMCOMMAND_H

#include "songcommand.h"

class SongEditProgramCommand : public SongCommand
{
public:
    SongEditProgramCommand(Song* song, const Program& program);
    void undo();
    void redo();

private:
    Program m_newProgram, m_oldProgram;
};

#endif // SONGEDITPROGRAMCOMMAND_H
