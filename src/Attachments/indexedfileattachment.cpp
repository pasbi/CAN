#include "indexedfileattachment.h"
#include "application.h"
#include "FileIndex/fileindex.h"
#include <QFileInfo>

IndexedFileAttachment::IndexedFileAttachment() :
    Attachment()
{

}

QString IndexedFileAttachment::filename() const
{
    if (m_hash.isEmpty())
    {
        return "";
    }
    else
    {
        return app().fileIndex()->filename( m_hash );
    }
}

bool IndexedFileAttachment::setFilename(QString filename)
{
    if ( app().fileIndex()->contains( filename ) )
    {
        m_hash = app().fileIndex()->hash( filename );
        open();
        return true;
    }
    else
    {
        qWarning() << "Cannot set file since " << filename << " is not indexed.";
        return false;
    }
}

bool IndexedFileAttachment::setHash(QByteArray hash)
{
    if ( app().fileIndex()->contains( hash ) || hash.isEmpty() )
    {
        if (hash != m_hash)
        {
            m_hash = hash;
            open();
            emit hashChanged( m_hash );
        }
        return true;
    }
    else
    {
        qWarning() << "Cannot set file since index does not contain " << QString::fromLatin1( hash.toHex() ) << ".";
        return false;
    }
}

void IndexedFileAttachment::deserialize(QDataStream &in)
{
    Attachment::deserialize(in);
    in >> m_hash;
}

void IndexedFileAttachment::serialize(QDataStream &out) const
{
    Attachment::serialize(out);
    out << m_hash;
}



IndexedFileAttachment::IndexedFileStatus IndexedFileAttachment::status() const
{
    if (hash().isEmpty())
    {
        return NoFileSet;
    }
    else if (filename().isEmpty())
    {
        return FileNotAvailable;
    }
    else
    {
        return FileAvailable;
    }
}
