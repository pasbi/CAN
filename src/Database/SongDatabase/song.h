#ifndef SONG_H
#define SONG_H

#include "taggable.h"
#include <QList>
#include "Attachments/attachment.h"

class Song : public Taggable
{
public:
    Song();

    void restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;


    /////////////////////////////////////////////////
    ////
    ///  Attributes
    //
    /////////////////////////////////////////////////
    QString title() const;
    void setTitle(const QString & title);

private:
    QList<Attachment*> m_attachments;
    QString m_title;    //TODO generic solution for arbitrary attributes

};

#endif // SONG_H
