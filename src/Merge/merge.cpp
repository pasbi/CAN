#include "merge.h"
#include <QFile>
#include <QMessageBox>
#include "mergedialog.h"
#include "global.h"
#include "mergeitem.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Attachments/attachment.h"

Merge::Merge(Project* masterProject, const QString &slaveFilename, QWidget* dialogParent) :
    m_masterProject(masterProject),
    m_slaveProject(openProject(slaveFilename)),
    m_dialogParent(dialogParent),
    m_songMerger(DatabaseMerger<Song>(masterProject->songDatabase(), slaveProject()->songDatabase())),
    m_eventMerger(DatabaseMerger<Event>(masterProject->eventDatabase(), slaveProject()->eventDatabase()))
{

    m_songMergeItems = m_songMerger.createItems();
    m_eventMergeItems = m_eventMerger.createItems();
    if (openMergeDialog())
    {
        performMerge();
    }
}

Merge::~Merge()
{
    delete m_slaveProject;
    m_slaveProject = nullptr;

    qDeleteAll(m_songMergeItems);
    m_songMergeItems.clear();
    qDeleteAll(m_eventMergeItems);
    m_eventMergeItems.clear();
    qDeleteAll(m_attachmentMergeItems.keys());
    m_attachmentMergeItems.clear();
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
    MergeDialog dialog(this, m_dialogParent);
    int code = dialog.exec();

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

template<class T> T* findDatabaseItem(const void *pointer, Database<T>* masterDatabase, Database<T>* slaveDatabase)
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
void performDatabaseItemMerge(Project* masterProject, Project* slaveProject, const QList<MergeItemBase*>& mergeItems)
{
    Database<T>* masterDatabase = findDatabase<T>(masterProject);
    Database<T>* slaveDatabase = findDatabase<T>(slaveProject);

    for (const MergeItemBase* mergeItem : mergeItems)
    {
        T* item = nullptr;
        switch (mergeItem->action())
        {
        case MergeItemBase::AddItemAction:
        {
            item = findDatabaseItem<T>(mergeItem->pointer(), masterDatabase, slaveDatabase);
            Q_ASSERT(item);
            masterDatabase->insertItem( item->copy(masterDatabase) );
            masterProject->setCanClose(false);
            break;
        }
        case MergeItemBase::DeleteItemAction:
        {
            item = findDatabaseItem<T>(mergeItem->pointer(), masterDatabase, slaveDatabase);
            Q_ASSERT(item);
            masterDatabase->removeItem(item);  //TODO this may fail if the song is in use somewhere.
            masterProject->setCanClose(false);
            break;
        }
        case MergeItemBase::NoAction:
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


MergeItemBase* Merge::decodeMimeData(const QMimeData* mimeData) const
{
    Q_ASSERT(mimeData->hasFormat("merge"));
    QDataStream stream(mimeData->data("merge"));
    quintptr intptr;
    stream >> intptr;
    MergeItemBase* mergeItem = static_cast<MergeItemBase*>(reinterpret_cast<void*>(intptr));
    Q_ASSERT( m_songMergeItems.contains(mergeItem)
              || m_eventMergeItems.contains(mergeItem)
              || m_attachmentMergeItems.contains(mergeItem) );
    return mergeItem;
}

QMimeData* Merge::encodeMimeData(const MergeItemBase* mergeItemBase) const
{
    quintptr intptr;
    intptr = reinterpret_cast<quintptr>(mergeItemBase);
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << intptr;
    QMimeData* mimeData = new QMimeData();
    mimeData->setData("merge", data);
    return mimeData;
}

void Merge::removeMergeItem(const MergeItemBase *mergeItem)
{
    // QList::remove and QMap::remove need non-const pointer
    MergeItemBase* nonconstMergeItem = const_cast<MergeItemBase*>(mergeItem);
    switch (mergeItem->type())
    {
    case MergeItemBase::SongType:
        Q_ASSERT(m_songMergeItems.contains(nonconstMergeItem));
        m_songMergeItems.removeOne(nonconstMergeItem);
        break;
    case MergeItemBase::EventType:
        Q_ASSERT(m_attachmentMergeItems.contains(nonconstMergeItem));
        m_eventMergeItems.removeOne(nonconstMergeItem);
        break;
    case MergeItemBase::AttachmentType:
        Q_ASSERT(m_attachmentMergeItems.contains(nonconstMergeItem));
        m_attachmentMergeItems.remove(nonconstMergeItem);
        break;
    case MergeItemBase::NoType:
    default:
        Q_UNREACHABLE();
    }
}

Attachment* findAttachment(MergeItemBase* mergeItem)
{
    Q_ASSERT(mergeItem->type() == MergeItemBase::AttachmentType);
    if (mergeItem->pointer())
    {
        return static_cast<Attachment*>(mergeItem->pointer());
    }
    else if (mergeItem->masterPointer())
    {
        return static_cast<Attachment*>(mergeItem->pointer());
    }
    else
    {
        Q_UNREACHABLE();
        return nullptr;
    }
}

void Merge::addMergeItem(MergeItemBase *mergeItem)
{
    switch (mergeItem->type())
    {
    case MergeItemBase::SongType:
        m_songMergeItems << mergeItem;
        break;
    case MergeItemBase::EventType:
        m_eventMergeItems << mergeItem;
        break;
    case MergeItemBase::AttachmentType:
        m_attachmentMergeItems.insert(mergeItem, ::findAttachment(mergeItem)->song());
        break;
    case MergeItemBase::NoType:
    default:
        Q_UNREACHABLE();
    }
}
