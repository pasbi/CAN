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
    m_identities.append( identity );
}

void IdentityManager::removeIdentity( const int i )
{
    m_identities.removeAt( i );
}

void IdentityManager::edit(int i, const QString & name, const QString & email, const QString& loginName, const QString & password )
{
    m_identities[i].setName(name);
    m_identities[i].setEmail(email);
    m_identities[i].setLoginName(loginName);
    m_identities[i].setPassword(password);
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
        ids << (QStringList() << i.name() << i.email() << i.loginName() << i.password());
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
                                  v.toStringList()[1],
                                  v.toStringList()[2],
                                  v.toStringList()[3] );
    }

    m_currentIndex = config.value("CurrentIdentity").toInt();
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

void IdentityManager::setIdentities(const QList<Identity> &identities)
{
    m_identities.clear();
    for (const Identity & i : identities)
    {
        if (i.isValid())
        {
            m_identities << i;
        }
    }
}

Identity IdentityManager::identity( const int i ) const
{
    Identity id = m_identities[i];
    return id;
}
