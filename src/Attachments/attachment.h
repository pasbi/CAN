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
    Attachment(const Attachment &) : QObject() {}

    void setName( const QString & name );
    QString name() const { return m_name; }

    void setSong( Song* song );
    Song* song() const { return m_song; }

    QString classname() const { return metaObject()->className(); }

    // copy must be given by argument due to inheritance structure.
    virtual void copy(Attachment* &copied) const = 0;

    void makeNameUnique();

    QJsonObject toJsonObject() const;
    static bool create(const QJsonObject & object, Attachment* &attachment, Song *song);

    virtual QString type() const = 0;

    QString description() const;

signals:
    void attachmentRenamed(QString);
private:
    Song* m_song = NULL;
    QString m_name;
};

#endif // ATTACHMENT_H
