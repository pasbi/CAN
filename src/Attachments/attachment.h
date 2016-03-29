#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include "creatable.h"
#include "Database/databaseitem.h"

class AbstractRenderer;
class Song;
class Attachment : public DatabaseItem<Attachment>, public Creatable
{
    Q_OBJECT
public:

    Attachment();
    virtual ~Attachment();

    void setName( const QString & name );
    QString name() const;

    void setSong( Song* song );
    Song* song() const { return m_song; }

    // e.g. audio attachment stops playing
    virtual void deactivate() { }

    void makeNameUnique();

    static Attachment* create(const QString& classname);

    // see Creatable
    virtual QString type() const = 0;

    QString label() const;

    virtual bool isPaintable() const { return false; }

    // this method cannot be pure virtual since an Attachment must not implement this.
    virtual void paint(AbstractRenderer *);

    QString attributeDisplay(const QString &key) const;

signals:
    void attachmentRenamed(QString);
private:
    Song* m_song = nullptr;

};

#endif // ATTACHMENT_H
