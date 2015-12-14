#ifndef AUDIOATTACHMENTVIEW_H
#define AUDIOATTACHMENTVIEW_H

#include "AttachmentView/IndexedFileAttachmentView/indexedfileattachmentview.h"
#include "Attachments/AudioAttachment/audioattachment.h"
#include "configurable.h"


namespace Ui {
class AudioAttachmentView;
}

class AudioAttachmentView : public IndexedFileAttachmentView
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachmentView )
    DECL_CONFIG( AudioAttachmentView )

public:
    AudioAttachmentView(QWidget* parent = 0);
    ~AudioAttachmentView();

protected slots:
    void polish();
    void chooseFile();


private slots:
    void abortSection() { recordSection(true);}
    void recordSection( bool abort = false );

    Player& player() { return attachment<AudioAttachment>()->player(); }

    void restoreCurrentSection();
    void deleteCurrentSection();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::AudioAttachmentView *ui;

    static const QString RECORD_LEFT_POSITION_ICON_PATH;
    static const QString RECORD_RIGHT_POSITION_ICON_PATH;



};

#endif // AUDIOATTACHMENTVIEW_H
