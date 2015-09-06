#ifndef DATE_H
#define DATE_H

#include <QJsonObject>
#include <QDateTime>
#include <QObject>

#include "taggable.h"
#include "commontypes.h"
#include "Database/databaseitem.h"

class Setlist;
template<typename T> class Database;
class Event : public DatabaseItem<Event>
{
    Q_OBJECT
public:
    enum Type { Rehearsal, Gig, Other };
    static const QStringList TYPES;
    static QString typeName(Type type);


    Event( Database<Event>*   database,
           const QDateTime& beginning = QDateTime::currentDateTime(),
           const QDateTime& ending    = QDateTime::currentDateTime(),
           Type             type      = Rehearsal,
           const QString &  label = "");
    ~Event();

    Type type() const { return m_type; }
    QString label() const { return m_label; }
    QString description() const;
    QDateTime beginning() const { return m_timeSpan.beginning; }
    QDateTime ending() const { return m_timeSpan.ending; }
    TimeSpan timeSpan() const { return m_timeSpan; }
    QString notices() const { return m_notices; }

    bool restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;

    void setLabel( const QString & label );
    void setBeginning( const QDateTime & beginning );
    void setEnding( const QDateTime & ending );
    void setTimeSpan( const TimeSpan& timeSpan);
    void setType( Type type );
    void setNotice( const QString & notice );

    const Setlist* setlist() const { return m_setlist; }
    Setlist* setlist() { return m_setlist; }

    QStringList textAttributes() const;

private:
    TimeSpan m_timeSpan;
    Type m_type;
    QString m_label;
    QString m_notices;

    Setlist* m_setlist;
};


#endif // DATE_H
