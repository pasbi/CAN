#include "remoteinfo.h"

RemoteInfo::RemoteInfo(const QString& url, const QString& path) :
    m_url(url),
    m_path(path)
{
}

RemoteInfo::RemoteInfo()
{

}

QString RemoteInfo::url() const
{
    return m_url;
}

QString RemoteInfo::path() const
{
    return m_path;
}

bool RemoteInfo::isValid() const
{
    return !m_url.isEmpty() && !m_path.isEmpty();
}

QDataStream& operator <<(QDataStream& out, const RemoteInfo& info)
{
    out << info.m_url;
    out << info.m_path;
    return out;
}

QDataStream& operator >>(QDataStream& in, RemoteInfo& info)
{
    in >> info.m_url;
    in >> info.m_path;
    return in;
}
