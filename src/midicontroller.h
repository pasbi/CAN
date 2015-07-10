#ifndef MIDICONTROLLER_H
#define MIDICONTROLLER_H

#include <QFile>
#include "program.h"
#include "global.h"
#include "configurable.h"


class MidiCommand
{

public:
    enum Channel { C_00 =  0, C_01 =  1, C_02 =  2, C_03 =  3,
                   C_04 =  4, C_05 =  5, C_06 =  6, C_07 =  7,
                   C_08 =  8, C_09 =  9, C_10 = 10, C_11 = 11,
                   C_12 = 12, C_13 = 13, C_14 = 14, C_15 = 15 };
    enum Type { NoteOff = 0, NoteOn = 1, PolyphonicAftertouch = 2, ControlChange = 3,
                ProgramChange = 4, Aftertouch = 5, PitchBendChange = 6, ChannelModeMessage = 7 };
    MidiCommand( Channel channel, Type type, uchar dataByte_1, uchar dataByte_2 = 0 ) :
        statusByte( channel + (type << 4) ),
        dataByte_1( dataByte_1 ),
        dataByte_2( dataByte_2 )
    {
        polishBytes();
    }

    MidiCommand( Type type, uchar dataByte_1, uchar dataByte_2 = 0 ) :
        statusByte( defaultChannel + (type << 4) ),
        dataByte_1( dataByte_1 ),
        dataByte_2( dataByte_2 )
    {
        polishBytes();
    }

    u_int8_t statusByte;
    u_int8_t dataByte_1;
    u_int8_t dataByte_2;

private:
    void polishBytes()
    {
        u_int8_t MSB = (1<<7);
        statusByte |= MSB;
        if (dataByte_1 & MSB)
        {
            qWarning() << "dataByte_1, MSB shall not be set. Data will be lost.";
            dataByte_1 &= ~MSB;
        }
        if (dataByte_2 & MSB)
        {
            qWarning() << "dataByte_2, MSB shall not be set. Data will be lost.";
            dataByte_2 &= ~MSB;
        }
    }

public:
    static Channel defaultChannel;
};

class MidiController
{
    DECL_CONFIG( MidiController )

private:
    MidiController();
public:
    static MidiController* singleton();

    void send( const MidiCommand & command );
    void send( const Program& program);
    void reinit();
    bool connected();

private:
    static MidiController* m_singleton;
    QFile m_midiDevice;
};

#endif // MIDICONTROLLER_H
