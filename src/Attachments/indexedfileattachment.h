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
    // we must rely on the implcit sharing of QByteArray/QString since slots are hardly realizable with const &
    bool setHash(QByteArray hash);
    bool setFilename(QString filename );

protected slots:
    virtual void open() = 0;

private:
    QByteArray m_hash;

};

#endif // INDEXEDFILEATTACHMENT_H
