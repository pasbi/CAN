#include "looselines.h"

QDataStream& operator<<( QDataStream& stream, const Line& line)
{
    stream << line.content;
    stream << line.spaceToNext;
    return stream;
}

QDataStream& operator>>( QDataStream& stream, Line& line)
{
    QString content;
    int spaceToNext;
    stream >> content >> spaceToNext;
    line.content = content;
    line.spaceToNext = spaceToNext;
    return stream;
}
