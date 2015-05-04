#include "audioattachmentview.h"
#include "ui_audioattachmentview.h"

const QString AudioAttachmentView::RECORD_LEFT_POSITION_ICON_PATH  = ":/oldIcons/oldIcons/turnleft.png";
const QString AudioAttachmentView::RECORD_RIGHT_POSITION_ICON_PATH = ":/oldIcons/oldIcons/turnright.png";
const QString AudioAttachmentView::RECORD_LEFT_POSITION_CAPTION = tr("Record left");
const QString AudioAttachmentView::RECORD_RIGHT_POSITION_CAPTION = tr("Record right");

DEFN_CREATABLE( AudioAttachmentView, AttachmentView );

AudioAttachmentView::AudioAttachmentView(QWidget* parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::AudioAttachmentView)
{
    m_audioWidget = new QWidget(this);
    ui->setupUi( m_audioWidget );
    setWidget( m_audioWidget );

    connect( ui->doubleSpinBoxTempo,        SIGNAL(valueChanged(double)),  this, SLOT(setPitchTempo()));
    connect( ui->doubleSpinBoxPitch,        SIGNAL(valueChanged(double)),  this, SLOT(setPitchTempo()));
    connect( ui->pushButtonPlayPause,       SIGNAL(clicked(bool)),         this, SLOT(on_pushButtonPlayPause_toggled(bool))        );
    connect( ui->pushButtonStop,            SIGNAL(clicked()),             this, SLOT(on_pushButtonStop_clicked())                 );
    connect( ui->slider,                    SIGNAL(valueChanged(double)),  this, SLOT(seek(double)) );


    QMenu* recordMenu = new QMenu( ui->toolButtonRecordSection );
    m_setPositionAction    = new QAction( QIcon(RECORD_LEFT_POSITION_ICON_PATH) , RECORD_LEFT_POSITION_CAPTION, recordMenu );
    m_abortRecordingAction = new QAction( QIcon(":/icons/icons/cross56.png"), tr("Abort recording"), recordMenu );
    recordMenu->addAction( m_setPositionAction );
    recordMenu->addAction( m_abortRecordingAction );
    ui->toolButtonRecordSection->setMenu( recordMenu );
    ui->toolButtonRecordSection->setPopupMode( QToolButton::MenuButtonPopup );
    ui->toolButtonRecordSection->setDefaultAction( m_setPositionAction );
    connect( recordMenu, SIGNAL(triggered(QAction*)), this, SLOT(recordSection(QAction*)) );
}

AudioAttachmentView::~AudioAttachmentView()
{
    on_pushButtonStop_clicked();
    delete ui;
}

void AudioAttachmentView::polish()
{
    IndexedFileAttachmentView::polish();

    AudioAttachment* a = attachment<AudioAttachment>();
    connect( &a->player(), SIGNAL(positionChanged(double)), ui->slider, SLOT(setValue(double)) );
    connect( &a->player(), SIGNAL(durationChanged(double)), ui->slider, SLOT(setMaximum(double)) );
    connect( a, &AudioAttachment::hashChanged, [this, a]()
    {
        ui->slider->setMaximum( a->player().duration() );
    });
    connect( a, SIGNAL(currentSectionChanged(const Section*)), ui->slider, SLOT(setSection(const Section*)));
    ui->slider->setSection( a->player().currentSection() );

    qDebug() << "set duration: " << a->player().duration();
    ui->slider->setMaximum( a->player().duration() );
    ui->doubleSpinBoxPitch->setValue( player().pitch() );
    ui->doubleSpinBoxTempo->setValue( player().tempo() );

    ui->sectionView->setModel( a->sectionsModel() );
}

void AudioAttachmentView::open()
{
}

void AudioAttachmentView::on_pushButtonStop_clicked()
{
    player().stop();
    ui->pushButtonPlayPause->blockSignals( true );
    ui->pushButtonPlayPause->setChecked( false );
    ui->pushButtonPlayPause->blockSignals( false );
}

void AudioAttachmentView::on_pushButtonPlayPause_toggled(bool checked)
{
    if (checked)
    {
        player().seek( player().position() );
        player().play();
    }
    else
    {
        player().pause();
    }
}

void AudioAttachmentView::seek(double pos)
{
    player().seek( pos );
}

void AudioAttachmentView::setPitchTempo()
{
    player().seek( ui->doubleSpinBoxPitch->value(), ui->doubleSpinBoxTempo->value(), player().position() );
}


void AudioAttachmentView::recordSection(QAction* action)
{
    enum State { Idle, LeftRecorded };
    static State state;
    static double leftPos = -1;

    if (action == m_abortRecordingAction)
    {
        state = Idle;
        m_setPositionAction->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );
        m_setPositionAction->setText( RECORD_LEFT_POSITION_CAPTION );
        ui->slider->clearIndicators();
    }
    else if (action == m_setPositionAction)
    {
        double pos = player().position();
        if (state == Idle)
        {
            ui->slider->clearIndicators();
            leftPos = pos;
            ui->slider->setLeftIndicator( leftPos );

            state = LeftRecorded;
            m_setPositionAction->setIcon( QIcon(RECORD_RIGHT_POSITION_ICON_PATH) );
            m_setPositionAction->setText( RECORD_RIGHT_POSITION_CAPTION );
        }
        else
        {
            ui->slider->clearIndicators();

            Section section(tr("Unnamed"), leftPos, pos );
            attachment<AudioAttachment>()->appendSection( section );

            state = Idle;
            m_setPositionAction->setIcon( QIcon(RECORD_LEFT_POSITION_ICON_PATH) );
            m_setPositionAction->setText( RECORD_LEFT_POSITION_CAPTION );

            leftPos = -1;
        }
    }
}
