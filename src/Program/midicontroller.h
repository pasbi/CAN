#ifndef MIDICONTROLLER_H
#define MIDICONTROLLER_H

#include <QFile>
#include "configurable.h"

class Program;
class MidiCommand;
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
