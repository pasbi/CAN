#ifndef SONG_H
#define SONG_H

#include <QList>
#include <QDateTime>

#include "Program/program.h"
#include "Database/databaseitem.h"
#include "chord.h"

class SongDatabase;
class Attachment;
class AttachmentDatabase;

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
public:
    static const QStringList ATTRIBUTE_KEYS;
    QString attributeDisplay(const QString& key) const;


    /////////////////////////////////////////////////
    ////
    ///  Attachments
    //
    /////////////////////////////////////////////////
private:
    AttachmentDatabase* m_attachmentDatabase;
    void connectAttachment(Attachment* attachment);
public:
    AttachmentDatabase* attachmentDatabase() const;
    QList<Attachment*> attachments() const;
    Attachment* attachment( int i ) const;
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
    Program m_program;

public:
    static QStringList stateNames();
    static QStringList labelNames();
    QString label() const;

    bool canRemove() const;

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

};

Q_DECLARE_METATYPE(const Song*)
//Do not register Song-stream operators. We never want to save song via QVariant!
Q_DECLARE_METATYPE(Song::Label)
REGISTER_META_TYPE_STREAM_OPERATORS(Song::Label, SongLabel)
Q_DECLARE_METATYPE(Song::State)
REGISTER_META_TYPE_STREAM_OPERATORS(Song::State, SongState)

DECLARE_ENUM_STREAM_OPERATORS(Song::State)
DECLARE_ENUM_STREAM_OPERATORS(Song::Label)

#endif // SONG_H
