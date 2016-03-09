#ifndef PROGRAM_H
#define PROGRAM_H

#include <Qt>

#ifdef Q_OS_LINUX
#define HAVE_PROGRAM
#endif

class Program
{
public:
    Program();
    ~Program();

    const static Program INVALID;

    bool isValid() const { return m_valid; }
    int bank() const { return m_bank; }
    int program() const { return m_program; }
    bool& isValid() { return m_valid; }
    int& bank() { return m_bank; }
    int& program() { return m_program; }

private:
    int m_bank;
    int m_program;
    bool m_valid;

    friend QDataStream& operator<<(QDataStream& out, const Program* program);
    friend QDataStream& operator>>(QDataStream& in,  Program* program);
};

QDataStream& operator<<(QDataStream& out, const Program* program);
QDataStream& operator>>(QDataStream& in,  Program* program);

#endif // PROGRAM_H
