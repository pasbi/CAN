#include "tojson.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Project/project.h"
#include "Attachments/attachment.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/SongDatabase/song.h"

namespace {
QJsonObject song2Json(const Song* song) {
    QJsonObject json;
    QString name = song->attribute("title").toString();
    name += " - ";
    name += song->attribute("artist").toString();

    QString pattern = QString();
    for (const Attachment* a : song->attachments())
    {
        if (a->type() == ChordPatternAttachment::TYPE)
        {
            if (!pattern.isNull()) {
                qWarning() << "Drop chord pattern: " << name;
            }
            pattern = static_cast<const ChordPatternAttachment*>(a)->attribute("chordPattern").toString();
        }
    }

    json.insert("name", name);
    json.insert("pattern", pattern);
    json.insert("scrollRate", 0.0);
    json.insert("patternTextSize", 18);
    return json;
}
}

ToJson::ToJson(Project* project)
{
    QJsonArray array;
    for (const Song* song : project->songDatabase()->items())
    {
        array.append(song2Json(song));
    }
    QJsonObject jsonObject;
    jsonObject.insert("items", array);
    m_jsonDocument = QJsonDocument();
    m_jsonDocument.setObject(jsonObject);
}

QString ToJson::toString() const
{
    return m_jsonDocument.toJson();
}





