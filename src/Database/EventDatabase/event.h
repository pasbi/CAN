#ifndef DATE_H
#define DATE_H

#include "taggable.h"
#include <QJsonObject>
#include <QDateTime>
#include <QObject>

class EventDatabase;
class Event : public QObject, public Taggable
{
public:
    ENUM( Type,  Rehearsal, Gig, Other )
    Event( EventDatabase*   database,
           const QDateTime& beginning = QDateTime(),
           const QDateTime& ending    = QDateTime(),
           Type             type      = Other,
           const QString &  label = "");

    Type type() const { return m_type; }
    QString label() const { return m_label; }
    QDateTime beginning() const { return m_beginning; }
    QDateTime ending() const { return m_ending; }
    QString notices() const { return m_notices; }
    EventDatabase* database() const { return m_database; }

    bool restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;

    static QString typeString(Type type, bool translated = false);

    void setLabel( const QString & label ) { m_label = label; }
    void setBeginning( const QDateTime & beginning ) { m_beginning = beginning; }
    void setEnding( const QDateTime & ending ) { m_ending = ending; }
    void setType( Type type ) { m_type = type; }
    void setNotice( const QString & notice );


private:
    EventDatabase* m_database;
    QDateTime m_beginning, m_ending;
    Type m_type;
    QString m_label;
    QString m_notices;

    static const Qt::DateFormat DATE_TIME_FORMAT;

};

#endif // DATE_H
