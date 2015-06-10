#include "orphantsetlist.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/eventdatabase.h"

#include "project.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/eventdatabase.h"


OrphantSetlist::OrphantSetlist( const QString& label ) :
    Setlist( createEvent(label) )
{
}

OrphantSetlist::~OrphantSetlist()
{
//  delete m_event;   // is deleted automatically since   this->QObject::child() == m_event.
    delete m_eventDatabase;
    delete m_project;
}

Event* OrphantSetlist::createEvent(const QString& label)
{
    m_project = new Project();
    m_eventDatabase = new EventDatabase( m_project );
    m_event = new Event( m_eventDatabase );
    m_event->setLabel( label );

    return m_event;
}
