#include "datetimedialog.h"
#include "ui_datetimedialog.h"
#include "global.h"

DateTimeDialog::DateTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateTimeDialog)
{
    ui->setupUi(this);
    connect(ui->calendarWidgetBeginning, SIGNAL(clicked(QDate)), this, SLOT(holdDuration()));
    connect(ui->calendarWidgetEnding, SIGNAL(clicked(QDate)), this, SLOT(computeDuration()));
    connect(ui->timeEditBeginning, SIGNAL(timeChanged(QTime)), this, SLOT(holdDuration()));
    connect(ui->timeEditEnding, SIGNAL(timeChanged(QTime)), this, SLOT(computeDuration()));

    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

DateTimeDialog::~DateTimeDialog()
{
    delete ui;
}

TimeSpan DateTimeDialog::timeSpan() const
{
    return TimeSpan(
                QDateTime( ui->calendarWidgetBeginning->selectedDate(), ui->timeEditBeginning->time() ),
                QDateTime( ui->calendarWidgetEnding->selectedDate(),    ui->timeEditEnding->time()    )
                );
}

void DateTimeDialog::setTimeSpan(TimeSpan timeSpan)
{
    ui->timeEditBeginning->setTime( timeSpan.beginning.time() );
    ui->calendarWidgetBeginning->setSelectedDate( timeSpan.beginning.date() );
    ui->timeEditEnding->setTime( timeSpan.ending.time() );
    ui->calendarWidgetEnding->setSelectedDate( timeSpan.ending.date() );

    computeDuration();
}

void DateTimeDialog::computeDuration()
{
    TimeSpan ts = timeSpan();
    m_duration = ts.ending.toMSecsSinceEpoch() - ts.beginning.toMSecsSinceEpoch();
}

void DateTimeDialog::holdDuration()
{
    QDateTime ending = QDateTime::fromMSecsSinceEpoch( timeSpan().beginning.toMSecsSinceEpoch() + m_duration );
    ui->timeEditEnding->setTime( ending.time() );
    ui->calendarWidgetEnding->setSelectedDate( ending.date() );
}
