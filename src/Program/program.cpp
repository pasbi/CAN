#include "program.h"
#include <QDataStream>

const Program Program::INVALID = Program();


Program::Program() :
   m_bank(0),
   m_program(0),
   m_valid(false)
{
}

Program::~Program()
{

}


QDataStream& operator<<(QDataStream& out, const Program* program)
{
    out << static_cast<qint32>(program->m_bank);
    out << static_cast<qint32>(program->m_program);
    out << static_cast<qint32>(program->m_valid);

    return out;
}

QDataStream& operator>>(QDataStream& in, Program* program)
{
    qint32 bank, prog, valid;
    in >> bank >> prog >> valid;
    program->m_bank = bank;
    program->m_program = prog;
    program->m_valid = valid;

    return in;
}
