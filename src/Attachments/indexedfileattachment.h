#ifndef INDEXEDFILEATTACHMENT_H
#define INDEXEDFILEATTACHMENT_H

#include "attachment.h"
#include "poppler/qt5/poppler-qt5.h"

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

    virtual void copy(Attachment* &attachment) const;



private:
    QByteArray m_hash;
    void openDocument();
    Poppler::Document* m_document = NULL;

};

#endif // INDEXEDFILEATTACHMENT_H
