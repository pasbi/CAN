#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QObject>

class CommandTranslator : public QObject { Q_OBJECT };

class Command : public QUndoCommand
{
public:
    enum Type { SongDatabaseRelated, EventDatabaseRelated, Other };
    Type type() const { return m_type; }

protected:
    explicit Command(Type type = Other);

private:
    const Type m_type = Other;
};

#endif // COMMAND_H
