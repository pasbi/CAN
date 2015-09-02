#ifndef SONG_H
#define SONG_H

#include <QList>
#include <QDateTime>

#include "taggable.h"
#include "Program/program.h"
#include "Database/databaseitem.h"

class Attachment;
template<typename T> class Database;
class Song : public DatabaseItem<Song>
{
    Q_OBJECT
public:
    Song(Database<Song> *database);
    ~Song();

public:
    bool restoreFromJsonObject(const QJsonObject &json);
    QJsonObject toJsonObject() const;

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
    void setTitle(const QString& title);
    void setArtist(const QString& artist);
    void setDuration(const QTime& duration);
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
    void attributeChanged();

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
