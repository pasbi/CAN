#ifndef DATEDATABASE_H
#define DATEDATABASE_H

#include "Database/database.h"
#include "event.h"
#include <QAbstractTableModel>

class EventDatabase : public QAbstractTableModel, public Database
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
    int rowCount(const QModelIndex &parent) const;
    int rowCount() const { return rowCount(QModelIndex()); }
    int columnCount(const QModelIndex &parent) const;
    int columnCount() const { return columnCount(QModelIndex()); }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void notifyDataChanged(const QModelIndex &index);
    void notifyDataChanged(const QModelIndex & start, const QModelIndex & end);
    void notifyDataChanged( const Event *event );
    Event* eventAtIndex(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data( const int row, const int column, const int role);

    void insertEvent( Event* event, const int index);
    void appendEvent( Event* event );
    bool insertRows(int row, int count, const QModelIndex &parent);
    int  removeEvent( Event* event );
    bool removeRows(int row, int count, const QModelIndex &parent);


    /////////////////////////////////////////////////
    ////
    ///  Converting to and from JsonObject
    //
    /////////////////////////////////////////////////
public:
    bool restoreFromJsonObject(const QJsonObject & object);
    QJsonObject toJsonObject() const;


    QList<Event*> events() const { return m_events; }

public slots:
    void reset();

private:
    QList<Event*> m_events;
    QList<Event*> m_tmpEventBuffer;

signals:
    void eventAdded(int, Event*);
    void eventRemoved(int);

private:
    friend class EventDatabaseEditEventCommand;
    bool setData_( const QModelIndex & index, const QVariant& value, int role );
};

#endif // DATEDATABASE_H
