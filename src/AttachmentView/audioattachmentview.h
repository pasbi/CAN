#ifndef AUDIOATTACHMENTVIEW_H
#define AUDIOATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"

class AudioAttachmentView : public IndexedFileAttachmentView
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachmentView );
public:
    AudioAttachmentView(QWidget* parent = 0);


    void restoreOptions(const QByteArray &options) { Q_UNUSED(options); }
    QByteArray options() const { return QByteArray(); }

protected:
    void polish();

protected slots:
    void open();
};

#endif // AUDIOATTACHMENTVIEW_H
