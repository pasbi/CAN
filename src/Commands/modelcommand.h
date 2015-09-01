#ifndef MODELCOMMAND_H
#define MODELCOMMAND_H

#include "command.h"
#include <typeinfo>
#include "Database/EventDatabase/setlist.h"



template<typename T>
class ModelCommand : public Command
{
public:
    ModelCommand(T* model) :
        Command(inferType()),
        m_model(model)
    {
    }

    T* model() const
    {
        return m_model;
    }

private:
    T* m_model;

    static Command::Type inferType()
    {
        QString type = QString(typeid(T).name());
        if (Command::SONG_RELATED_TYPENAMES.contains(type))
        {
            return Command::SongDatabaseRelated;
        }
        else if (Command::EVENT_RELATED_TYPENAMES.contains(type))
        {
            return Command::EventDatabaseRelated;
        }
        else
        {
            return Command::Other;
        }

    }
};


class SetlistCommand : public ModelCommand<Setlist>
{
protected:
    SetlistCommand(Setlist* setlist) :
        ModelCommand<Setlist>(setlist)
    {

    }
};

template<typename AttachmentT>
class AttachmentCommand : public ModelCommand<AttachmentT>
{
protected:
    AttachmentCommand(AttachmentT* setlist) :
        ModelCommand<AttachmentT>(setlist)
    {

    }
};

#endif // MODELCOMMAND_H
