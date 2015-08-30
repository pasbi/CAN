#include "midicontroller.h"
#include "global.h"

DEFN_CONFIG( MidiController, "Midi" );

// this option is off by one. I.e. value 1 represents Midi-Channel #0, ..., value 8 represents Midi-Channel #7.
CONFIGURABLE_ADD_DEFEATABLE_ITEM(  MidiController,
                        Channel,
                        QT_TRANSLATE_NOOP( "ConfigurableItem", "Channel" ),
                        QT_TRANSLATE_NOOP( "ConfigurableItem", "ConfigurableItem/MidiCommand/Channel" ),
                        0,
                        false,
                        ConfigurableItemOptions::SpinBoxOptions( 1, 8, 1 ) );



MidiCommand::Channel MidiCommand::defaultChannel = MidiCommand::C_00;
MidiController* MidiController::m_singleton = NULL;

MidiController::MidiController()
{
    reinit();
}

MidiController* MidiController::singleton()
{
    if (!m_singleton)
    {
        m_singleton = new MidiController();
    }
    return m_singleton;
}

void MidiController::reinit()
{
    // reinitialize midi connection, e.g. open /dev/midi{0-9}
    m_midiDevice.close();
    m_midiDevice.setFileName( "/dev/midi1" );
    if (!m_midiDevice.open( QIODevice::WriteOnly ))
    {
        qWarning() << "No Mididevice";
    }
}

bool MidiController::connected()
{
    return m_midiDevice.isOpen();
}

void MidiController::send(const Program &program)
{
    // first, select Bank
    uchar bank = program.bank();
    send( MidiCommand( MidiCommand::ControlChange, 32, bank ) );

    // then, select program
    if (program.program() > 99)
    {
        qWarning() << "Program number out of supported range (1..100)";
    }
    else
    {
        uchar progNum = program.program();
        send( MidiCommand( MidiCommand::ProgramChange, 0, progNum ));
    }
}

void MidiController::send(const MidiCommand& command)
{
    if (!connected())
    {
        qWarning() << "Cannot send midi command when no device is connected.";
    }
    else
    {
        u_int8_t buffer[3] = { command.statusByte, command.dataByte_1, command.dataByte_2 };
        m_midiDevice.write( (char*) buffer, 3 );
        m_midiDevice.flush();
    }
}
