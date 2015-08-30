#include "command.h"

Command::Command(Type type) :
    QUndoCommand(),
    m_type(type)
{
}
