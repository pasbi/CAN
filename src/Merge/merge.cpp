#include "merge.h"
#include <QFile>
#include <QMessageBox>
#include "mergedialog.h"
#include "global.h"
#include "mergeitem.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Attachments/attachment.h"

//TODO creating new event and opening project results in crash
//TODO build combineEventDIalog

template<typename T>
QList<DatabaseItemBase*> convertDatabaseItems(const QList<T*>& items)
{
    QList<DatabaseItemBase*> databaseItems;
    for (T* item : items)
    {
        databaseItems << dynamic_assert_cast<DatabaseItemBase*>(item);
    }
    return databaseItems;
}

Merge::Merge(Project* masterProject, const QString &slaveFilename, QWidget* dialogParent) :
    m_masterProject(masterProject),
    m_slaveProject(openProject(slaveFilename)),
    m_dialogParent(dialogParent),
    m_songMerger( new DatabaseMerger( convertDatabaseItems(masterProject->songDatabase()->items()),
                                      convertDatabaseItems(slaveProject()->songDatabase()->items()) ) ),
    m_eventMerger(new DatabaseMerger( convertDatabaseItems(masterProject->eventDatabase()->items()),
                                      convertDatabaseItems(slaveProject()->eventDatabase()->items()) ) )
{
    if (openMergeDialog())
    {
//        performMerge();
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

