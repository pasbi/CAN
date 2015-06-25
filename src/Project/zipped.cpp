#include "zipped.h"
#include <QStack>
#include <QFileInfo>
#include "global.h"

bool removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

Zipped::Zipped(const QString &ending) :
    m_ending(ending)
{
}

Zipped::~Zipped()
{

}

bool compress(QByteArray & data)
{
    data = qCompress(data);
    return !data.isEmpty();

}

bool uncompress(QByteArray & data)
{
    data = qUncompress(data);
    return !data.isEmpty();
}

bool Zipped::loadZip(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "cannot open " << filename;
        return false;
    }

    QByteArray data = file.readAll();
    if (!uncompress(data))
    {
        qWarning() << "cannot uncompress data in " << filename;
        return false;
    }

    QDataStream stream(&data, QIODevice::ReadOnly);

    reset();
    if (!deserializeDirectory(stream, path()))
    {
        qWarning() << "cannot deserialize " << path();
        return false;
    }

    if (!loadFromTempDir())
    {
        qWarning() << "cannot load from temp dir.";
        return false;
    }

    return true;
}

bool Zipped::saveZip(const QString &filename)
{
    if (!saveToTempDir())
    {
        qWarning() << "cannot save to temp dir.";
        return false;
    }

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);

    if (!serializeDirectory(stream, path()))
    {
        qWarning() << "cannot serialize " << path();
        return false;
    }

    if (!compress(buffer))
    {
        qWarning() << "cannot compress " << filename;
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "cannot open " << filename << " for writing.";
        return false;
    }

    if (file.write(buffer) < 0)
    {
        qWarning() << "cannot write to " << filename;
        return false;
    }

    return true;
}

bool Zipped::reset() const
{
    bool succeeded = true;

    QStringList dirs = dir().entryList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden );
    foreach (const QString & p, dirs)
    {
        QString dirPath = QDir(path()).absoluteFilePath( p );
        if (!QDir( dirPath ).removeRecursively())
        {
            qWarning() << "cannot remove " << dirPath;
            succeeded = false;
        }
        else
        {
        }
    }

    QStringList files = dir().entryList(QDir::Files);
    foreach (const QString & p, files)
    {
        QString filepath = QDir(path()).absoluteFilePath(p);
        if (!QFile( filepath ).remove())
        {
            qWarning() << "cannot remove " << filepath;
            succeeded = false;
        }
        else
        {
        }
    }

    return succeeded;
}

bool Zipped::resetUserData() const
{
    bool succeeded = true;

    QStringList dirs = dir().entryList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden );
    foreach (const QString & p, dirs)
    {
        QString dirPath = QDir(path()).absoluteFilePath( p );
        if ( !p.startsWith(".") )
        {
            if (!QDir( dirPath ).removeRecursively())
            {
                qWarning() << "cannot remove " << dirPath;
                succeeded = false;
            }
            else
            {
            }
        }
    }

    QStringList files = dir().entryList(QDir::Files);
    foreach (const QString & p, files)
    {
        QString filepath = QDir(path()).absoluteFilePath(p);
        if ( !p.startsWith(".") )
        {
            if (!QFile( filepath ).remove())
            {
                qWarning() << "cannot remove " << filepath;
                succeeded = false;
            }
            else
            {
            }
        }
    }

    return succeeded;
}

bool Zipped::serializeDirectory(QDataStream& stream, const QString & path)
{
    // 1. filenames
    // 2. file contents
    // 3. dir names
    // 4. dir contents

    const QStringList filenames = QDir(path).entryList(QDir::Files);
    /* 1 */
    stream << filenames;

    /* 2 */
    foreach (const QString & filename, filenames)
    {
        const QString absPath = QDir(path).absoluteFilePath(filename);
        QFile file(absPath);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Cannot open file " << absPath << " for reading.";
            return false;
        }
        stream << file.readAll();
    }

    const QStringList dirnames = QDir(path).entryList(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot);
    /* 3 */
    stream << dirnames;

    /* 4 */
    foreach (const QString dirname, dirnames)
    {
        const QString absPath = QDir(path).absoluteFilePath(dirname);
        if (!serializeDirectory(stream, absPath))
        {
            return false;
        }
    }

    return true;
}

bool Zipped::deserializeDirectory(QDataStream& stream, const QString & path)
{
    // 1. filenames
    // 2. file contents
    // 3. dir names
    // 4. dir contents

    QStringList filenames, dirnames;



    stream >> filenames;
    foreach (const QString & filename, filenames)
    {
        const QString abspath = QDir(path).absoluteFilePath(filename);
        QFile file(abspath);

        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning() << "cannot create file " << path;
            return false;
        }

        QByteArray filecontent;
        stream >> filecontent;

        if ( file.write(filecontent) < 0 )
        {
            qWarning() << "cannot write to file " << path;
            return false;
        }
    }

    stream >> dirnames;
    foreach (const QString & dirname, dirnames)
    {
        QString abspath = QDir(path).absoluteFilePath(dirname);
        if (!QDir(path).mkdir(dirname))
        {
            qWarning() << "cannot create dir " << abspath;
            return false;
        }

        if (!deserializeDirectory(stream, abspath))
        {
            return false;
        }
    }

    return true;
}

QStringList Zipped::find() const
{
    QStringList files;
    QStack<QString> dirs;

    dirs.push(path());

    while (!dirs.isEmpty())
    {
        QString dirPath = dirs.pop();
        foreach (QString filename, QDir(dirPath).entryList(QDir::Files | QDir::Hidden))
        {
            filename = QDir(dirPath).absoluteFilePath(filename);
            filename = QDir(path()).relativeFilePath(filename);
            files << filename;
        }

        foreach (QString dirname, QDir(dirPath).entryList(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot))
        {
            dirname = QDir(dirPath).absoluteFilePath(dirname);
            dirs.push(dirname);
        }
    }

    return files;
}

