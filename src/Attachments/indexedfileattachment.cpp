#include "indexedfileattachment.h"
#include "application.h"
#include "FileIndex/fileindex.h"
#include <QFileInfo>

IndexedFileAttachment::IndexedFileAttachment() :
    Attachment()
{
    addAttributeKey("hash");
}

QString IndexedFileAttachment::filename() const
{
    QByteArray hash = attribute("hash").toByteArray();
    if (hash.isEmpty())
    {
        return "";
    }
    else
    {
        return app().fileIndex()->filename( hash );
    }
}

bool IndexedFileAttachment::setFilename(QString filename)
{
    if ( app().fileIndex()->contains( filename ) )
    {
        setAttribute("hash", app().fileIndex()->hash( filename ));
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
    QByteArray oldHash = attribute("hash").toByteArray();
    if ( app().fileIndex()->contains( hash ) || hash.isEmpty() )
    {
        if (hash != oldHash)
        {
            setAttribute("hash", hash);
            open();
            emit hashChanged( hash );
        }
        return true;
    }
    else
    {
        qWarning() << "Cannot set file since index does not contain " << QString::fromLatin1( hash.toHex() ) << ".";
        return false;
    }
}

QByteArray IndexedFileAttachment::hash() const
{
    return attribute("hash").toByteArray();
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
