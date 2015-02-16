#include "attachmenteditor.h"
#include <QHBoxLayout>

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

    qDebug() << classname << Creatable::category(classname);
    assert( Creatable::category(classname) == "AttachmentView" );

    AttachmentView* attachmentView = 0;
    CREATE( classname, attachmentView );
    assert( attachmentView );
    attachmentView->setAttachment( attachment );

    return attachmentView;

}

void AttachmentEditor::setAttachment(Attachment *attachment)
{
    static AttachmentView* currentView = NULL;

    delete currentView;
    delete m_scrollArea->layout();
    QVBoxLayout* layout = new QVBoxLayout(m_scrollArea);

    if (attachment)
    {
        currentView = createAttachmentView( attachment );
        currentView->setParent( m_scrollArea );
        layout->addWidget( currentView );
        delete m_scrollArea->widget();
    }
    else
    {
        currentView = NULL;
    }
    m_scrollArea->setLayout( layout );
}
