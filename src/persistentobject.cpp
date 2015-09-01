#include "persistentobject.h"
#include <QFile>
#include <QJsonDocument>
#include "global.h"
#include <QDateTime>

const Qt::DateFormat PersistentObject::DATE_TIME_FORMAT = Qt::ISODate;


PersistentObject::PersistentObject()
{
    m_randomID = createRandomID();
}

PersistentObject::~PersistentObject()
{
}

bool PersistentObject::loadFrom(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        WARNING << "Cannot open file " << path << " for reading.";
        return false;
    }

    QByteArray fileContent = file.readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(fileContent, &error);
    if (error.error != QJsonParseError::NoError)
    {
        WARNING << "Cannot parse " << path << ": " << error.errorString();
        return false;
    }

    if (!doc.isObject())
    {
        WARNING << "Expected JsonObject\n" << QString(doc.toJson());
        return false;
    }

    if (!restoreFromJsonObject(doc.object()))
    {
        WARNING << "Cannot restore object\n" << QString(doc.toJson());
        return false;
    }
    return true;
}

void PersistentObject::seedRandomID()
{
    qsrand( QDateTime::currentMSecsSinceEpoch() );
}

QString PersistentObject::createRandomID()
{
    QString s;

    for (int i = 0; i < 32; ++i)
    {
        char c = qrand() % 36;
        if (c < 10)
        {
            s += QChar( char(c + '0') );
        }
        else
        {
            c -= 10;
            s += QChar( char( c + 'a' ) );
        }
    }
    return s;
}

bool PersistentObject::restoreFromJsonObject( const QJsonObject & object )
{
    Q_UNUSED(object);
    return true;
}

QJsonObject PersistentObject::toJsonObject() const
{
    return QJsonObject();
}
