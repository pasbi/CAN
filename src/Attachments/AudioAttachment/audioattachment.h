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

    void deactivate() { player().pause(); }

    QStringList skipSerializeAttributes() const;

    QVariant attribute(const QString &key) const;
    void setAttribute(const QString &key, const QVariant &value);
    Ratio similarity(const DatabaseItemBase *other) const;


public slots:
    void open();

private:
    SectionsModel * const m_sectionsModel;
    Player m_player;
    QString m_filename;
    static Ratio compareSectionsModel(const SectionsModel* a, const SectionsModel* b);



protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

};



#endif // AUDIOATTACHMENT_H
