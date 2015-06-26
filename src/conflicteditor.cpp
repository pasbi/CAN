#include "conflicteditor.h"
#include "ui_conflicteditor.h"
#include "util.h"
#include <QScrollBar>

ConflictEditor::ConflictEditor(const QList<ConflictFile*>& conflictingFiles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConflictEditor)
{
    ui->setupUi(this);

    m_files = conflictingFiles;

    for (ConflictFile* file : m_files)
    {
        for (Conflict* conflict : file->conflicts())
        {
            m_items << new Item( ui->listWidget, conflict );
            ui->listWidget->addItem( m_items.last() );
        }
    }

    ui->plainTextEditRemote->setTextInteractionFlags( Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse );
    connect( ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectConflict()) );
    connect( ui->buttonOk,     SIGNAL(clicked()), this, SLOT(accept()));
    connect( ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));

}

ConflictEditor::~ConflictEditor()
{
    delete ui;
    qDeleteAll( m_items );
}

 Conflict *ConflictEditor::currentConflict() const
{
    return ((Item*) ui->listWidget->currentItem())->conflict();
}

void ConflictEditor::selectConflict()
{
    const Conflict* conflict = currentConflict();

    ui->plainTextEditLocal->setPlainText( conflict->m_local );
    ui->plainTextEditRemote->setPlainText( conflict->m_remote );

    ui->plainTextEditLocal->setStyleSheet( "" );
    ui->plainTextEditRemote->setStyleSheet( "" );

    QString seemsToBeDisabledPalette = "* { background-color: #EEEEEE; color: #999999 }";

    switch (conflict->m_resolvePolicy)
    {
    case Conflict::KeepCustom:
        break;
    case Conflict::KeepLocal:
        ui->plainTextEditRemote->setStyleSheet( seemsToBeDisabledPalette );
        break;
    case Conflict::KeepRemote:
        ui->plainTextEditLocal->setStyleSheet( seemsToBeDisabledPalette );
        break;
    case Conflict::Undefined:
        break;
    }
}

void ConflictEditor::on_buttonKeepRemote_clicked()
{
    Conflict* conflict = currentConflict();
    conflict->m_custom = ui->plainTextEditRemote->toPlainText();
    conflict->m_resolvePolicy = conflict->m_custom == conflict->m_remote ? Conflict::KeepRemote : Conflict::KeepCustom;

    ui->listWidget->currentItem()->setIcon(QIcon(":/icons/icons/check40.png"));
    selectConflict();
}

void ConflictEditor::on_buttonKeepLocal_clicked()
{
    Conflict* conflict = currentConflict();
    conflict->m_custom = ui->plainTextEditLocal->toPlainText();
    conflict->m_resolvePolicy = conflict->m_custom == conflict->m_local ? Conflict::KeepLocal : Conflict::KeepCustom;

    ui->listWidget->currentItem()->setIcon(QIcon(":/icons/icons/check40.png"));
    selectConflict();
}

void ConflictEditor::accept()
{
    resolveConflicts();
    QDialog::accept();
}

void ConflictEditor::resolveConflicts()
{
    qDebug() << "resolve " << m_files.length() << " files";
    for (ConflictFile* file : m_files)
    {
        file->resolveConflicts();
    }
}

void ConflictEditor::resolveAllMine()
{
    for (ConflictFile* file : m_files)
    {
        for (Conflict* c : file->conflicts())
        {
            qDebug() << "conflict " << c << " --> local";
            c->m_resolvePolicy = Conflict::KeepLocal;
        }
    }
    resolveConflicts();
}

void ConflictEditor::resolveAllTheirs()
{
    for (ConflictFile* file : m_files)
    {
        for (Conflict* c : file->conflicts())
        {
            qDebug() << "conflict " << c << " --> remote";
            c->m_resolvePolicy = Conflict::KeepRemote;
        }
    }
    resolveConflicts();
}
