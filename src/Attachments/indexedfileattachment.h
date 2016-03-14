#ifndef INDEXEDFILEATTACHMENT_H
#define INDEXEDFILEATTACHMENT_H

#include "attachment.h"

/**
 * @brief The IndexedFileAttachment class is the base class for all attachments that hold blobs like mp3 or pdf files.
 *  Its file is identified by a hash value of the file content.
 *  This class depends on FileIndex whith which the filename can be retrieved.
 *  Note that this never saves the actual file nor the filename but an hash value.
 */
class IndexedFileAttachment : public Attachment
{
    Q_OBJECT
protected:
    IndexedFileAttachment();

public:

    enum IndexedFileStatus { NoAudioAttachment = 0, NoFileSet = 1, FileNotAvailable = 2, FileAvailable = 3};
    IndexedFileStatus status() const;

    QString filename() const;
    QByteArray hash() const;

public:
    virtual QStringList acceptedEndings() const = 0;

public slots:
    bool setHash(QByteArray hash);
    bool setFilename(QString filename );
    virtual void open() = 0;

signals:
    void hashChanged( QByteArray hash );

};

#endif // INDEXEDFILEATTACHMENT_H
