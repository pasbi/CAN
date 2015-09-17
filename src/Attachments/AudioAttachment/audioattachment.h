#ifndef AUDIOATTACHMENT_H
#define AUDIOATTACHMENT_H

#include "Attachments/indexedfileattachment.h"
#include "player.h"

class SectionsModel;
class Section;
class AudioAttachment : public IndexedFileAttachment
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachment )
public:
    AudioAttachment();
    QStringList acceptedEndings() const { return QStringList() << "*"; }
    void copy(Attachment *&copied) const;

    SectionsModel* sectionsModel() const { return m_sectionsModel; }
    Player& player() { return m_player; }
    const Player& player() const { return m_player; }

    void appendSection( const Section& section );
    void setSection( const Section* section );

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);


public slots:
    void open();

private:
    SectionsModel * const m_sectionsModel;
    Player m_player;
    QString m_filename;

    const Section* m_currentSection = NULL;

signals:
    void currentSectionChanged( const Section* );

};

#endif // AUDIOATTACHMENT_H
