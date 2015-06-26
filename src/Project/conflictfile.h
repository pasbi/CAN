#ifndef CONFLICTFILE_H
#define CONFLICTFILE_H

#include <QStringList>
#include <QList>
#include <QFile>

class GitRepository;

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
        m_lineNumberEnd( lineNumberEnd ),
        m_resolvePolicy( Undefined )
    {
    }

    const QString m_local;
    const QString m_remote;
    const QString m_type;
    const int m_lineNumberBegin;
    const int m_lineNumberEnd;
    QString m_custom;
    enum ResolvePolicy { Undefined, KeepLocal, KeepRemote, KeepCustom };
    ResolvePolicy m_resolvePolicy;

    Conflict operator=(const Conflict& c) const { return Conflict( c.m_type,
                                                                    c.m_local,
                                                                    c.m_remote,
                                                                    c.m_lineNumberBegin,
                                                                    c.m_lineNumberEnd ); }
};

class ConflictFile
{
public:
    ConflictFile(const GitRepository *project, const QString & filename );

    QString content() const
    {
        return m_content.join("\n");
    }

    QString filename() const
    {
        return m_filename;
    }

    QList<Conflict>& conflicts()
    {
        return m_conflicts;
    }

    void resolveConflicts();
    bool save() const;

    ConflictFile& operator = (const ConflictFile& file) const;


private:
    enum State { NoConflict, InLocal, InRemote };

    QStringList m_content;
    const QString m_filename;
    QList<Conflict> m_conflicts;

    QList<Conflict> findConflicts();
    QString readFile( const QString & filename );
    QString determineType( int lineNumber );

    /**
     * @brief m_project store the associated project to be able to git-add resolved conflicts.
     */
    const GitRepository* m_project = NULL;


};


#endif // CONFLICTFILE_H
