#include "setlistwidget.h"
#include "ui_setlistwidget.h"
#include "application.h"
#include "Database/SongDatabase/songdatabase.h"
#include "project.h"
#include "DatabaseView/SongDatabaseView/songtableviewcontainer.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include "PDFCreator/pdfcreator.h"
#include <QMessageBox>

DEFN_CONFIG( SetlistWidget, "SetlistWidget" );
CONFIGURABLE_ADD_ITEM_HIDDEN( SetlistWidget, DefaultPDFSavePath, QDir::homePath() );

SetlistWidget::SetlistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetlistWidget),
    m_selector( new SetlistItemSelector( this ))
{
    ui->setupUi(this);

    setSetlist( NULL );

    connect(ui->listView, SIGNAL(clicked()), this, SLOT(updateButtonsEnabled()) );
}

SetlistWidget::~SetlistWidget()
{
    delete ui;
}

void SetlistWidget::updateButtonsEnabled()
{
    bool buttonsEnabled = !!ui->listView->model();
    bool itemButtonsEnabled = !ui->listView->selectedItems().isEmpty();
    bool upperLimit = false;
    bool lowerLimit = false;

    if (!ui->listView->selectionModel())
    {
        upperLimit = true;
        lowerLimit = true;
    }
    else
    {
        for (const QModelIndex & index : ui->listView->selectionModel()->selectedRows())
        {
            if (index.row() == 0)
            {
                lowerLimit = true;
            }
            if (index.row() == setlist()->rowCount() - 1)
            {
                upperLimit = true;
            }
        }
    }
    ui->buttonAdd->setEnabled( buttonsEnabled );
    ui->buttonShowSongs->setEnabled( buttonsEnabled );
    ui->buttonDelete->setEnabled( itemButtonsEnabled );
    ui->buttonSortDown->setEnabled( !upperLimit );
    ui->buttonSortUp->setEnabled( !lowerLimit );
    ui->buttonExportPDF->setEnabled( buttonsEnabled );
}

void SetlistWidget::setSetlist(Setlist *setlist)
{
    ui->listView->setModel( setlist );

    updateButtonsEnabled();

}


bool ascending(const QModelIndex& a, const QModelIndex& b)
{
    return a < b;
}

bool descending(const QModelIndex& a, const QModelIndex& b)
{
    return !ascending(a, b);
}

#include "Commands/SetlistCommands/setlistmoverowscommand.h"
void SetlistWidget::on_buttonSortUp_clicked()
{
    if (setlist() && ui->listView->selectionModel())
    {
        QModelIndexList indices = ui->listView->selectionModel()->selectedRows();
        if (!indices.isEmpty())
        {
            qSort( indices.begin(), indices.end(), ascending);
            for (const QModelIndex& index : indices)
            {
                app().pushCommand( new SetlistMoveRowsCommand( setlist(), index, -1 ) );
            }
        }
    }
    updateButtonsEnabled();
}

void SetlistWidget::on_buttonSortDown_clicked()
{
    if (setlist() && ui->listView->selectionModel())
    {
        QModelIndexList indices = ui->listView->selectionModel()->selectedRows();
        if (!indices.isEmpty())
        {
            qSort( indices.begin(), indices.end(), descending);
            for (const QModelIndex& index : indices)
            {
                app().pushCommand( new SetlistMoveRowsCommand( setlist(), index, 1 ) );
            }
        }
    }
    updateButtonsEnabled();
}

#include "Commands/SetlistCommands/setlistremoveitemcommand.h"
void SetlistWidget::on_buttonDelete_clicked()
{
    QList<SetlistItem*> selectedSetlistItems = ui->listView->selectedItems();
    if (setlist() && !selectedSetlistItems.isEmpty())
    {
        app().project()->beginMacro( tr("Remove Setlist Items") );
        for (SetlistItem* si : selectedSetlistItems)
        {
            app().pushCommand( new SetlistRemoveItemCommand( setlist(), si ) );
        }
        app().project()->endMacro();
    }
    else
    {
        qWarning() << "Cannot add Item to no set list.";
    }
}

#include "Commands/SetlistCommands/setlistadditemcommand.h"
void SetlistWidget::on_buttonAdd_clicked()
{
    if (setlist())
    {
        app().pushCommand( new SetlistAddItemCommand( setlist(), new SetlistItem() ) );
    }
    else
    {
        qWarning() << "Cannot add Item to no set list.";
    }
}

SetlistView* SetlistWidget::listView() const
{
    return ui->listView;
}

void SetlistWidget::on_buttonShowSongs_clicked()
{
    m_selector->show();
}

void SetlistWidget::hideEvent(QHideEvent *e)
{
    m_selectorIsVisible = m_selector->isVisible();
    m_selectorGeometry = m_selector->geometry();
    m_selectorPosition = m_selector->pos();

    m_selector->hide();
    QWidget::hideEvent(e);
}

void SetlistWidget::showEvent(QShowEvent *e)
{
    if (m_selectorIsVisible)
    {
        m_selector->show();
        m_selector->setGeometry( m_selectorGeometry );
        m_selector->move( m_selectorPosition );
    }
    QWidget::showEvent(e);
}

Setlist* SetlistWidget::setlist() const
{
    return ui->listView->model();
}


void SetlistWidget::on_buttonExportPDF_clicked()
{
    Setlist* currentSetlist = setlist();
    if (currentSetlist)
    {
        QString filename = currentSetlist->event()->label()
                + "_"
                + QLocale().toString( currentSetlist->event()->beginning().date(), QLocale().dateFormat( QLocale::ShortFormat )).replace(".", "_");

        // QFileDialog::getSaveFilename does not asks for overwriting files when the default filename is used. Workaround: Disable overwrite
        // confirmation for all files and ask for it in an other dialog.
        bool allowWriteFile;
        do
        {
            filename = QFileDialog::getSaveFileName(    this,
                                                        tr("Export PDF ..."),
                                                        QDir::home().absoluteFilePath( filename ),
                                                        "*.pdf",
                                                        NULL,
                                                        QFileDialog::DontConfirmOverwrite );

            if (filename.isEmpty())
            {
                break;  // putting this break in while( ... ), means complicating the whole thing since it will be
                        // expanded to *.pdf in the following and thus would not be empty
            }
            if (!filename.endsWith(".pdf"))
            {
                filename.append(".pdf");
            }

            allowWriteFile = !QFileInfo(filename).exists();
            if ( !filename.isEmpty() && !allowWriteFile )
            {
                if (QMessageBox::question( this, windowTitle(), QString(tr("%1 already exists.\n"
                                                                           "Do you want to replace it?")).arg(filename), QMessageBox::No, QMessageBox::Yes )
                        == QMessageBox::Yes )
                {
                    allowWriteFile = true;
                }
            }

        } while ( !allowWriteFile      // filename is not in use by now or user allows to overwrite.
                );


        if (!filename.isEmpty())
        {
            QFileInfo fi(filename);

            if (fi.exists() && !fi.isReadable())
            {
                QMessageBox::warning( this,
                                      tr("Cannot write"),
                                      QString(tr("File %1 is not writable.").arg(filename)),
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton );
            }
            else
            {
                PDFCreator pc(currentSetlist);
                pc.save( filename );
                config.set( "DefaultPDFSavePath", filename );

            }
        }
    }
}

