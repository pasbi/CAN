#ifndef ORPHANTSETLIST_H
#define ORPHANTSETLIST_H

#include "Database/EventDatabase/setlist.h"

class Event;
class EventDatabase;
class Project;

/**
 * @brief The OrphantSetlist class is a convenience class for temporary setlists.
 *   you neither need a Project, Event or EventDatabase.
 */
class OrphantSetlist : public Setlist
{
public:
    // normally, event stores the label.
    OrphantSetlist( const QString& label );
    Event* event() const { return m_event; }

    ~OrphantSetlist();

private:
    Event* createEvent(const QString& label);
    Event* m_event;
    Project* m_project;
    EventDatabase* m_eventDatabase;

};

#endif // ORPHANTSETLIST_H
