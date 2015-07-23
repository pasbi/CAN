#ifndef IDENTITY_H
#define IDENTITY_H

#include <QString>
#include "git.h"

class git_signature;
class Identity
{
public:
    Identity(const QString & name, const QString & email, const QString & loginName, const QString & password );
    Identity();

    git_signature* signatureNow() const;
    QString name() const;
    QString email() const;
    QString loginName() const;
    QString password() const;
    void setName( const QString & name );
    void setEmail( const QString & email );
    void setLoginName( const QString & loginName );
    void setPassword( const QString & password );
    bool isValid() const;

private:
    QString m_name;
    QString m_email;
    QString m_loginName;
    QString m_password;

};

#endif // IDENTITY_H
