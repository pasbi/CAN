#ifndef SETLISTWIDGET_H
#define SETLISTWIDGET_H

#include <QWidget>

namespace Ui {
class SetListWidget;
}

class SetListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetListWidget(QWidget *parent = 0);
    ~SetListWidget();

private slots:
    void on_buttonSortUp_clicked();

    void on_buttonSortDown_clicked();

    void on_buttonDelete_clicked();

    void on_buttonAdd_clicked();

private:
    Ui::SetListWidget *ui;
};

#endif // SETLISTWIDGET_H
