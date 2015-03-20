#include "identitymanager.h"
#include <QStringList>

DEFN_CONFIG( IdentityManager, "Identities" );
CONFIGURABLE_ADD_ITEM_HIDDEN( IdentityManager, Identities, QStringList() );
CONFIGURABLE_ADD_ITEM_HIDDEN( IdentityManager, CurrentIdentity, 0 );

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
    config.set( "Identities", QVariant::fromValue( ids ) );
    config.set( "CurrentIdentity", m_currentIndex );
}

void IdentityManager::restore( )
{
    QList<QVariant> ids;
    ids = config.value("Identities").toList();

    m_identities.clear();
    for (const QVariant & v : ids)
    {
        m_identities << Identity( v.toStringList()[0],
                                  v.toStringList()[1] );
    }

    m_currentIndex = config.value("CurrentIdentity").toInt();
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
    identityManager()->m_identities[m_index].setName( m_newName );
    identityManager()->m_identities[m_index].setEmail( m_newEmail );
}

void EditIdentity::undo()
{
    identityManager()->m_identities[m_index].setName( m_oldName );
    identityManager()->m_identities[m_index].setEmail( m_oldEmail );
}

bool similar(const QString & a, const QString & b)
{
    if (a.isEmpty() || b.isEmpty())
    {
        return false;
    }

    // detect edit direction changes (from deleting to adding chars or vice versa)
    static bool increasing;
    if (a.size() > b.size())
    {
        if (!increasing)
        {
            increasing = true;
            return false;
        }
    }
    else
    {
        if (increasing)
        {
            increasing = false;
            return false;
        }
    }
    return true;
}

bool EditIdentity::mergeWith(const QUndoCommand *other)
{
    const EditIdentity* otherEdit = dynamic_cast<const EditIdentity*>(other);
    if (!otherEdit)
    {
        return false;
    }

    if ( similar(m_newEmail, otherEdit->m_newEmail)
         && similar(m_newName, otherEdit->m_newName ))
    {
        m_newEmail = otherEdit->m_newEmail;
        m_newName = otherEdit->m_newName;
        return true;
    }
    else
    {
        return false;
    }

}

void IdentityManager::removeInvalidIdentities()
{
    QList<Identity> ids;
    for ( const Identity & i : m_identities )
    {
        if (i.isValid())
        {
            ids.append( i );
        }
    }
    m_identities = ids;
}

Identity IdentityManager::currentIdentity() const
{
    if (m_currentIndex >= 0 && m_currentIndex < size())
    {
        return m_identities[ m_currentIndex ];
    }
    else
    {
        return Identity();
    }
}
