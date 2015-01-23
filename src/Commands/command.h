#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>

class Command : public QUndoCommand
{
public:
    explicit Command(QUndoCommand *parent = 0);

};

#endif // COMMAND_H
