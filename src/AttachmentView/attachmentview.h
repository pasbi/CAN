#ifndef ATTACHMENTVIEW_H
#define ATTACHMENTVIEW_H

#include <QWidget>
#include "Attachments/attachment.h"
#include "configurable.h"

class AttachmentView : public QWidget, public Creatable
{
    Q_OBJECT
public:
    explicit AttachmentView( QWidget *parent = 0);
    virtual void setAttachment(Attachment* attachment);

    template< typename T = Attachment >
    T* attachment() const
    {
        return qobject_assert_cast<T*>( m_attachment );
    }

    /**
     * @brief updateAttachmentView is called when something in changes and
     *   view should react on it. Not to be confused with the event when a
     *   new attachment shall be displayed.
     */
    virtual void updateAttachmentView() {}

    virtual void restoreOptions(const QByteArray &options) { Q_UNUSED(options); }
    virtual QByteArray options() const { return QByteArray(); }


protected:
    virtual void polish() = 0;

private:
    Attachment* m_attachment;

};

#endif // ATTACHMENTVIEW_H
