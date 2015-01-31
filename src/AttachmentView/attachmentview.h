#ifndef ATTACHMENTVIEW_H
#define ATTACHMENTVIEW_H

#include <QWidget>
#include "Attachments/attachment.h"

class AttachmentView : public QWidget
{
    Q_OBJECT
public:
    explicit AttachmentView(QWidget *parent = 0);
    void setAttachment(Attachment* attachment);

    template<typename T>
    T* attachment() const;

protected:
    virtual void connectWithAttachment() = 0;

private:
    Attachment* m_attachment = 0;

};

#endif // ATTACHMENTVIEW_H
