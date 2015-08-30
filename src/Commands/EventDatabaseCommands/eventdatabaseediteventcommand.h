#ifndef EVENTDATABASEEDITEVENTCOMMAND_H
#define EVENTDATABASEEDITEVENTCOMMAND_H

#include "Commands/modelcommand.h"

class EventDatabaseEditEventCommand : public EventDatabaseCommand
{
public:
    EventDatabaseEditEventCommand(EventDatabase*        eventDatabase,
                                  const QModelIndex &   index,
                                  const QVariant &      newData,
                                  const int             role    );

    void undo();
    void redo();

private:
    const QModelIndex   m_index;
    const QVariant      m_newData;
    const QVariant      m_oldData;
    const int           m_role;
};

#endif // EVENTDATABASEEDITEVENTCOMMAND_H
