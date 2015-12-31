#include "audioattachmentview.h"
#include "ui_audioattachmentview.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"
#include "QSignalBlocker"
#include "DatabaseView/ItemDelegates/durationdelegate.h"
#include "DatabaseView/ItemDelegates/lineeditdelegate.h"
#include "Commands/AttachmentCommands/AudioAttachmentCommands/insertsectioncommand.h"
#include "AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.h"


const QString AudioAttachmentView::RECORD_LEFT_POSITION_ICON_PATH  = ":/icons/icons/turnleft.png";
const QString AudioAttachmentView::RECORD_RIGHT_POSITION_ICON_PATH = ":/icons/icons/turnright.png";

DEFN_CREATABLE( AudioAttachmentView, AttachmentView );

AudioAttachmentView::AudioAttachmentView(QWidget* parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::AudioAttachmentView)
{
    ui->setupUi( contentWidget() );
    setWidget( contentWidget() );


    connect( ui->pushButtonClearSection,    SIGNAL(clicked()),              this, SLOT(abortSection())                              );
    connect( ui->pushButtonRecordSection,   SIGNAL(clicked()),              this, SLOT(recordSection())                             );

    ui->pushButtonRecordSection->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );
    ui->pushButtonClearSection->setIcon( QIcon(":/icons/icons/cross56.png") );

    ui->sectionView->setContextMenuPolicy( Qt::ActionsContextMenu );
    ui->sectionView->horizontalHeader()->hide();
    ui->sectionView->verticalHeader()->hide();

    QAction* deleteSectionAction = new QAction( ui->sectionView );
    deleteSectionAction->setText(tr("Delete Section"));
    deleteSectionAction->setIcon(QIcon(":/icons/icons/rubbish7.png"));
    deleteSectionAction->setShortcut( QKeySequence("Del") );
    deleteSectionAction->setShortcutContext(Qt::WidgetShortcut);
    ui->sectionView->addAction( deleteSectionAction );
    connect( deleteSectionAction, SIGNAL(triggered()), this, SLOT(deleteCurrentSection()) );

    QAction* restoreSectionAction = new QAction( ui->sectionView );
    restoreSectionAction->setText(tr("Restore Section"));
    restoreSectionAction->setIcon(QIcon(":/icons/icons/download73.png"));
    restoreSectionAction->setShortcutContext(Qt::WidgetShortcut);
    ui->sectionView->addAction( restoreSectionAction );
    connect( restoreSectionAction, SIGNAL(triggered()), this, SLOT(restoreCurrentSection()) );

    connect( ui->sectionView, SIGNAL(clicked(QModelIndex)), this, SLOT(restoreCurrentSection()) );
    connect( ui->pushButtonRestoreSection, SIGNAL(clicked()), this, SLOT(restoreCurrentSection()) );
    connect( ui->pushButtonDeleteSection, SIGNAL(clicked()), this, SLOT(deleteCurrentSection()) );

    ui->sectionView->setItemDelegateForColumn(0, new LineEditDelegate(this));
    ui->sectionView->setItemDelegateForColumn(1, new DurationDelegate(this));
    ui->sectionView->setItemDelegateForColumn(2, new DurationDelegate(this));

}

AudioAttachmentView::~AudioAttachmentView()
{
    delete ui;
}

void AudioAttachmentView::polish()
{
#ifdef HAVE_SOUNDTOUCH
    IndexedFileAttachmentView::polish();

    AudioAttachment* a = attachment<AudioAttachment>();

    open();

    ui->playerWidget->setPlayer(&a->player());
    connect( a, &AudioAttachment::hashChanged, [this, a]()
    {
        // update duration, etc.
        ui->playerWidget->setPlayer(&a->player());
    });

    ui->sectionView->setModel( a->sectionsModel() );

    connect( a->sectionsModel(), &QAbstractTableModel::rowsAboutToBeRemoved, [this, a]()
    {
        a->player().setCurrentSection(Section());
    });
    connect( a->sectionsModel(), &QAbstractTableModel::rowsInserted, [this, a](const QModelIndex&, int index)
    {
        a->player().setCurrentSection( a->sectionsModel()->section(index) );
    });

#endif
}


void AudioAttachmentView::recordSection(bool abort)
{
#ifdef HAVE_SOUNDTOUCH
    enum State { Idle, LeftRecorded };
    static State state;
    static double leftPos = -1;

    AudioSlider* slider = ui->playerWidget->slider();

    if (abort)
    {
        state = Idle;
        ui->pushButtonRecordSection->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );
        slider->clearIndicators();
        attachment<AudioAttachment>()->player().setCurrentSection( Section() );
    }
    else
    {
        double pos = slider->value();
        if (state == Idle)
        {
            slider->clearIndicators();
            leftPos = pos;
            slider->setLeftIndicator( leftPos );
            attachment<AudioAttachment>()->player().setCurrentSection( Section() );

            state = LeftRecorded;
            ui->pushButtonRecordSection->setIcon( QIcon(RECORD_RIGHT_POSITION_ICON_PATH) );
        }
        else
        {
            slider->clearIndicators();

            Section section(tr("Unnamed"), leftPos, pos );

            app().pushCommand( new InsertSectionCommand(attachment<AudioAttachment>()->sectionsModel(), section));

            state = Idle;
            ui->pushButtonRecordSection->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );

            leftPos = -1;
        }
    }
#else
    Q_UNUSED( abort );
#endif
}

void AudioAttachmentView::restoreCurrentSection()
{
    Section section;
    QModelIndexList indexes = ui->sectionView->selectionModel()->selectedRows();
    if (!indexes.isEmpty() && indexes.first().isValid())
    {
        section = attachment<AudioAttachment>()->sectionsModel()->section( indexes.first().row() );
    }
    attachment<AudioAttachment>()->player().setCurrentSection( section );
}

void AudioAttachmentView::deleteCurrentSection()
{
    QModelIndexList indexes = ui->sectionView->selectionModel()->selectedRows();
    if (!indexes.isEmpty() && indexes.first().isValid())
    {
        attachment<AudioAttachment>()->sectionsModel()->removeSection( indexes.first().row() );
        attachment<AudioAttachment>()->player().setCurrentSection(Section());
    }
}

void AudioAttachmentView::chooseFile()
{
    ui->playerWidget->stop();
    IndexedFileAttachmentView::chooseFile();
}


