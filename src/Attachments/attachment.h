#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include "taggable.h"
#include "creatable.h"

class Song;
class Attachment : public QObject, public Taggable, public Creatable
{
    Q_OBJECT
public:
    Attachment();

    void setName( const QString & name );
    QString name() const { return m_name; }

    void setSong( Song* song );
    Song* song() const { return m_song; }

    QString classname() const { return metaObject()->className(); }

    void makeNameUnique();


    QJsonObject toJsonObject() const;
    static bool create(const QJsonObject & object, Attachment* &attachment);
signals:
    void attachmentRenamed(QString);
private:
    Song* m_song = NULL;
    QString m_name;
};

#endif // ATTACHMENT_H
