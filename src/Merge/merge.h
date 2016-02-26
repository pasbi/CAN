#ifndef MERGE_H
#define MERGE_H

#include <QString>
#include "Project/project.h"
#include "mergeitem.h"
#include "Database/SongDatabase/song.h"
#include "databasemerger.h"

class Event;
class QMimeData;

class Merge
{
public:
    Merge(Project *masterProject, const QString& slaveFilename, QWidget *dialogParent);
    ~Merge();

    bool isValid() const;

    Project* masterProject() const { return m_masterProject; }
    Project* slaveProject() const { return m_slaveProject; }

    QList<MergeItemBase*> songMergeItems() const { return m_songMergeItems; }
    QList<MergeItemBase*> eventMergeItems() const { return m_eventMergeItems; }

    MergeItemBase* decodeMimeData(const QMimeData* mimeData) const;
    QMimeData* encodeMimeData(const MergeItemBase* mergeItemBase) const;

    /**
     * @brief removeMergeItem removes mergeItem from the merger. Ownership of mergeItem is transfered to caller.
     * @param mergeItem removes the mergeItem from the internal list.
     */
    void removeMergeItem(const MergeItemBase *mergeItem);

    /**
     * @brief addMergeItem adds mergeItem to internal list. Ownership of mergeItem is transfered to this.
     * @param mergeItem
     */
    void addMergeItem(MergeItemBase* mergeItem);

private:
    Project* m_masterProject;
    Project* m_slaveProject;

    Project* openProject(const QString& filename);
    QWidget* m_dialogParent;
    void warning(const QString& message);

    bool openMergeDialog();
    void performMerge();

    // the MergeItemBase objects are owned by this.
    QList<MergeItemBase*> m_songMergeItems;
    QList<MergeItemBase*> m_eventMergeItems;
    QMap<MergeItemBase*, Song*> m_attachmentMergeItems;

    void performSongMerge();
    void performAttachmentMerge();
    void performEventMerge();

    Song* findSong(const void *pointer);
    Event* findEvent(const void* pointer);
    Attachment* findAttachment(const void* pointer, Song* song);

    DatabaseMerger<Song> m_songMerger;
    DatabaseMerger<Event> m_eventMerger;
};

#endif // MERGE_H
