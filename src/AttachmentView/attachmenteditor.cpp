#include "attachmenteditor.h"

#include <QHBoxLayout>
#include <QScrollArea>

#include "Attachments/attachment.h"
#include "attachmentview.h"

AttachmentEditor::AttachmentEditor(QWidget *parent) :
    QWidget(parent)
{
    m_scrollArea = new QScrollArea(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );
    layout->addWidget( m_scrollArea );

    QHBoxLayout* scrollAreaLayout = new QHBoxLayout(m_scrollArea);
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );
    m_scrollArea->setLayout( scrollAreaLayout );
    setAttachment( nullptr );


}

AttachmentView* createAttachmentView(Attachment* attachment)
{
    QString classname = attachment->classname();
    classname = classname.append("View");

    assert( Creatable::category(classname) == "AttachmentView" );

    AttachmentView* attachmentView = Creatable::create<AttachmentView>( classname );
    attachmentView->setAttachment( attachment );

    return attachmentView;

}

void AttachmentEditor::setAttachment(Attachment *attachment)
{
    if (m_currentView)
    {
        m_currentView->hide();
    }

    if (attachment)
    {
        if (!m_attachmentViews.contains(attachment))
        {
            m_currentView = createAttachmentView(attachment);
            m_currentView->setParent(m_scrollArea);
            m_scrollArea->layout()->addWidget(m_currentView);
            connect( m_currentView, SIGNAL(focusAttachment(const Attachment*)), this, SIGNAL( focusAttachment( const Attachment* )));
            m_attachmentViews.insert(attachment, m_currentView);
        }
        else
        {
            m_currentView = m_attachmentViews[attachment];
        }
        m_currentView->show();
    }
    else
    {
        m_currentView = nullptr;
    }
}

void AttachmentEditor::updateAttachmentView()
{
    if (m_currentView)
    {
        m_currentView->updateAttachmentView();
    }
}

void AttachmentEditor::deactivateAttachments()
{
    for (AttachmentView* av : m_attachmentViews.values())
    {
        av->deactivate();
    }
}
