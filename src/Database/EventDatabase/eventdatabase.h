#ifndef DATEDATABASE_H
#define DATEDATABASE_H

#include "Database/database.h"
#include "event.h"

class EventDatabase : public Database<Event>
{
    Q_OBJECT
public:
    EventDatabase(Project *project);

    /////////////////////////////////////////////////
    ////
    ///  QAbstractTableModel
    //
    /////////////////////////////////////////////////
public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    //Drag'n'Drop. Do not support any drag/drop operation.
    Qt::DropActions supportedDragActions() const;

    // we need mimeData and dropMimeData for copy/paste
    QString fileNameBase() const { return "event"; }
};

#endif // DATEDATABASE_H
