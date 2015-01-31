#include "attachmenteditor.h"
#include <QHBoxLayout>

AttachmentEditor::AttachmentEditor(QWidget *parent) :
    QWidget(parent)
{
    m_scrollArea = new QScrollArea(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( m_scrollArea );
    setAttachment( NULL );
}

AttachmentView* createAttachmentView(Attachment* attachment)
{
    QString classname = attachment->classname();
    classname = classname.append("View");

    assert( Creatable::category(classname) == "AttachmentView" );

    AttachmentView* attachmentView = 0;
    CREATE( classname, attachmentView );
    attachmentView->setAttachment( attachment );

    return attachmentView;

}

void AttachmentEditor::setAttachment(Attachment *attachment)
{
    if (attachment)
    {
        AttachmentView* aview = createAttachmentView( attachment );
        aview->setParent( m_scrollArea );
        m_scrollArea->setWidget( aview );
        aview->show();
    }
    else
    {
        m_scrollArea->setWidget( new QWidget( m_scrollArea ) );
    }
}
