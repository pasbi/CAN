#ifndef AUDIOATTACHMENTVIEW_H
#define AUDIOATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"
#include "Attachments/AudioAttachment/audioattachment.h"
#include <QMenu>


namespace Ui {
class AudioAttachmentView;
}

class AudioAttachmentView : public IndexedFileAttachmentView
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachmentView )
    DECL_CONFIG( AudioAttachmentView );

public:
    AudioAttachmentView(QWidget* parent = 0);
    ~AudioAttachmentView();


    void restoreOptions(const QByteArray &options) { Q_UNUSED(options); }
    QByteArray options() const { return QByteArray(); }

protected:
    void polish();

protected slots:
    void open();

private slots:
    void setPitchTempo();
    void on_pushButtonStop_clicked();
    void on_pushButtonPlayPause_toggled(bool checked);
    void seek( double pos );

    void recordSection( QAction* action );

    Player& player() { return attachment<AudioAttachment>()->player(); }


private:
    Ui::AudioAttachmentView *ui;
    QWidget* m_audioWidget = NULL;

    QAction* m_setPositionAction = NULL;
    QAction* m_abortRecordingAction = NULL;

    static const QString RECORD_LEFT_POSITION_ICON_PATH;
    static const QString RECORD_RIGHT_POSITION_ICON_PATH;
    static const QString RECORD_LEFT_POSITION_CAPTION;
    static const QString RECORD_RIGHT_POSITION_CAPTION;



};

#endif // AUDIOATTACHMENTVIEW_H
