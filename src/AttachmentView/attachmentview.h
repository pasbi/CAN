#ifndef ATTACHMENTVIEW_H
#define ATTACHMENTVIEW_H

#include <QWidget>
#include "creatable.h"
#include "global.h"

class Attachment;
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

    /**
     * @brief deactivate deactivates this attachment view, e.g. pauses audio attachment.
     */
    virtual void deactivate() {}

protected:
    virtual void polish() = 0;

private:
    Attachment* m_attachment;

signals:
    void focusAttachment( const Attachment* );

private slots:
    void listenToAttachmentDestruction();

};

#endif // ATTACHMENTVIEW_H
