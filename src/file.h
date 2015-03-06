#ifndef FILE_H
#define FILE_H

#include <QStringList>
#include <QList>
#include <QFile>


struct Conflict
{
    Conflict( const QString & type,     // e.g. "Song", "Song/Attachment", ...
              const QString & local,
              const QString & remote,
              const int       lineNumberBegin,
              const int       lineNumberEnd   ) :
        m_local( local ),
        m_remote( remote ),
        m_type( type ),
        m_lineNumberBegin( lineNumberBegin ),
        m_lineNumberEnd( lineNumberEnd )
    {
    }

    const QString m_local;
    const QString m_remote;
    const QString m_type;
    const int m_lineNumberBegin;
    const int m_lineNumberEnd;
    mutable QString m_custom;
    enum ResolvePolicy { Undefined, KeepLocal, KeepRemote, KeepCustom };
    mutable ResolvePolicy m_resolvePolicy = Undefined;
};

class File
{
public:
    File( const QString & filename );

    QString content() const
    {
        return m_content.join("\n");
    }

    QString filename() const
    {
        return m_filename;
    }

    const QList<Conflict> & conflicts() const
    {
        return m_conflicts;
    }

    void resolveConflicts();
    bool save() const;


private:
    enum State { NoConflict, InLocal, InRemote };

    QStringList m_content;
    const QString m_filename;
    QList<Conflict> m_conflicts;

//    const QString filterLocalContent(); // removes all conflicts by dropping remote version
    QList<Conflict> findConflicts();
    QString readFile( const QString & filename );
    QString determineType( int lineNumber );


};


#endif // FILE_H
