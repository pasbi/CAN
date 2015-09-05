#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QObject>
#include <typeinfo>
#include <QStringList>

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
    static const QStringList SETLIST_RELATED_TYPENAMES;

    template<typename T>
    static Type inferType()
    {
        QString type = QString(typeid(T).name());
        if (SONG_RELATED_TYPENAMES.contains(type))
        {
            return SongDatabaseRelated;
        }
        else if (EVENT_RELATED_TYPENAMES.contains(type))
        {
            return EventDatabaseRelated;
        }
        else if (SETLIST_RELATED_TYPENAMES.contains(type))
        {
            return EventDatabaseRelated;
        }
        else
        {
            return Other;
        }
    }

private:
    const Type m_type = Other;

};

#endif // COMMAND_H
