#ifndef MODELCOMMAND_H
#define MODELCOMMAND_H

#include "command.h"
#include <QString>
#include <QDebug>
#include <typeinfo>
#include "global.h"


template<typename T>
class ModelCommand : public Command
{
public:
    ModelCommand(T* model) :
        Command(inferType<T>()),
        m_model(model)
    {
    }

    T* model() const
    {
        return m_model;
    }

    QString itemName() const
    {
        if (Command::EVENT_RELATED_TYPENAMES.contains(typeid(T).name()))
        {
            return CommandTranslator::tr("Event");
        }
        else if (Command::SONG_RELATED_TYPENAMES.contains(typeid(T).name()))
        {
            return CommandTranslator::tr("Song");
        }
        else if (Command::SETLIST_RELATED_TYPENAMES.contains(typeid(T).name()))
        {
            return CommandTranslator::tr("Setlist Item");
        }
        else
        {
            assert(false);
            return "Unexpected Type";
        }
    }

private:
    T* m_model;
};

#endif // MODELCOMMAND_H
