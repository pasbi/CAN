#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include "persistentobject.h"
#include "creatable.h"

class PDFCreator;
class Song;
class Attachment : public QObject, public PersistentObject, public Creatable
{
    Q_OBJECT
public:

    Attachment();
    virtual ~Attachment();

    void setName( const QString & name );
    QString name() const { return m_name; }

    void setSong( Song* song );
    Song* song() const { return m_song; }

    // e.g. audio attachment stops playing
    virtual void deactivate() { }

    void makeNameUnique();

    QJsonObject toJsonObject() const;
    static Attachment* create(const QString& classname, Song *song);

    // see Creatable
    virtual QString type() const = 0;

    QString description() const;

    Attachment* copy() const;
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

    virtual bool isPaintable() const { return false; }

    // this method cannot be pure virtual since an Attachment must not implement this.
    virtual void paint(PDFCreator *);

signals:
    void attachmentRenamed(QString);
private:
    Song* m_song = nullptr;
    QString m_name;
};

#endif // ATTACHMENT_H
