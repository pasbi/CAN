#ifndef INDEXEDFILEATTACHMENT_H
#define INDEXEDFILEATTACHMENT_H

#include "attachment.h"

/**
 * @brief The IndexedFileAttachment class is the base class for all attachments that hold blobs like mp3 or pdf files.
 */
class IndexedFileAttachment : public Attachment
{

public:
    IndexedFileAttachment();

    bool fileExists() const;
    QString filename() const;
    bool setFilename(const QString & filename);

private:
    QByteArray m_hash;

};

#endif // INDEXEDFILEATTACHMENT_H
