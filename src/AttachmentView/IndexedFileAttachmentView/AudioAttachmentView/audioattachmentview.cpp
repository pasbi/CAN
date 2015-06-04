#include "audioattachmentview.h"
#include "ui_audioattachmentview.h"

const QString AudioAttachmentView::RECORD_LEFT_POSITION_ICON_PATH  = ":/oldIcons/oldIcons/turnleft.png";
const QString AudioAttachmentView::RECORD_RIGHT_POSITION_ICON_PATH = ":/oldIcons/oldIcons/turnright.png";

DEFN_CREATABLE( AudioAttachmentView, AttachmentView );

AudioAttachmentView::AudioAttachmentView(QWidget* parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::AudioAttachmentView)
{
    m_audioWidget = new QWidget(this);
    ui->setupUi( m_audioWidget );
    setWidget( m_audioWidget );

    connect( ui->doubleSpinBoxTempo,        SIGNAL(valueChanged(double)),   this, SLOT(setPitchTempo()));
    connect( ui->doubleSpinBoxPitch,        SIGNAL(valueChanged(double)),   this, SLOT(setPitchTempo()));
    connect( ui->pushButtonPlayPause,       SIGNAL(clicked(bool)),          this, SLOT(on_pushButtonPlayPause_toggled(bool))        );
    connect( ui->pushButtonStop,            SIGNAL(clicked()),              this, SLOT(on_pushButtonStop_clicked())                 );
    connect( ui->slider,                    SIGNAL(valueChanged(double)),   this, SLOT(seek(double)) );
    connect( ui->pushButtonClearSection,    SIGNAL(clicked()),              this, SLOT(abortSection()) );
    connect( ui->pushButtonRecordSection,   SIGNAL(clicked()),              this, SLOT(recordSection()) );


    ui->pushButtonRecordSection->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );
    ui->pushButtonClearSection->setIcon( QIcon(":/icons/icons/cross56.png") );

    ui->sectionView->setContextMenuPolicy( Qt::ActionsContextMenu );
    ui->sectionView->horizontalHeader()->hide();
    ui->sectionView->verticalHeader()->hide();

    QAction* deleteSectionAction = new QAction( ui->sectionView );
    deleteSectionAction->setText(tr("Delete Section"));
    deleteSectionAction->setIcon(QIcon(":/icons/icons/rubbish7.png"));
    deleteSectionAction->setShortcut( QKeySequence("Del") );
    ui->sectionView->addAction( deleteSectionAction );
    connect( deleteSectionAction, SIGNAL(triggered()), this, SLOT(deleteCurrentSection()) );

    QAction* restoreSectionAction = new QAction( ui->sectionView );
    restoreSectionAction->setText(tr("Restore Section"));
    restoreSectionAction->setIcon(QIcon(":/icons/icons/download73.png"));
    ui->sectionView->addAction( restoreSectionAction );
    connect( restoreSectionAction, SIGNAL(triggered()), this, SLOT(restoreCurrentSection()) );

    connect( ui->sectionView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(restoreCurrentSection()) );
    connect( ui->pushButtonRestoreSection, SIGNAL(clicked()), this, SLOT(restoreCurrentSection()) );
    connect( ui->pushButtonDeleteSection, SIGNAL(clicked()), this, SLOT(deleteCurrentSection()) );
}

AudioAttachmentView::~AudioAttachmentView()
{
    on_pushButtonStop_clicked();
    ui->pushButtonStop->click();
    delete ui;
}

void AudioAttachmentView::polish()
{
    IndexedFileAttachmentView::polish();

    AudioAttachment* a = attachment<AudioAttachment>();

//    connect( &a->player(), &Player::positionChanged, [this](double value)
//    {
//        ui->slider->setValue( value / ui->doubleSpinBoxTempo->value() );
//    });

//    connect( &a->player(), SIGNAL(durationChanged(double)), ui->slider, SLOT(setMaximum(double)) );

    connect( a, &AudioAttachment::hashChanged, [this, a]()
    {
//        ui->slider->setMaximum( a->player().duration() );
    });
    connect( a, SIGNAL(currentSectionChanged(const Section*)), ui->slider, SLOT(setSection(const Section*)));
//    ui->slider->setSection( a->player().currentSection() );

//    ui->slider->setMaximum( a->player().duration() );
//    ui->doubleSpinBoxPitch->setValue( player().pitch() );
//    ui->doubleSpinBoxTempo->setValue( player().tempo() );

    ui->sectionView->setModel( a->sectionsModel() );
    open();
}

void AudioAttachmentView::on_pushButtonStop_clicked()
{
//    player().stop();
    ui->pushButtonPlayPause->blockSignals( true );
    ui->pushButtonPlayPause->setChecked( false );
    ui->pushButtonPlayPause->blockSignals( false );
}

void AudioAttachmentView::on_pushButtonPlayPause_toggled(bool checked)
{
    if (checked)
    {
//        player().seek( player().position() );
//        player().play();
    }
    else
    {
//        player().pause();
    }
}

void AudioAttachmentView::seek(double pos)
{
//    player().seek( ui->doubleSpinBoxTempo->value() * pos );
}

void AudioAttachmentView::setPitchTempo()
{
//    player().seek( ui->doubleSpinBoxPitch->value(), ui->doubleSpinBoxTempo->value(), player().position() );
}


void AudioAttachmentView::recordSection(bool abort)
{
    enum State { Idle, LeftRecorded };
    static State state;
    static double leftPos = -1;

    if (abort)
    {
        state = Idle;
        ui->pushButtonRecordSection->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );
        ui->slider->clearIndicators();
        attachment<AudioAttachment>()->setSection( NULL );
    }
    else
    {
        double pos = 0;// player().position();
        if (state == Idle)
        {
            ui->slider->clearIndicators();
            leftPos = pos;
            ui->slider->setLeftIndicator( leftPos );
            attachment<AudioAttachment>()->setSection( NULL );

            state = LeftRecorded;
            ui->pushButtonRecordSection->setIcon( QIcon(RECORD_RIGHT_POSITION_ICON_PATH) );
        }
        else
        {
            ui->slider->clearIndicators();

            Section section(tr("Unnamed"), leftPos, pos );
            attachment<AudioAttachment>()->appendSection( section );

            state = Idle;
            ui->pushButtonRecordSection->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );

            leftPos = -1;
        }
    }
}

void AudioAttachmentView::restoreCurrentSection()
{
    const Section* section = NULL;
    QModelIndexList indexes = ui->sectionView->selectionModel()->selectedRows();
    if (!indexes.isEmpty() && indexes.first().isValid())
    {
        section = attachment<AudioAttachment>()->sectionsModel()->section( indexes.first().row() );
    }
    attachment<AudioAttachment>()->setSection( section );
}

void AudioAttachmentView::deleteCurrentSection()
{
    QModelIndexList indexes = ui->sectionView->selectionModel()->selectedRows();
    if (!indexes.isEmpty() && indexes.first().isValid())
    {
        attachment<AudioAttachment>()->sectionsModel()->removeSection( indexes.first().row() );
    }
}

void AudioAttachmentView::open()
{
    IndexedFileAttachmentView::open();
}

void AudioAttachmentView::chooseFile()
{
    on_pushButtonStop_clicked();
    ui->pushButtonStop->click();
    IndexedFileAttachmentView::chooseFile();
}

