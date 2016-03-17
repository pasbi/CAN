#include "merge.h"
#include <QFile>
#include <QMessageBox>
#include "mergedialog.h"
#include "global.h"
#include "mergeitem.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Attachments/attachment.h"
#include "Database/EventDatabase/setlist.h"
#include "Database/EventDatabase/setlistitem.h"
#include "application.h"

Merge::Merge(Project* masterProject, Project *slaveProject) :
    m_masterProject(masterProject),
    m_slaveProject(slaveProject),
    m_songMerger( new DatabaseMerger<Song>( masterProject->songDatabase(),
                                            slaveProject->songDatabase() ) ),
    m_eventMerger(new DatabaseMerger<Event>( masterProject->eventDatabase(),
                                             slaveProject->eventDatabase() ) )
{
}

Merge::~Merge()
{
    delete m_songMerger;
    m_songMerger = nullptr;

    delete m_eventMerger;
    m_eventMerger = nullptr;
}

bool Merge::isValid() const
{
    return m_masterProject && m_slaveProject;
}

void Merge::performMerge(QList<const void*>& undeletableItems)
{
    // very important to perform event merge before song merge

    DatabaseMergerBase::NewPointerTable updatePointers;

    QList<const void*> undeletableEvents;
    m_eventMerger->performMerge( updatePointers, undeletableEvents );
    Q_ASSERT(undeletableEvents.isEmpty());  // all events should be deleteable

    m_songMerger->performMerge( updatePointers, undeletableItems );

    for (Event* event : masterProject()->eventDatabase()->items())
    {
        for (SetlistItem* setlistItem : event->setlist()->items())
        {
            for (const DatabaseMergerBase::NewPointerTableItem& item: updatePointers)
            {
                const void* master = item.master;
                const void* slave = item.slave;

                if (setlistItem->attribute("song").value<const Song*>() == slave)
                {
                    setlistItem->setAttribute("song", QVariant::fromValue<const Song*>(nullptr));    // set nullptr silently to prevent usage of maybe-deleted old song ptr.
                    Song* song = event->database()->project()->songDatabase()->item(master);
                    Q_ASSERT(song);
                    setlistItem->setSong(song);
                }
            }
        }
    }
}



QString Merge::labelItem(const void *item)
{
    // only Songs may be undeletable. So we can assume item is a SongDatabase member.
    Song* song = nullptr;
    QString origin;
    Song* masterSong = songDatabaseMerger()->masterDatabase()->item(item);
    Song* slaveSong = songDatabaseMerger()->slaveDatabase()->item(item);
    if (masterSong)
    {
        Q_ASSERT(slaveSong == nullptr);
        song = masterSong;
        origin = QObject::tr("Master");
    }
    if (slaveSong)
    {
        Q_ASSERT(masterSong == nullptr);
        song = slaveSong;
        origin = QObject::tr("Other");
    }

    return QString("%1: %2").arg(origin).arg(song->label());
}





