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
    setAttachment( NULL );
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
        m_attachmentViewOptions[m_currentView->attachment()] = m_currentView->options();
    }

    delete m_currentView;
    delete m_scrollArea->layout();
    QVBoxLayout* layout = new QVBoxLayout(m_scrollArea);


    if (attachment)
    {
        m_currentView = createAttachmentView( attachment );
        if (m_attachmentViewOptions.contains( attachment ))
        {
            m_currentView->restoreOptions(m_attachmentViewOptions[attachment]);
        }

        m_currentView->setParent( m_scrollArea );
        layout->addWidget( m_currentView );

        connect( m_currentView, SIGNAL(focusAttachment(const Attachment*)), this, SIGNAL( focusAttachment( const Attachment* )));

        delete m_scrollArea->widget();
    }
    else
    {
        m_currentView = NULL;
    }

    m_scrollArea->setLayout( layout );

}

void AttachmentEditor::updateAttachmentView()
{
    if (m_currentView)
    {
        m_currentView->updateAttachmentView();
    }
}
