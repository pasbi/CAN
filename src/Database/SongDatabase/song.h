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
    static const QStringList ATTRIBUTE_KEYS;
    QVariant attribute(const QString& key) const;
    QVariant attribute(int i) const;
    void setAttribute(const QString& key, const QVariant& value);
    void setAttribute(int i, const QVariant &value);
    QString attributeDisplay(const QString& key) const;
    QString attributeDisplay(int i) const;


    QStringList textAttributes() const; //TODO can this be removed?

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
REGISTER_META_TYPE_STREAM_OPERATORS(Song::, Label)
REGISTER_META_TYPE_STREAM_OPERATORS(Song::, State)
QDataStream& operator <<(QDataStream& out, const Song::State& state);
QDataStream& operator >>(QDataStream& in,        Song::State& state);
QDataStream& operator <<(QDataStream& out, const Song::Label& state);
QDataStream& operator >>(QDataStream& in,        Song::Label& state);

#endif // SONG_H
