#ifndef AUDIOATTACHMENT_H
#define AUDIOATTACHMENT_H

#include "Attachments/indexedfileattachment.h"
#include "player.h"
#include "section.h"

class SectionsModel;
class AudioAttachment : public IndexedFileAttachment
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachment )
public:
    AudioAttachment();
    QStringList acceptedEndings() const { return QStringList({"mp3", "ogg", "aif", "aiff"}); }
    void copy(Attachment *&copied) const;

    SectionsModel* sectionsModel() const { return m_sectionsModel; }
    Player& player() { return m_player; }
    const Player& player() const { return m_player; }

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);


public slots:
    void open();

private:
    SectionsModel * const m_sectionsModel;
    Player m_player;
    QString m_filename;
};

#endif // AUDIOATTACHMENT_H
