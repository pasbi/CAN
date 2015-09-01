#ifndef DATE_H
#define DATE_H

#include <QJsonObject>
#include <QDateTime>
#include <QObject>

#include "taggable.h"
#include "commontypes.h"
#include "setlist.h"

template<typename T> class Database;
class Event : public QObject, public Taggable
{
    Q_OBJECT
public:
    enum Type { Rehearsal, Gig, Other };
    Event( Database<Event>*   database,
           const QDateTime& beginning = QDateTime::currentDateTime(),
           const QDateTime& ending    = QDateTime::currentDateTime(),
           Type             type      = Rehearsal,
           const QString &  label = "");

    Type type() const { return m_type; }
    QString label() const { return m_label; }
    QString description() const;
    QDateTime beginning() const { return m_timeSpan.beginning; }
    QDateTime ending() const { return m_timeSpan.ending; }
    TimeSpan timeSpan() const { return m_timeSpan; }
    QString notices() const { return m_notices; }
    Database<Event>* database() const { return m_database; }

    bool restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;

    static QString typeString(Type type, bool translated = false);

    void setLabel( const QString & label ) { m_label = label; }
    void setBeginning( const QDateTime & beginning ) { m_timeSpan.beginning = beginning; }
    void setEnding( const QDateTime & ending ) { m_timeSpan.ending = ending; }
    void setTimeSpan( const TimeSpan& timeSpan) { m_timeSpan = timeSpan; }
    void setType( Type type ) { m_type = type; }
    void setNotice( const QString & notice );

    const Setlist* setlist() const { return &m_setlist; }
    Setlist* setlist() { return &m_setlist; }

    Event* copy() const;


private:
    Database<Event>* m_database;
    TimeSpan m_timeSpan;
    Type m_type;
    QString m_label;
    QString m_notices;

    Setlist m_setlist;
};


#endif // DATE_H
