#include "midicommand.h"
#include "global.h"

MidiCommand::Channel MidiCommand::defaultChannel = MidiCommand::C_00;

MidiCommand::MidiCommand( Channel channel, Type type, uchar dataByte_1, uchar dataByte_2 ) :
    statusByte( channel + (type << 4) ),
    dataByte_1( dataByte_1 ),
    dataByte_2( dataByte_2 )
{
    polishBytes();
}

MidiCommand::MidiCommand( Type type, uchar dataByte_1, uchar dataByte_2 ) :
    statusByte( defaultChannel + (type << 4) ),
    dataByte_1( dataByte_1 ),
    dataByte_2( dataByte_2 )
{
    polishBytes();
}

void MidiCommand::polishBytes()
{
    quint8 MSB = (1<<7);
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
