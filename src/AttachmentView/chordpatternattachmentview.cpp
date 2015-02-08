#include "chordpatternattachmentview.h"
#include "ui_chordpatternattachmentview.h"
#include <QToolBar>

DEFN_CREATABLE(ChordPatternAttachmentView, AttachmentView);

ChordPatternAttachmentView::ChordPatternAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::ChordPatternAttachmentView)
{
    ui->setupUi(this);
    new QToolBar("Hello", this);
}

ChordPatternAttachmentView::~ChordPatternAttachmentView()
{
    delete ui;
}
