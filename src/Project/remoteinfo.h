#ifndef REMOTEINFO_H
#define REMOTEINFO_H

#include <QString>

class RemoteInfo
{
public:
    RemoteInfo(const QString& url, const QString& path);
    RemoteInfo();

    QString url() const;
    QString path() const;
    bool isValid() const;

private:
    QString m_url;
    QString m_path;

    friend QDataStream& operator <<(QDataStream& out, const RemoteInfo& info);
    friend QDataStream& operator >>(QDataStream& in, RemoteInfo& info);
};

QDataStream& operator <<(QDataStream& out, const RemoteInfo& info);
QDataStream& operator >>(QDataStream& in, RemoteInfo& info);

#endif // REMOTEINFO_H
