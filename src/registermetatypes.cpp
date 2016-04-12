#include "registermetatypes.h"

#include "Attachments/AudioAttachment/section.h"
#include "Database/EventDatabase/setlistitem.h"
#include "Database/EventDatabase/event.h"
#include "Database/SongDatabase/song.h"
#include <QList>
#include "Merge/mergeitem.h"
#include "chord.h"

Q_DECLARE_METATYPE(QList<int>)

typedef QMap<QString, QString> QStringMap;
Q_DECLARE_METATYPE(QStringMap)

RegisterMetaTypes::RegisterMetaTypes()
{
    qRegisterMetaTypeStreamOperators<Section>();
    qRegisterMetaTypeStreamOperators<SetlistItem::Type>();
    qRegisterMetaTypeStreamOperators<Event::Type>();
    qRegisterMetaTypeStreamOperators<Song::Label>();
    qRegisterMetaTypeStreamOperators<Song::State>();
    qRegisterMetaTypeStreamOperators<QList<int>>();
    qRegisterMetaTypeStreamOperators<MergeItem::Action>();
    qRegisterMetaTypeStreamOperators<MergeItem::Decision>();
    qRegisterMetaTypeStreamOperators<Chord>();
    qRegisterMetaType<Chord>();

    qRegisterMetaTypeStreamOperators<QStringMap>();

}
