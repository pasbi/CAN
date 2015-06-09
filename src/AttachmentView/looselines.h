#ifndef LOOSELINES_H
#define LOOSELINES_H

#include <QByteArray>
#include <QString>
#include <QDataStream>

class Line
{
public:
    QString content;
    qint32 spaceToNext;
};

QDataStream& operator<<( QDataStream& stream, const Line& line);
QDataStream& operator>>( QDataStream& stream, Line& line);
/**
 * @brief The LooseLines class number of lines with arbitrary lines in between.
 */
class LooseLines : public QList<Line>
{
public:
    void add( const QString& line, int spaceToNext ) { append( { line, spaceToNext } ); }
};


#endif // LOOSELINES_H
