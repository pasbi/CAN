#ifndef MERGE_H
#define MERGE_H

#include <QString>
#include "Project/project.h"
#include "mergetreeitem.h"
#include "Database/SongDatabase/song.h"

class Event;

class Merge
{
public:
    Merge(Project *masterProject, const QString& slaveFilename, QWidget *dialogParent);
    ~Merge();

    bool isValid() const;

    Project* masterProject() const { return m_masterProject; }
    Project* slaveProject() const { return m_slaveProject; }

private:
    Project* m_masterProject;
    Project* m_slaveProject;

    Project* openProject(const QString& filename);
    QWidget* m_dialogParent;
    void warning(const QString& message);

    bool openMergeDialog();
    void performMerge();

    QList<MergeInfo<Song>> m_songMergeItems;
    QList<MergeInfo<Event>> m_eventMergeItems;
    QMap<Song*, MergeInfo<Attachment>> m_attachmentMergeItems;

    void performSongMerge();
    void performAttachmentMerge();
    void performEventMerge();

    Song* findSong(const void *pointer);
    Event* findEvent(const void* pointer);
    Attachment* findAttachment(const void* pointer, Song* song);

};

#endif // MERGE_H
