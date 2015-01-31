#ifndef SONGCOMMAND_H
#define SONGCOMMAND_H

#include "Commands/command.h"
#include "Database/SongDatabase/song.h"

class SongCommand : public Command
{
public:
    SongCommand( Song* song );

    Song* song() const { return m_song; }

private:
    Song* m_song;
};

#endif // SONGCOMMAND_H
