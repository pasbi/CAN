#ifndef INDEXEDFILEATTACHMENT_H
#define INDEXEDFILEATTACHMENT_H

#include "attachment.h"

/**
 * @brief The IndexedFileAttachment class is the base class for all attachments that hold blobs like mp3 or pdf files.
 *  Its file is identified by a hash value of the file content.
 *  This class depends on FileIndex whith which the filename can be retrieved.
 *  Note that this never saves the actual file nor the filename but an hash value.
 *
 * //TODO
 *  Since the Project and thus this attachment is meant to be used on multiple computers at same time. Thus it may
 * happen that user A sets PDF p1 but user B has a very similar PDF p2.
 * If this constellation happens to annoy significantly, I will implement some kind of history:
 *  - User does not overwrite a hash value but pushes it to a list.
 *  - The first hash in that list that points to an existing file, will be used.
 *  - User must have the chance to clear history or just certain entries.
 *  - User shall enjoy full support of undo/redo capabilities.
 * Problems: Replacement of files is hard (i.e. file p1 is outdated and becomes replaced by p2 by user A. User B will see
 * file p1 since it is listed in history immediately behind p2 and p2 is probably not existent on B's PC.
 * That is, B will probably not notice that p1 is obsolete, which is inacepptable.
 * The solution is, that A removes all obsolete files from the history. But A may forget to do so, since this is not obvious.
 * -> TODO: find a convenient solution for A
 */
class IndexedFileAttachment : public Attachment
{
    Q_OBJECT
public:
    IndexedFileAttachment();
    IndexedFileAttachment(const Attachment& attachment);

    bool fileExists() const;
    QString filename() const;
    QByteArray hash() const { return m_hash; }

protected:
    // copy function of virtual class should not be used outside final-class-implementation.
    virtual void copy(Attachment *&copied) const;

public:
    virtual QStringList acceptedEndings() const = 0;

    virtual QJsonObject toJsonObject() const;
    virtual bool restoreFromJsonObject(const QJsonObject &object);


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
