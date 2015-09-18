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
    bool fileExists() const;
    QString filename() const;
    QByteArray hash() const { return m_hash; }

public:
    virtual QStringList acceptedEndings() const = 0;

public slots:
    bool setHash(QByteArray hash);
    bool setFilename(QString filename );
    virtual void open() = 0;

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

signals:
    void hashChanged( QByteArray hash );

private:
    QByteArray m_hash;

};

#endif // INDEXEDFILEATTACHMENT_H
