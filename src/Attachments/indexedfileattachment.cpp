#include "indexedfileattachment.h"
#include "application.h"

IndexedFileAttachment::IndexedFileAttachment()
{
}

bool IndexedFileAttachment::fileExists() const
{
    return !m_hash.isEmpty() && app().fileIndex().contains( filename() );
}

QString IndexedFileAttachment::filename() const
{
    if (m_hash.isEmpty())
    {
        return "";
    }
    else
    {
        return app().fileIndex().filename( m_hash );
    }
}

bool IndexedFileAttachment::setFilename(QString filename)
{
    if ( app().fileIndex().contains( filename ) )
    {
        m_hash = app().fileIndex().hash( filename );
        open();
        return true;
    }
    else
    {
        WARNING << "Cannot set file since " << filename << " is not indexed.";
        return false;
    }
}

bool IndexedFileAttachment::setHash(QByteArray hash)
{
    if ( app().fileIndex().contains( hash ) )
    {
        m_hash = hash;
        open();
        emit hashChanged( m_hash );
        return true;
    }
    else
    {
        WARNING << "Cannot set file since index does not contain " << QString::fromLatin1( hash.toHex() ) << ".";
        return false;
    }
}

void IndexedFileAttachment::copy(Attachment *&attachment) const
{
    IndexedFileAttachment* a = dynamic_cast<IndexedFileAttachment*>(attachment);
    assert( a );

    a->m_hash = m_hash;
    //TODO a->open(); ?
}

QJsonObject IndexedFileAttachment::toJsonObject() const
{
    QJsonObject object = Attachment::toJsonObject();

    object.insert("hash", QString::fromLatin1( m_hash.toHex() ));

    return object;
}

bool IndexedFileAttachment::restoreFromJsonObject(const QJsonObject &object)
{
    bool success = true;
    if (success && (success = checkJsonObject( object, "hash", QJsonValue::String )))
    {
        m_hash = QByteArray::fromHex( object["hash"].toString().toLatin1() );
    }

    return Attachment::restoreFromJsonObject( object ) && success;
}





