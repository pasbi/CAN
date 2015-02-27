#ifndef AUDIOATTACHMENTVIEW_H
#define AUDIOATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"

namespace Ui {
class AudioAttachmentView;
}

class AudioAttachmentView : public IndexedFileAttachmentView
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachmentView );
public:
    AudioAttachmentView(QWidget* parent = 0);
    ~AudioAttachmentView();


    void restoreOptions(const QByteArray &options) { Q_UNUSED(options); }
    QByteArray options() const { return QByteArray(); }

protected:
    void polish();

protected slots:
    void open();

private:
    Ui::AudioAttachmentView *ui;
    QWidget* m_audioWidget = NULL;
};

#endif // AUDIOATTACHMENTVIEW_H
