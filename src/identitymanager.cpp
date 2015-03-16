#include "identitymanager.h"
#include <QStringList>

DEFN_CONFIG( IdentityManager, "Identities" );
CONFIGURABLE_ADD_ITEM( IdentityManager, Identities, QStringList(), ConfigurationItemOptions::HiddenInterface() );


IdentityManager::IdentityManager()
{
}

void IdentityManager::addIdentity( Identity identity )
{
    AddIdentity* addIdentity = new AddIdentity( this, identity );
    m_undoStack.push( addIdentity );
}

void IdentityManager::removeIdentity( const int i )
{
    RemoveIdentity* removeIdentity = new RemoveIdentity( this, i );
    m_undoStack.push( removeIdentity );
}

void IdentityManager::edit(int i, const QString & name, const QString & email)
{
    EditIdentity* editIdentity = new EditIdentity( this, i, name, email );
    m_undoStack.push( editIdentity );
}

QStringList IdentityManager::identities( ) const
{
    QStringList is;
    for ( const Identity & i : m_identities )
    {
        is << idToString(i);
    }
    return is;
}

QString IdentityManager::identityString(const int index) const
{
    QString s = idToString(m_identities[index]);
    return s;
}

void IdentityManager::save( ) const
{
    QList<QVariant> ids;
    for (const Identity & i : m_identities )
    {
        ids << (QStringList() << i.name() << i.email());
    }
    config.setItem( "Identities", QVariant::fromValue( ids ) );
}

void IdentityManager::restore( )
{
    QList<QVariant> ids;
    ids = config.item("Identities").toList();

    m_identities.clear();
    for (const QVariant & v : ids)
    {
        m_identities << Identity( v.toStringList()[0],
                                  v.toStringList()[1] );
    }
}





IdentityCommand::IdentityCommand( IdentityManager* manager )
{
    m_manager = manager;
}

AddIdentity::AddIdentity(IdentityManager* manager, Identity newIdentity ) :
    IdentityCommand( manager ),
    m_identity( newIdentity ),
    m_index( manager->size() )
{
}

void AddIdentity::undo()
{
    identityManager()->m_identities.removeAt( m_index );
}

void AddIdentity::redo()
{
    identityManager()->m_identities.append( m_identity );
}

RemoveIdentity::RemoveIdentity( IdentityManager* manager, const int index ) :
    IdentityCommand( manager ),
    m_index( index ),
    m_removedIdentity( identityManager()->m_identities[index] )
{
}

void RemoveIdentity::undo()
{
    identityManager()->m_identities.insert( m_index, m_removedIdentity );
}

void RemoveIdentity::redo()
{
    identityManager()->m_identities.removeAt( m_index );
}

EditIdentity::EditIdentity( IdentityManager* manager, const int index, const QString & newName, const QString & newEmail ) :
    IdentityCommand( manager ),
    m_index( index ),
    m_newName( newName ),
    m_newEmail( newEmail ),
    m_oldName( manager->m_identities[index].name() ),
    m_oldEmail( manager->m_identities[index].email() )
{
}

void EditIdentity::redo()
{
    qDebug() << "REDO";
    identityManager()->m_identities[m_index].setName( m_newName );
    identityManager()->m_identities[m_index].setEmail( m_newEmail );
}

void EditIdentity::undo()
{
    identityManager()->m_identities[m_index].setName( m_oldName );
    identityManager()->m_identities[m_index].setEmail( m_oldEmail );
}

bool EditIdentity::mergeWith(const QUndoCommand *other)
{
    const EditIdentity* otherEdit = dynamic_cast<const EditIdentity*>(other);
    assert( otherEdit );

    m_newEmail = otherEdit->m_newEmail;
    m_newName = otherEdit->m_newName;

    return true;
}
