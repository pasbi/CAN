#include "indexedfileattachment.h"
#include "application.h"

IndexedFileAttachment::IndexedFileAttachment()
{
}

bool IndexedFileAttachment::fileExists() const
{
    return app().fileIndex().contains( filename() );
}

QString IndexedFileAttachment::filename() const
{
    return app().fileIndex().filename( m_hash );
}

bool IndexedFileAttachment::setFilename(const QString & filename)
{
    if ( app().fileIndex().contains( filename ) )
    {
        m_hash = app().fileIndex().hash( filename );
        return true;
    }
    else
    {
        WARNING << "Cannot set file, since " << filename << " is not indexed.";
        return false;
    }
}

