#include "tagdialog.h"
#include "ui_tagdialog.h"
#include <QMouseEvent>

TagDialog::TagDialog(const QStringList & tags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagDialog)
{
    ui->setupUi( this );
    ui->widget->setTags( tags );
    m_tags = tags;
}

TagDialog::~TagDialog()
{
    delete ui;
}

QStringList TagDialog::tags() const
{
    return m_tags;
}

// static convienece function
bool TagDialog::setTags(QWidget *parent, Taggable *taggable)
{
    TagDialog dialog( taggable->tags(), parent );
    if (dialog.exec() == QDialog::Accepted)
    {
        taggable->setTags( dialog.tags() );
        return true;
    }
    else
    {
        return false;
    }
}

void TagDialog::accept()
{
    m_tags = ui->widget->tags();
    QDialog::accept();
}


