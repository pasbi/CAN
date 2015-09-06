#include "command.h"

#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"

const QStringList Command::SONG_RELATED_TYPENAMES    = QStringList() << typeid(Song).name()
                                                                     << typeid(Database<Song>).name()
                                                                     << typeid(DatabaseSortProxy<Song>).name();
const QStringList Command::EVENT_RELATED_TYPENAMES   = QStringList() << typeid(Event).name()
                                                                     << typeid(Database<Event>).name()
                                                                     << typeid(DatabaseSortProxy<Event>).name();
const QStringList Command::SETLIST_RELATED_TYPENAMES = QStringList() << typeid(SetlistItem).name()
                                                                     << typeid(Database<SetlistItem>).name()
                                                                     << typeid(DatabaseSortProxy<SetlistItem>).name();


Command::Command(Type type) :
    QUndoCommand(),
    m_type(type)
{
}

Command::~Command()
{
}
