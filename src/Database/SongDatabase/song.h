#ifndef SONG_H
#define SONG_H

#include <QList>
#include <QDateTime>

#include "Program/program.h"
#include "Database/databaseitem.h"
#include "chord.h"
#include "pedanticmap.h"

class SongDatabase;
class Attachment;
template<typename T> class Database;
class Song : public DatabaseItem<Song>
{
    Q_OBJECT
public:
    Song(Database<Song> *database);
    ~Song();

    enum Label { NoLabel, Acoustic, Normal, AcousticNormal };
    enum State { NoState, Inactive, NeedsPractice, Works };

    /////////////////////////////////////////////////
    ////
    ///  Attributes
    //
    /////////////////////////////////////////////////
private:
    PedanticVariantMap m_attributes;

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
    QString title() const;
    QString artist() const;
    QTime duration() const;
    QString description() const;
    QDateTime creationTime() const;
    Label label() const;
    State state() const;
    QStringList singers() const;
    QStringList soloPlayers() const;
    QString singersDisplay() const { return singers().join(", "); }
    QString soloPlayersDisplay() const { return soloPlayers().join(", "); }
    QString stateDisplay() const;
    QString labelDisplay() const;
    Chord key() const;
    QString comments() const;
    PedanticVariantMap attributes() const;
    QString attributeDisplay(const QString& key) const;
    void setAttributes(const PedanticVariantMap &attributes);

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


public:
    static QStringList stateNames();
    static QStringList labelNames();

};

Q_DECLARE_METATYPE(Song::Label)
Q_DECLARE_METATYPE(Song::State)

#endif // SONG_H
