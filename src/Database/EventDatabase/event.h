#ifndef DATE_H
#define DATE_H

#include "taggable.h"
#include <QJsonObject>
#include <QDateTime>
#include <QObject>

class EventDatabase;
class Date : public QObject, public Taggable
{
public:
    ENUM( Type,  Rehearsal, Gig, Other )
    Date( EventDatabase* database, const QDateTime& beginning, const QDateTime& ending, Type type, const QString & label = "");

    Type type() const { return m_type; }
    QString label() const { return m_label; }
    QDateTime beginning() const { return m_beginning; }
    QDateTime ending() const { return m_ending; }
    EventDatabase* database() const { return m_database; }

    bool restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;

    static QString typeString(Type type, bool translated = false);

    void setLabel( const QString & label ) { m_label = label; }
    void setBeginning( const QDateTime & beginning ) { m_beginning = beginning; }
    void setType( Type type ) { m_type = type; }


private:
    EventDatabase* m_database;
    QDateTime m_beginning, m_ending;
    Type m_type;
    QString m_label;

    static const Qt::DateFormat DATE_TIME_FORMAT;

};

#endif // DATE_H
