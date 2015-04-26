#ifndef AUDIOATTACHMENTVIEW_H
#define AUDIOATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"
#include "Attachments/AudioAttachment/audioattachment.h"


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
    void on_doubleSpinBoxTempo_editingFinished();
    void on_doubleSpinBoxPitch_editingFinished();
    void on_pushButtonStop_clicked();
    void on_pushButtonPlayPause_toggled(bool checked);
    void on_doubleSpinBoxElapsed_editingFinished();
    void on_doubleSpinBoxRemaining_editingFinished();
    void on_slider_sliderMoved(int position);
    void updateSliders();
    void updateSliderRange();

    AudioPlayer& player() { return attachment<AudioAttachment>()->player(); }


private:
    Ui::AudioAttachmentView *ui;
    QWidget* m_audioWidget = NULL;

    static const double SLIDER_MULTIPLIER;



};

#endif // AUDIOATTACHMENTVIEW_H
