#include "midicontroller.h"
#include "global.h"
#include "program.h"
#include "midicommand.h"
#include <QFileInfo>

MidiController* MidiController::m_singleton = nullptr;

const QString MidiController::MIDI_DEVICE_FILENAME = "/dev/midi1";

MidiController::MidiController()
{
    reinit();
    initFileSystemWatcher();
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
    m_midiDevice.close();
    m_midiDevice.setFileName( MIDI_DEVICE_FILENAME );
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
        quint8 buffer[3] = { command.statusByte, command.dataByte_1, command.dataByte_2 };
        m_midiDevice.write( (char*) buffer, 3 );
        m_midiDevice.flush();
    }
}

void MidiController::initFileSystemWatcher()
{
    QString dirname  = QFileInfo(MIDI_DEVICE_FILENAME).path();

    assert(m_midiDeviceWatcher.addPath(dirname));
    QObject::connect( &m_midiDeviceWatcher, &QFileSystemWatcher::directoryChanged, [this](QString)
    {
        if (QFileInfo(MIDI_DEVICE_FILENAME).exists())
        {
            reinit();
        }
    });
}
