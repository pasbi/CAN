#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <QDateTime>

typedef struct TimeSpan
{
    TimeSpan() {}
    TimeSpan( QDateTime b, QDateTime e) :
        beginning(b),
        ending(e)
    {    }

    QDateTime beginning;
    QDateTime ending;

    bool isValid()
    {
        return beginning <= ending && beginning.isValid() && ending.isValid();
    }
} Timespan;

Q_DECLARE_METATYPE(TimeSpan)


#endif // COMMONTYPES_H
