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

    DatabaseMerger* songDatabaseMerger() const { return m_songMerger; }
    DatabaseMerger* eventDatabaseMerger() const { return m_eventMerger; }

    MergeItem* decodeMimeData(const QMimeData* mimeData) const;
    QMimeData* encodeMimeData(const MergeItem* mergeItemBase) const;

private:
    Project* m_masterProject;
    Project* m_slaveProject;

    Project* openProject(const QString& filename);
    QWidget* m_dialogParent;
    void warning(const QString& message);

    bool openMergeDialog();

    DatabaseMerger* m_songMerger;
    DatabaseMerger* m_eventMerger;
};

#endif // MERGE_H
