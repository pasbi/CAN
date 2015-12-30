#ifndef SONG_H
#define SONG_H

#include <QList>
#include <QDateTime>

#include "Program/program.h"
#include "Database/databaseitem.h"
#include "chord.h"
#include <QBitArray>

class SongDatabase;
class Attachment;
template<typename T> class Database;
class Song : public DatabaseItem<Song>
{
    Q_OBJECT
public:
    Song(Database<Song> *database);
    ~Song();

    enum Label { NoLabel, Acoustic, Normal };
    enum State { NoState, Inactive, NeedsPractice, Works };
    static const QStringList STATE_NAMES;
    static const QStringList LABEL_NAMES;

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
    Label m_label;
    State m_state;
    Chord m_key;
    QStringList m_singers;
    QStringList m_soloPlayers;
    QString m_comments;

public:
    void setTitle(const QString& title);
    void setArtist(const QString& artist);
    void setDuration(const QTime& duration);
    void setlabel(Label label);
    void setState(State state);
    void setSingers(const QStringList& singers);
    void setSoloPlayers(const QStringList& soloPlayers);
    void setComments(const QString& comments);
    void setKey(Chord key);
    QString title() const { return m_title; }
    QString artist() const { return m_artist; }
    QTime duration() const { return m_duration; }
    QString description() const { return title(); }
    QDateTime creationTime() const { return m_creationDateTime; }
    Label label() const { return m_label; }
    State state() const { return m_state; }
    QStringList singers() const { return m_singers; }
    QStringList soloPlayers() const { return m_soloPlayers; }
    Chord key() const { return m_key; }
    QString comments() const { return m_comments; }

    QStringList textAttributes() const;

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

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

};
#endif // SONG_H
