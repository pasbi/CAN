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
    virtual ~Command();

    static const QStringList SONG_RELATED_TYPENAMES;
    static const QStringList EVENT_RELATED_TYPENAMES;

private:
    const Type m_type = Other;
};

#endif // COMMAND_H
