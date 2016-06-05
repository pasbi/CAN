#ifndef SECTION_H
#define SECTION_H

#include <QJsonObject>
#include "global.h"
#include <QTime>
#include "ratio.h"

class Section
{
public:
    Section(const QString & caption, const QTime &begin, const QTime &end );
    Section(const QString & caption, double begin, double end);
    Section();

    QString caption() const { return m_caption; }
    QTime begin() const { return m_begin; }
    QTime end() const { return m_end; }

    double beginDouble() const;
    double endDouble() const;

    void setCaption( const QString & caption ) { m_caption = caption; }
    void setBegin(const QTime& begin);
    void setEnd(const QTime& end);

    bool isValid() const;

    bool operator==(const Section& other) const;
    bool operator!=(const Section& other) const;

    Ratio similarity(const Section* other) const;



private:
    QString m_caption;
    QTime m_begin;
    QTime m_end;

    friend QDataStream& operator<<(QDataStream& out, const Section& model);
    friend QDataStream& operator>>(QDataStream& in, Section& model);

};

Q_DECLARE_METATYPE(Section)

QDataStream& operator<<(QDataStream& out, const Section& model);
QDataStream& operator>>(QDataStream& in, Section& model);


#endif // SECTION_H
