#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QByteArray>

/**
 * @brief The File class represents an ordinary file. I.e. it stores the content as QByteArray and the filename (relative or absolute)
 */
class File
{
public:
    File( const QString& filename, const QByteArray& content );
    QString filename;
    QByteArray content;
};

#endif // FILE_H
