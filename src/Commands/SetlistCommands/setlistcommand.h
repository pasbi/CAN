#ifndef SETLISTCOMMAND_H
#define SETLISTCOMMAND_H

#include "../command.h"
#include "Database/EventDatabase/setlist.h"

class SetlistCommand : public Command
{
public:
    explicit SetlistCommand( Setlist* setlist );
    Setlist* setlist() const { return m_setlist; }

private:
    Setlist* const m_setlist;
};

#endif // SETLISTCOMMAND_H
