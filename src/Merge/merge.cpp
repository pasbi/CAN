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

Merge::Merge(Project* masterProject, const QString &slaveFilename, QWidget* dialogParent) :
    m_masterProject(masterProject),
    m_slaveProject(openProject(slaveFilename)),
    m_dialogParent(dialogParent),
    m_songMerger( new DatabaseMerger<Song>( masterProject->songDatabase(),
                                            slaveProject()->songDatabase() ) ),
    m_eventMerger(new DatabaseMerger<Event>( masterProject->eventDatabase(),
                                             slaveProject()->eventDatabase() ) )
{
    if (openMergeDialog())
    {
        QList<Song*> undeletableSongs;
        performMerge(undeletableSongs);
        if (undeletableSongs.isEmpty())
        {
            QMessageBox::information( dialogParent,
                                      app().applicationName(),
                                      QWidget::tr("Merge successfull"),
                                      QMessageBox::Ok );
        }
        else
        {
            QStringList warningString;
            warningString << QWidget::tr("Some Songs could not be removed since they are used.");
            warningString << QWidget::tr("Please try to remove them manually.");
            for (const Song* song : undeletableSongs)
            {
                warningString << "\n" + song->label();
            }
            QMessageBox::warning( dialogParent,
                                  app().applicationName(),
                                  warningString.join("\n"),
                                  QMessageBox::Ok );
        }
    }
}

Merge::~Merge()
{
    // slave project is owned.
    delete m_slaveProject;
    m_slaveProject = nullptr;

    delete m_songMerger;
    m_songMerger = nullptr;

    delete m_eventMerger;
    m_eventMerger = nullptr;

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

void Merge::performMerge(QList<Song*>& undeletableSongs)
{
    // very important to perform event merge before song merge

    DatabaseMergerBase::NewPointerTable updatePointers;

    QList<Event*> undeletableEvents;
    m_eventMerger->performMerge( updatePointers, undeletableEvents );
    Q_ASSERT(undeletableEvents.isEmpty());  // all events should be deleteable

    m_songMerger->performMerge( updatePointers, undeletableSongs );

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

//TODO handle attachments




