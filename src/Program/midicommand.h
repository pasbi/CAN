#ifndef MIDICOMMAND_H
#define MIDICOMMAND_H

#include <Qt>

class MidiCommand
{

public:
    enum Channel { C_00 =  0, C_01 =  1, C_02 =  2, C_03 =  3,
                   C_04 =  4, C_05 =  5, C_06 =  6, C_07 =  7,
                   C_08 =  8, C_09 =  9, C_10 = 10, C_11 = 11,
                   C_12 = 12, C_13 = 13, C_14 = 14, C_15 = 15 };
    enum Type { NoteOff = 0, NoteOn = 1, PolyphonicAftertouch = 2, ControlChange = 3,
                ProgramChange = 4, Aftertouch = 5, PitchBendChange = 6, ChannelModeMessage = 7 };
    MidiCommand( Channel channel, Type type, uchar dataByte_1, uchar dataByte_2 = 0 );
    MidiCommand( Type type, uchar dataByte_1, uchar dataByte_2 = 0 );

    quint8 statusByte;
    quint8 dataByte_1;
    quint8 dataByte_2;

private:
    void polishBytes();

public:
    static Channel defaultChannel;
};

#endif // MIDICOMMAND_H
