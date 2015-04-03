#ifndef IDENTITYMANAGER_H
#define IDENTITYMANAGER_H

#include "../../../ZipGit/src/identity.h"
#include <QList>
#include "configurable.h"
#include <QUndoCommand>
#include <QUndoStack>

class IdentityManager;
class IdentityCommand : public QUndoCommand
{
private:
    IdentityManager* m_manager;
public:
    IdentityCommand( IdentityManager* manager );
    IdentityManager* identityManager() const { return m_manager; }

};

class AddIdentityCommand : public IdentityCommand
{
private:
    const Identity m_identity;
    const int m_index;
public:
    AddIdentityCommand( IdentityManager* manager, Identity newIdentity );
    void undo();
    void redo();
};

class RemoveIdentityCommand : public IdentityCommand
{
private:
    const int m_index;
    Identity m_removedIdentity;
public:
    RemoveIdentityCommand( IdentityManager* manager, const int index );
    void undo();
    void redo();

};

class EditIdentityCommand : public IdentityCommand
{
    const int m_index;
    QString m_newName;
    QString m_newEmail;
    QString m_newLoginName;
    QString m_newPassword;
    const QString m_oldName;
    const QString m_oldEmail;
    const QString m_oldLoginName;
    const QString m_oldPassword;
public:
    EditIdentityCommand(IdentityManager*   manager,
                         const int          index,
                         const QString &    newName,
                         const QString &    newEmail,
                         const QString &    newLoginName,
                         const QString &    newPassword    );

    void redo();
    void undo();
    bool mergeWith(const QUndoCommand *other);
    int id() const { return 1; }
};

class IdentityManager
{
    DECL_CONFIG( FileIndex );
public:
    IdentityManager();

    void addIdentity(Identity identity );
    void removeIdentity( const int i );
    QStringList identities( ) const;
    Identity identity( const int i ) const { return m_identities[i]; }
    int size() const { return m_identities.size(); }
    void edit(int i, const QString & name, const QString & email, const QString &loginName, const QString &password);
    QString identityString(const int index) const;

    Identity currentIdentity() const;
    int currentIdentityIndex() const { return m_currentIndex; }


    void restore();
    void save() const;
    void removeInvalidIdentities();
    QUndoStack* undoStack() { return &m_undoStack; }

public slots:
    void setCurrentIndex( int i ) {  m_currentIndex = i; }

private:
    friend class AddIdentity;
    friend class RemoveIdentity;
    friend class EditIdentity;

private:
    QList<Identity> m_identities;
    int m_currentIndex = -1;
    QUndoStack m_undoStack;
    static QString idToString( const Identity & i ) { return  QString("%1 [%2]").arg( i.name(), i.email() ); }
};

#endif // IDENTITYMANAGER_H
