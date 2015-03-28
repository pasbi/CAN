#ifndef DATEDATABASE_H
#define DATEDATABASE_H

#include "Database/database.h"
#include "date.h"
#include <QAbstractTableModel>

class DateDatabase : public QAbstractTableModel, public Database
{
    Q_OBJECT
public:
    DateDatabase(Project *project);

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
    void notifyDataChanged( const Date *date );
    Date* dateAtIndex(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data( const int row, const int column, const int role);


    /////////////////////////////////////////////////
    ////
    ///  Converting to and from JsonObject
    //
    /////////////////////////////////////////////////
public:
    bool restoreFromJsonObject(const QJsonObject & object);
    QJsonObject toJsonObject() const;


    QList<Date*> dates() const { return m_dates; }

public slots:
    void reset();

private:
    QList<Date*> m_dates;
};

#endif // DATEDATABASE_H
