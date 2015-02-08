#include "chordpatternattachmentview.h"
#include "ui_chordpatternattachmentview.h"
#include "global.h"

DEFN_CREATABLE(ChordPatternAttachmentView, AttachmentView);

ChordPatternAttachmentView::ChordPatternAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::ChordPatternAttachmentView)
{
    ui->setupUi(this);
    m_toolBar = new QToolBar( this );

    addAction( new QAction( QIcon(":/icons/icons/up51.png"), "Transpose Up", this));
    addAction( new QAction( QIcon(":/icons/icons/down27.png"), "Transpose Down", this));


    m_toolBar->addActions( actions() );
    ui->verticalLayout->insertWidget(0, m_toolBar);
}

ChordPatternAttachmentView::~ChordPatternAttachmentView()
{
    delete ui;
}
