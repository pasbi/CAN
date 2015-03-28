#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class DateTimeDialog;
}

class DateTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateTimeDialog(QWidget *parent = 0);
    ~DateTimeDialog();

    QDateTime dateTime() const;
    void setDateTime( QDateTime dateTime );

private:
    Ui::DateTimeDialog *ui;
};

#endif // DATETIMEDIALOG_H
