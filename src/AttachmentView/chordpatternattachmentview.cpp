#include "chordpatternattachmentview.h"
#include "ui_chordpatternattachmentview.h"

DEFN_CREATABLE(ChordPatternAttachmentView, AttachmentView);

ChordPatternAttachmentView::ChordPatternAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::ChordPatternAttachmentView)
{
    ui->setupUi(this);
}

ChordPatternAttachmentView::~ChordPatternAttachmentView()
{
    delete ui;
}
