#include "audioattachmentview.h"
#include "ui_audioattachmentview.h"


DEFN_CREATABLE( AudioAttachmentView, AttachmentView );

AudioAttachmentView::AudioAttachmentView(QWidget* parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::AudioAttachmentView)
{
    m_audioWidget = new QWidget(this);
    ui->setupUi( m_audioWidget );
    setWidget( m_audioWidget );
}

AudioAttachmentView::~AudioAttachmentView()
{
    delete ui;
}

void AudioAttachmentView::polish()
{
    IndexedFileAttachmentView::polish();
}

void AudioAttachmentView::open()
{

}
