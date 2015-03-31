#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include <QDialog>
#include <QDateTime>
#include "commontypes.h"

namespace Ui {
class DateTimeDialog;
}

class DateTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateTimeDialog(QWidget *parent = 0);
    ~DateTimeDialog();

    TimeSpan timeSpan() const;
    void setTimeSpan( TimeSpan timeSpan );    

private:
    Ui::DateTimeDialog *ui;
    qint64 m_duration = 0;

private slots:
    void computeDuration();
    void holdDuration();
};

#endif // DATETIMEDIALOG_H
