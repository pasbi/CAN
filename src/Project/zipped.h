#ifndef ZIPPED_H
#define ZIPPED_H

#include <QTemporaryDir>
#include <QDataStream>
#include "file.h"

class Zipped
{
protected:
    Zipped(const QString & ending);
    virtual ~Zipped();

public:
    /**
     * @brief load loads file at path, tries to unzip it into temporary directory.
     * @param path
     * @return
     */
    virtual bool loadZip(const QString & filename);

    /**
     * @brief save zips the temporary directory content and saves it at path.
     * @param path
     * @return
     */
    virtual bool saveZip(const QString & filename);

    QString path() const { return m_tempDir.path(); }
    QString makeAbsolute(const QString & relative) const { return dir().absoluteFilePath(relative); }
    QDir dir() const { return QDir(m_tempDir.path()); }

    QString ending() const { return m_ending; }

    QStringList find() const;

    /**
     * @brief reset removes all files in directory.
     * @return true if all files were removed.
     */
    bool reset( ) const;

    /**
     * @brief resetUserData removes all files in directory but hidden files.
     *  hidden files are files that start with a dot (.), on windows too.
     * @return true if all non hidden files were removed.
     */
    bool resetUserData() const;

    /**
     * @brief loadFromTempDir loads the project from temporary directory.
     * @return
     */
public:
    virtual bool loadFromTempDir() = 0;
    virtual bool saveToTempDir();
    /**
     * @brief onRemoveFile is called when a file is removed.
     * @param filename the absolute filename of the removed file.
     */
    virtual void onRemoveFile( const QString& filename ) const { Q_UNUSED( filename ); }

    /**
     * @brief onAddFile is called when a file is added or its content has changed.
     * @param filename the absolute filename of the added/changed file.
     */
    virtual void onAddFile( const QString& filename ) const { Q_UNUSED( filename ); }

protected:
    /**
     * @brief getFiles returns the content of all files that shall be saved to temp dir.
     * @return
     */
    virtual QList<File> getFiles() const = 0;

private:
    QTemporaryDir m_tempDir;


    static bool serializeDirectory(QDataStream& stream, const QString & path);
    static bool deserializeDirectory(QDataStream& stream, const QString & path);
    const QString m_ending;
};

#endif // ZIPPED_H
