#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>

class Command : public QUndoCommand
{
public:
    enum Type { SongDatabaseRelated, EventDatabaseRelated, Other };
    explicit Command(QUndoCommand *parent = 0);

public:
    Type type() const { return m_type; }

protected:
    Type m_type = Other;


};

#endif // COMMAND_H
