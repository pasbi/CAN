#ifndef ATTACHMENTVIEW_H
#define ATTACHMENTVIEW_H

#include <QWidget>
#include "Attachments/attachment.h"

class AttachmentView : public QWidget, public Creatable
{
    Q_OBJECT
public:
    explicit AttachmentView(QWidget *parent = 0);
    void setAttachment(Attachment* attachment);

    template<typename T>
    T* attachment() const
    {
        T* attachment = dynamic_cast<T*>( m_attachment );
        if ( !attachment )
        {
            assert( false );
        }
        return attachment;
    }

    Attachment* attachment() const { return m_attachment; }

    /**
     * @brief updateAttachmentView is called when something in changes and
     *   view should react on it. Not to be confused with the event when a
     *   new attachment shall be displayed.
     */
    virtual void updateAttachmentView() {}


protected:
    virtual void polish() = 0;

private:
    Attachment* m_attachment = 0;

};

#endif // ATTACHMENTVIEW_H
