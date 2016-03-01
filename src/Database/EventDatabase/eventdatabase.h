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
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //Drag'n'Drop. Do not support any drag/drop operation.
    Qt::DropActions supportedDragActions() const;

    // we need mimeData and dropMimeData for copy/paste
    QString fileNameBase() const { return "event"; }
    QString itemName(int n) const;
};

#endif // DATEDATABASE_H
