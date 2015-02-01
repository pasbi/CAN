#include "attachmentchooser.h"
#include "ui_attachmentchooser.h"

AttachmentChooser::AttachmentChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttachmentChooser)
{
    ui->setupUi(this);
}

AttachmentChooser::~AttachmentChooser()
{
    delete ui;
}
