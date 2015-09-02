#ifndef SECTION_H
#define SECTION_H

#include <QJsonObject>
#include "global.h"

class Section
{
public:
    Section( const QString & caption, const double begin, const double end );
    Section();

    QString caption() const { return m_caption; }
    double begin() const { return m_begin; }
    double end() const { return m_end; }

    void setCaption( const QString & caption ) { m_caption = caption; }

    bool valid() const { return m_begin < m_end && m_begin >= 0 && m_end >= 0 && !m_caption.isNull(); }

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject& object );



private:
    QString m_caption;
    double m_begin;
    double m_end;
};

#endif // SECTION_H
