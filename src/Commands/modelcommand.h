#ifndef MODELCOMMAND_H
#define MODELCOMMAND_H

#include "command.h"

template<typename T>
class ModelCommand : public Command
{
public:
    ModelCommand(T* model, Type type = Other) :
        Command(type),
        m_model(model)
    {
    }

    T* model() const
    {
        return m_model;
    }

private:
    T* m_model;
};

#include "Database/SongDatabase/songdatabase.h"
class SongDatabaseCommand : public ModelCommand<SongDatabase>
{
protected:
    SongDatabaseCommand(SongDatabase* songDatabase) :
        ModelCommand<SongDatabase>(songDatabase, SongDatabaseRelated)
    {

    }
};

#include "Database/EventDatabase/eventdatabase.h"
class EventDatabaseCommand : public ModelCommand<EventDatabase>
{
protected:
    EventDatabaseCommand(EventDatabase* eventDatabase) :
        ModelCommand<EventDatabase>(eventDatabase, EventDatabaseRelated)
    {

    }
};

#include "Database/EventDatabase/setlist.h"
class SetlistCommand : public ModelCommand<Setlist>
{
protected:
    SetlistCommand(Setlist* setlist) :
        ModelCommand<Setlist>(setlist, EventDatabaseRelated)
    {

    }
};

template<typename AttachmentT>
class AttachmentCommand : public ModelCommand<AttachmentT>
{
protected:
    AttachmentCommand(AttachmentT* setlist) :
        ModelCommand<AttachmentT>(setlist, Command::SongDatabaseRelated)
    {

    }
};

#endif // MODELCOMMAND_H
