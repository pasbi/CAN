#ifndef PROGRAM_H
#define PROGRAM_H

typedef struct Program
{
    Program() :
        bank(0),
        program(0),
        valid(false)
    {

    }

    int bank;
    int program;
    bool valid;

    const static Program INVALID;
} Program;


#endif // PROGRAM_H
