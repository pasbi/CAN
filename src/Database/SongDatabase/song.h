#ifndef SONG_H
#define SONG_H

#include "taggable.h"
#include <QList>
#include "Attachments/attachment.h"
#include <QVariantMap>
#include "song.h"

class SongDatabase;
class Song : public Taggable
{
public:
    Song(SongDatabase *database);
    ~Song();
private:
    SongDatabase* m_songDatabase;



public:
    bool restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;


    /////////////////////////////////////////////////
    ////
    ///  Attributes
    //
    /////////////////////////////////////////////////
private:
    QVariantList m_attributes;
public:
    /**
     * @brief attribute return attribute at index or QVariant if index does not exist.
     * @param index
     * @return
     */
    QVariant attribute(int index) const;

    /**
     * @brief attribute return reference to attribute at index.
     *  creates the attribute if it does not exist.
     * @param index
     * @return
     */
    QVariant& attribute(int index);

    void setAttribute(int index, const QVariant & data);
    void insertAttribute(int index, const QVariant & data);
    void removeAttribute(int index);
    QString title() const;
    void setTitle(const QString & title);


    /////////////////////////////////////////////////
    ////
    ///  Attachments
    //
    /////////////////////////////////////////////////
private:
    QList<Attachment*> m_attachments;
public:
    QList<Attachment*> attachments() const { return m_attachments; }
    Attachment* attachment( int i ) const { return m_attachments[i]; }
    QStringList attachmentNames() const;
    int removeAttachment(Attachment *attachment );
    void addAttachment(Attachment *attachment );
    void insertAttachment(Attachment* attachment, int index);

};

#endif // SONG_H
