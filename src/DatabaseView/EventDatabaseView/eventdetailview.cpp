#include "eventdetailview.h"
#include "ui_eventdetailview.h"
#include <QDialog>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QDialogButtonBox>

EventDetailView::EventDetailView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDetailView),
    m_event(NULL)
{
    ui->setupUi(this);

    connect(ui->showCalendarBeginning, &QPushButton::clicked, [this]()
    {
        setBeginningDateTime(getDateTime( ui->dateTimeEditBeginning->dateTime() ));
    });

    connect(ui->showCalendarEnding, &QPushButton::clicked, [this]()
    {
        setEndingDateTime(getDateTime( ui->dateTimeEditEnding->dateTime() ));
    });

    connect(ui->dateTimeEditBeginning, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(setBeginningDateTime(QDateTime)));
    connect(ui->dateTimeEditEnding, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(setEndingDateTime(QDateTime)));

    setEvent( NULL );
}

EventDetailView::~EventDetailView()
{
    delete ui;
}

void EventDetailView::computeDuration()
{
    m_duration =
            ui->dateTimeEditEnding->dateTime().toMSecsSinceEpoch() -
            ui->dateTimeEditBeginning->dateTime().toMSecsSinceEpoch();
}

void EventDetailView::setEndingDateTime(QDateTime ending)
{
    ui->dateTimeEditEnding->blockSignals(true);
    ui->dateTimeEditEnding->setDateTime( ending );
    if (m_event)
    {
        m_event->setEnding( ending );
    }
    computeDuration();
    ui->dateTimeEditEnding->blockSignals(false);
}

void EventDetailView::setBeginningDateTime(QDateTime beginning, bool keepDuration)
{
    ui->dateTimeEditEnding->blockSignals(true);
    ui->dateTimeEditBeginning->blockSignals(true);
    QDateTime ending = QDateTime::fromMSecsSinceEpoch( m_duration + beginning.toMSecsSinceEpoch() );
    ui->dateTimeEditBeginning->setDateTime( beginning );

    if (keepDuration)
    {
        ui->dateTimeEditEnding->setDateTime( ending );
    }

    if (m_event)
    {
        m_event->setBeginning( beginning );
        if (keepDuration)
        {
            m_event->setEnding( ending );
        }
    }
    computeDuration();
    ui->dateTimeEditEnding->blockSignals(false);
    ui->dateTimeEditBeginning->blockSignals(false);
}

void EventDetailView::setEvent(Event *event)
{
    if (m_event)
    {
        m_event->disconnect(this);
    }
    m_event = event;
    if (m_event)
    {
        connect(ui->textEdit, &QTextEdit::textChanged, [this](){
             m_event->setNotice( ui->textEdit->toPlainText() );
        });

        setBeginningDateTime( m_event->beginning(), false );
        setEndingDateTime( m_event->ending() );
        ui->textEdit->setPlainText( m_event->notices() );
    }
    ui->dateTimeEditBeginning->setVisible( (bool) m_event );
    ui->dateTimeEditEnding->setVisible( (bool) m_event );
    ui->label->setVisible( (bool) m_event );
    ui->label_2->setVisible( (bool) m_event );
    ui->showCalendarBeginning->setVisible( (bool) m_event );
    ui->showCalendarEnding->setVisible( (bool) m_event );
    ui->textEdit->setVisible( (bool) m_event );
}

QDateTime EventDetailView::getDateTime(QDateTime current)
{
    QDialog dialog;
    QVBoxLayout layout;
    QCalendarWidget dateEdit;
    dateEdit.setSelectedDate( current.date() );
    layout.addWidget( &dateEdit );
    QTimeEdit timeEdit;
    timeEdit.setTime( current.time() );
    layout.addWidget( &timeEdit );
    QDialogButtonBox box;
    box.setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect(&box, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&box, SIGNAL(rejected()), &dialog, SLOT(reject()));
    layout.addWidget( &box );
    dialog.setLayout( &layout );

    if (dialog.exec() == QDialog::Accepted)
    {
        return QDateTime( dateEdit.selectedDate(), timeEdit.time() );
    }
    else
    {
        return QDateTime();
    }
}


