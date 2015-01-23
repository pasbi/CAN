#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include <sstream>
#include <QString>

#define WARNING \
    GENERIC_LOG(Warning, false)

#define FATAL \
    GENERIC_LOG(FATAL, true)

#define GENERIC_LOG(TYPE, IS_FATAL) \
    WarningLogger(#TYPE, __FILE__, __LINE__, IS_FATAL).stream()

class WarningLogger : public std::stringstream
{
public:
    WarningLogger(const QString & type, const QString & file, const int line, bool isFatal) :
        m_type(type),
        m_file(file),
        m_line(line),
        m_fatal(isFatal)
    {

    }

    std::stringstream& stream()
    {
        return *this;
    }

    ~WarningLogger()
    {
        // do sth with the message.
        qWarning() << QString("[%1]: %2, at %3:%4").arg(m_type).arg(QString(str().c_str())).arg(m_file).arg(m_line);
        if (m_fatal)
        {
            exit(1);
        }
    }

private:
    const QString m_type;
    const QString m_file;
    const int m_line;
    const bool m_fatal;
};

std::stringstream& operator<<(std::stringstream & stream, const QString & qstring);
std::stringstream& operator<<(std::stringstream & stream, const char* chrs );
std::stringstream& operator<<(std::stringstream & stream, const void* ptr );

#endif // GLOBAL_H
