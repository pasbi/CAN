#ifndef TOJSON_H
#define TOJSON_H

#include <QJsonDocument>

class Project;
class ToJson
{
public:
    ToJson(Project *project);
    QString toString() const;

private:
    QJsonDocument m_jsonDocument;
};

#endif // TOJSON_H
