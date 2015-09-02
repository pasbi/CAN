#include "setlistwidget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

#include "PDFCreator/pdfcreator.h"
#include "Commands/SetlistCommands/setlistnewitemcommand.h"
#include "ui_setlistwidget.h"
#include "application.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Project/project.h"
#include "DatabaseView/SongDatabaseView/songtableviewcontainer.h"
#include "util.h"
#include "Database/EventDatabase/setlistitem.h"
#include "Database/EventDatabase/setlist.h"
#include "setlistitemselector.h"

DEFN_CONFIG( SetlistWidget, "SetlistWidget" );

SetlistWidget::SetlistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetlistWidget),
    m_selector( new SetlistItemSelector( this ))
{
    ui->setupUi(this);

    connect(ui->listView, SIGNAL(mousePress()), this, SLOT(updateButtonsEnabled()) );
    setSetlist( NULL );

    m_attentionPixmap = QPixmap::fromImage(QImage(":/icons/icons/lightbulb20.png")).scaledToWidth( 32, Qt::SmoothTransformation );
    ui->infoLabelIcon->setFixedSize( 32, 32 );

    connect( ui->comboBox, SIGNAL(currentTextChanged(QString)), ui->listView, SLOT(setFilterTag(QString)) );

}

SetlistWidget::~SetlistWidget()
{
    delete ui;
}

void SetlistWidget::updateButtonsEnabled()
{
    bool buttonsEnabled = !!ui->listView->model();

    ui->buttonAdd->setEnabled( buttonsEnabled );
    ui->buttonShowSongs->setEnabled( buttonsEnabled );
    ui->buttonExportPDF->setEnabled( buttonsEnabled );
}

void SetlistWidget::setSetlist(Setlist *setlist)
{
    Setlist* oldSetlist = ui->listView->model();
    if (oldSetlist)
    {
        disconnect( oldSetlist, SIGNAL(rowsInserted(QModelIndex,int,int)) );
        disconnect( oldSetlist, SIGNAL(rowsRemoved(QModelIndex,int,int)) );
        disconnect( oldSetlist, SIGNAL(dataChanged(QModelIndex,QModelIndex)) );
    }

    ui->listView->setModel( setlist );
    if (setlist)
    {
        connect( setlist, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateInfoLabel()) );
        connect( setlist, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateInfoLabel()) );
        connect( setlist, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateInfoLabel()) );
    }
    updateInfoLabel();
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

void SetlistWidget::on_buttonAdd_clicked()
{
    if (setlist())
    {
        app().pushCommand( new SetlistNewItemCommand( setlist(), new SetlistItem(setlist()) ) );
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

    m_selector->hide();
    QWidget::hideEvent(e);
}

void SetlistWidget::showEvent(QShowEvent *e)
{
    if (m_selectorIsVisible)
    {
        m_selector->show();
    }
    updateInfoLabel();
    updateTagComboBox();
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
        PDFCreator::exportSetlist( currentSetlist, this );
    }
}


void SetlistWidget::on_listView_doubleClicked(const QModelIndex &index)
{
    SetlistItem* item = ui->listView->model()->itemAt( index );
    if (!item)
    {
        return;
    }

#ifdef EDIT_SONG_ON_DOUBLE_CLICK
    const Song* song = item->song();
    if (song)
    {
        app().selectSong( song );
    }
#endif

}

void SetlistWidget::updateInfoLabel()
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    int songsWithValidDuration = 0;
    int allSongs = 0;
    Setlist*  sl = ui->listView->model();

    bool containsInvalid = false;
    if (sl)
    {
        for (const SetlistItem* i : sl->items())
        {
            if (i->type() == SetlistItem::SongType)
            {
                if (i->song()->duration().isValid())
                {
                    seconds += i->song()->duration().second();
                    minutes += i->song()->duration().minute();
                    hours   += i->song()->duration().hour();
                    songsWithValidDuration++;
                }
                else
                {
                    containsInvalid = true;
                }
                allSongs++;
            }
        }


        while (seconds >= 60)
        {
            seconds -= 60;
            minutes++;
        }

        while (minutes >= 60)
        {
            minutes -= 60;
            hours++;
        }

        QString h = QString("%1").arg(hours);
        QString m = QString("%1").arg(minutes);
        if (m.length() < 2) m = "0" + m;
        QString s = QString("%1").arg(seconds);
        if (s.length() < 2) s = "0" + s;

        QString toolTip = tr("Attention: %1 songs have no information about their duration.").arg( allSongs - songsWithValidDuration );
        if (containsInvalid)
        {
            ui->infoLabelIcon->setPixmap( m_attentionPixmap );
            ui->infoLabel->setToolTip( toolTip );
            ui->infoLabelIcon->setToolTip( toolTip );
        }
        else
        {
            ui->infoLabelIcon->setPixmap( QPixmap() );
            ui->infoLabelIcon->setToolTip( "" );
            ui->infoLabelIcon->setToolTip( "" );
        }
        ui->infoLabel->setText( QString(tr("%1 songs, duration: %2:%3:%4")).arg( allSongs ).arg( h, m, s ) );
    }
    else
    {
        ui->infoLabel->setText( "" );
    }
}

void SetlistWidget::updateTagComboBox()
{
    QString currentTag = ui->comboBox->currentText();
    ui->comboBox->clear();
    ui->comboBox->addItem("");
    ui->comboBox->addItems( Taggable::allTags() );
    if (Taggable::allTags().contains( currentTag ))
    {
        ui->comboBox->setCurrentText( currentTag );
    }
    else
    {
        ui->comboBox->setCurrentText( "" );
    }
}
