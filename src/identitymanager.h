#ifndef IDENTITYMANAGER_H
#define IDENTITYMANAGER_H

#include "../../ZipGit/src/identity.h"
#include <QList>
#include "configurable.h"
#include <QUndoCommand>
#include <QUndoStack>

class IdentityManager
{
    DECL_CONFIG( FileIndex )
public:
    IdentityManager();

    void addIdentity(Identity identity );
    void removeIdentity( const int i );
    QStringList identities( ) const;
    Identity identity( const int i ) const;
    int size() const { return m_identities.size(); }
    void edit(int i, const QString & name, const QString & email, const QString &loginName, const QString &password);
    QString identityString(const int index) const;

    Identity currentIdentity() const;
    int currentIdentityIndex() const { return m_currentIndex; }

    void setIdentities( const QList<Identity> & identities );


    void restore();
    void save() const;

public slots:
    void setCurrentIndex( int i ) {  m_currentIndex = i; }

private:
    QList<Identity> m_identities;
    int m_currentIndex = -1;
    static QString idToString( const Identity & i ) { return  QString("%1 [%2]").arg( i.name(), i.email() ); }
};

#endif // IDENTITYMANAGER_H
