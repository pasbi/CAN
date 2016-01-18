#ifndef DATE_H
#define DATE_H

#include <QJsonObject>
#include <QDateTime>
#include <QObject>

#include "Database/databaseitem.h"

class Setlist;
template<typename T> class Database;
class Event : public DatabaseItem<Event>
{
    Q_OBJECT
public:
    enum Type { Rehearsal, Gig, Other };


    Event(Database<Event>*   database,
           const QDateTime& beginning = QDateTime::currentDateTime(),
           Type             type      = Rehearsal,
           const QString &  label = "");
    ~Event();

    Type type() const { return m_type; }
    QString label() const { return m_label; }
    QString description() const;
    QDateTime beginning() const { return m_beginning; }
    QString notices() const { return m_notices; }

    void setLabel( const QString & label );
    void setBeginning( const QDateTime & beginning );
    void setType( Type type );
    void setNotice( const QString & notice );

    const Setlist* setlist() const { return m_setlist; }
    Setlist* setlist() { return m_setlist; }

    QStringList textAttributes() const;

    static QStringList eventTypeNames();
    static QString eventTypeName(Type type);

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    QDateTime m_beginning;
    Type m_type;
    QString m_label;
    QString m_notices;

    Setlist* m_setlist;
};


#endif // DATE_H
