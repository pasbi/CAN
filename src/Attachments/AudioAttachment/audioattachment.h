#ifndef AUDIOATTACHMENT_H
#define AUDIOATTACHMENT_H

#include "Attachments/indexedfileattachment.h"
#include "sectionsmodel.h"
#include "player.h"

class AudioAttachment : public IndexedFileAttachment
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachment )
public:
    AudioAttachment();
    QStringList acceptedEndings() const { return QStringList() << "mp3" << "ogg"; }
    void copy(Attachment *&copied) const;

    SectionsModel* sectionsModel() const { return m_sectionsModel; }
    Player& player() { return m_player; }
    const Player& player() const { return m_player; }


public slots:
    void open();

private:
    SectionsModel * const m_sectionsModel;
    Player m_player;

};

#endif // AUDIOATTACHMENT_H
