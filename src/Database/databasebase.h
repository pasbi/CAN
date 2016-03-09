#ifndef DATABASEBASE_H
#define DATABASEBASE_H

#include <QAbstractTableModel>
#include <QAbstractProxyModel>
#include <QJsonObject>
#include <QJsonDocument>

class Project;

class DatabaseBase : public QAbstractTableModel
{
    Q_OBJECT
protected:
    DatabaseBase(Project *project, QObject *parent = nullptr);
    virtual ~DatabaseBase();
    void reset();

public:
    Project* project() const;

    Qt::DropActions supportedDropActions() const;
    virtual QString itemName(int n) const = 0;

private:
    Project* m_project;

signals:
    void reseted();

protected:
    virtual void serialize(QDataStream& out) const = 0;
    virtual void deserialize(QDataStream& in) = 0;

    friend QDataStream& operator<<(QDataStream& out, const DatabaseBase* database);
    friend QDataStream& operator>> (QDataStream& out, DatabaseBase* database);
};

QDataStream& operator<<(QDataStream& out, const DatabaseBase* database);
QDataStream& operator>>(QDataStream& out, DatabaseBase* database);
#endif // DATABASEBASE_H
