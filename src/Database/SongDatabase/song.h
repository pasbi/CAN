#ifndef SONG_H
#define SONG_H

#include "taggable.h"
#include <QList>
#include "Attachments/attachment.h"
#include <QVariantMap>
#include <QDateTime>
#include "program.h"

class SongDatabase;
class Song : public QObject, public Taggable
{
    Q_OBJECT
public:
    Song(SongDatabase *database);
    ~Song();
private:
    SongDatabase* m_songDatabase;



public:
    bool restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;
    SongDatabase* database() const { return m_songDatabase; }
    Song* copy() const;


    /////////////////////////////////////////////////
    ////
    ///  Attributes
    //
    /////////////////////////////////////////////////
private:
    QString m_title;
    QString m_artist;
    QDateTime m_creationDateTime;
    QTime m_duration;
public:
    void setTitle(const QString& title) { m_title = title; }
    void setArtist(const QString& artist) { m_artist = artist; }
    void setDuration(const QTime& duration) { m_duration = duration; }
    QString title() const { return m_title; }
    QString artist() const { return m_artist; }
    QTime duration() const { return m_duration; }
    QString description() const { return title(); }
    QDateTime creationTime() const { return m_creationDateTime; }


    /////////////////////////////////////////////////
    ////
    ///  Attachments
    //
    /////////////////////////////////////////////////
private:
    QList<Attachment*> m_attachments;
    void connectAttachment(Attachment* attachment);
public:
    QList<Attachment*> attachments() const { return m_attachments; }
    Attachment* attachment( int i ) const { return m_attachments[i]; }
    QStringList attachmentNames() const;
    int removeAttachment(Attachment *attachment );
    void addAttachment(Attachment *attachment );
    void insertAttachment(Attachment* attachment, int index);


signals:
    void attachmentAdded(int i);
    void attachmentRemoved(int i);
    void attachmentRenamed(int i, QString);

    /////////////////////////////////////////////////
    ////
    ///  Programs
    //
    /////////////////////////////////////////////////
public:
    Program program() const { return m_program; }
    void setProgram( const Program& program ) { m_program = program; }
private:
    Program m_program = Program();



};

#endif // SONG_H
