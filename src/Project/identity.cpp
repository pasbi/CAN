#include "identity.h"
#include "git.h"
#include "gitutil.h"

Identity::Identity(const QString &name, const QString &email, const QString &loginName, const QString &password) :
    m_name( name ),
    m_email( email ),
    m_loginName( loginName ),
    m_password( password )
{
}

Identity::Identity() :
    m_name(),
    m_email(),
    m_loginName(),
    m_password()
{

}

git_signature* Identity::signatureNow() const
{
    git_signature* sign;
    if (!CHECK_GIT( git_signature_now(&sign, CSTR(m_name), CSTR(m_email)) ))
    {
        qWarning() << "Failed to create identity " << m_name << m_email;
        return NULL;
    }
    return sign;
}

QString Identity::name() const
{
    return m_name;
}

QString Identity::email() const
{
    return m_email;
}

QString Identity::loginName() const
{
    return m_loginName;
}

QString Identity::password() const
{
    return m_password;
}


void Identity::setName( const QString & name )
{
    m_name = name;
}

void Identity::setEmail( const QString & email )
{
    m_email = email;
}

void Identity::setLoginName( const QString & loginName )
{
    m_loginName = loginName;
}

void Identity::setPassword( const QString & password )
{
    m_password = password;
}

bool Identity::isValid() const
{
    return !m_name.isEmpty() && !m_email.isEmpty();
}


