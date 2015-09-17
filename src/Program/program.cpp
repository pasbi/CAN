#include "program.h"

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

void Program::serialize(QDataStream &out) const
{
    out << static_cast<qint32>(m_bank);
    out << static_cast<qint32>(m_program);
    out << static_cast<qint32>(m_valid);
}

void Program::deserialize(QDataStream &in)
{
    qint32 bank, program, valid;
    in >> bank >> program >> valid;
    m_bank = bank;
    m_program = program;
    m_valid = valid;
}
