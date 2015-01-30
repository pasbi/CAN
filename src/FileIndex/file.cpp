#include "file.h"
#include <QCryptographicHash>

File::File(Dir* parent, const QString &path) : FileIndexEntry(parent, path)
{
}

FileIndexEntry* File::find(QString &filepath)
{
    if (filepath == path())
    {
        filepath = "";
        return this;
    }
    else
    {
        return NULL;
    }
}

void File::updateHash()
{
    QFile file(absolutePath());

    if (file.open(QIODevice::ReadOnly))
    {

        m_hash = QCryptographicHash::hash( file.readAll(),
                                           QCryptographicHash::Sha3_512 );
    }
    else
    {
        m_hash.clear();
    }

}
