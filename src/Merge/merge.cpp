#include "merge.h"
#include <QFile>
#include <QMessageBox>
#include "mergedialog.h"
#include "global.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/SongDatabase/songdatabase.h"

Merge::Merge(Project* masterProject, const QString &slaveFilename, QWidget* dialogParent) :
    m_masterProject(masterProject),
    m_slaveProject(openProject(slaveFilename)),
    m_dialogParent(dialogParent)
{
    if (openMergeDialog())
    {
        performMerge();
    }
}

Merge::~Merge()
{
    delete m_slaveProject;
    m_slaveProject = nullptr;
}

void Merge::warning(const QString &message)
{
    if (m_dialogParent)
    {
        QMessageBox::warning(m_dialogParent, QWidget::tr("Merge error"), message, QMessageBox::Ok, QMessageBox::Ok);
    }
    else
    {
        qWarning() << "Merge error: " << message;
    }
}

Project* Merge::openProject(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        warning(QString("Cannot open file %1 for reading").arg(filename));
        return nullptr;
    }
    else
    {
        QByteArray data = file.readAll();

        switch (Project::isValid(data))
        {
        case Project::Valid:
        {
            QDataStream stream(data);
            Project* project = new Project();
            stream >> *project;
            return project;
        }
        case Project::InvalidHash:
        case Project::InvalidKey:
        default:
            warning(QString(QWidget::tr("Cannot open %1. Unknown file format.")).arg(filename));
            return nullptr;
        }
    }
}

bool Merge::isValid() const
{
    return m_masterProject && m_slaveProject;
}

bool Merge::openMergeDialog()
{
    MergeDialog dialog(m_dialogParent);
    dialog.setMerger(this);
    int code = dialog.exec();

    m_songMergeItems = dialog.songMergeItems();
    m_eventMergeItems = dialog.eventMergeItems();

    return code == QDialog::Accepted;
}


Attachment* Merge::findAttachment(const void *pointer, Song *song)
{
    for (Attachment* attachment : song->attachments())
    {
        if (attachment == pointer)
        {
            return attachment;
        }
    }
    return nullptr;
}

template<class T> T* findItem(const void *pointer, Database<T>* masterDatabase, Database<T>* slaveDatabase)
{
    for (T* song : slaveDatabase->items())
    {
        if (pointer == song)
        {
            return song;
        }
    }

    for (T* song : masterDatabase->items())
    {
        if (pointer == song)
        {
            return song;
        }
    }

    return nullptr;
}

template<class T> Database<T>* findDatabase(Project*)
{
    return nullptr;
}

template<> Database<Song>* findDatabase(Project* project)
{
    return project->songDatabase();
}

template<> Database<Event>* findDatabase(Project* project)
{
    return project->eventDatabase();
}

template<class T>
void performDatabaseItemMerge(Project* masterProject, Project* slaveProject, QList<MergeInfo<T>> mergeInfos)
{
    Database<T>* masterDatabase = findDatabase<T>(masterProject);
    Database<T>* slaveDatabase = findDatabase<T>(slaveProject);

    for (const MergeInfo<T>& merge : mergeInfos)
    {
        T* item = nullptr;
        switch (merge.action())
        {
        case MergeInfoBase::AddItemAction:
        {
            item = findItem<T>(merge.dataPointer(), masterDatabase, slaveDatabase);
            Q_ASSERT(item);
            masterDatabase->insertItem( item->copy(masterDatabase) );
            masterProject->setCanClose(false);
            break;
        }
        case MergeInfoBase::DeleteItemAction:
        {
            item = findItem<T>(merge.dataPointer(), masterDatabase, slaveDatabase);
            Q_ASSERT(item);
            masterDatabase->removeItem(item);  //TODO this may fail if the song is in use somewhere.
            masterProject->setCanClose(false);
            break;
        }
        case MergeInfoBase::NoAction:
        default:
            ;
        }
    }
}


void Merge::performAttachmentMerge()
{

}


void Merge::performMerge()
{
    performDatabaseItemMerge<Song>(m_masterProject, m_slaveProject, m_songMergeItems);
    performDatabaseItemMerge<Event>(m_masterProject, m_slaveProject, m_eventMergeItems);
    performAttachmentMerge();
}
