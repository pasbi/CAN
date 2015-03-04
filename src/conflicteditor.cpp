#include "conflicteditor.h"
#include "ui_conflicteditor.h"
#include "util.h"

ConflictEditor::ConflictEditor(const QString & directory, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConflictEditor)
{
    ui->setupUi(this);
    m_directory = directory;

    QStringList allFiles = Util::findAllFiles( directory );
    for ( const QString & filename : allFiles )
    {
        File file(filename);
        if (!file.conflicts().isEmpty())
        {
            m_files.append( file );
        }
    }

    for (const File & file : m_files)
    {
        for (const Conflict & conflict : file.conflicts())
        {
            m_items << Item( ui->listWidget, conflict );
            ui->listWidget->addItem( &m_items.last() );
        }
    }
}

ConflictEditor::~ConflictEditor()
{
    delete ui;
}
