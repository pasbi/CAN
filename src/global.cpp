#include "global.h"

std::stringstream& operator << ( std::stringstream& stream, const QString & qstring )
{
    stream << qstring.toStdString();
    return stream;
}

std::stringstream& operator << ( std::stringstream& stream, const char* chrs )
{
    stream << QString(chrs);
    return stream;
}

std::stringstream& operator<<(std::stringstream & stream, const void* ptr )
{
    int pointerWidth = sizeof(void*) * 2;
    quint64 n = reinterpret_cast<quint64>(ptr);

    QString string = QString("%1").arg(n, 0, 16);
    while (string.length() < pointerWidth)
    {
        string.prepend("0");
    }
    string.prepend("0x");

    stream << string;
    return stream;
}

