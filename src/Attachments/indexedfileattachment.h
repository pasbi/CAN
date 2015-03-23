#ifndef INDEXEDFILEATTACHMENT_H
#define INDEXEDFILEATTACHMENT_H

#include "attachment.h"

/**
 * @brief The IndexedFileAttachment class is the base class for all attachments that hold blobs like mp3 or pdf files.
 */
class IndexedFileAttachment : public Attachment
{
    Q_OBJECT
public:
    IndexedFileAttachment();

    bool fileExists() const;
    QString filename() const;
    QByteArray hash() const { return m_hash; }



    virtual void copy(Attachment* &attachment) const;
    virtual QStringList acceptedEndings() const = 0;

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);


public slots:
    bool setHash(QByteArray hash);
    bool setFilename(QString filename );
    virtual void open() = 0;

signals:
    void hashChanged( QByteArray hash );

private:
    QByteArray m_hash;

};

#endif // INDEXEDFILEATTACHMENT_H
