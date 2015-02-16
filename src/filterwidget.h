#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>

namespace Ui {
class FilterWidget;
}

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget *parent = 0);
    ~FilterWidget();
    void setFilterProxy( QSortFilterProxyModel* proxy );


public slots:
    void setPattern(const QString & pattern);

signals:
    void filterChanged(QString);

private:
    Ui::FilterWidget *ui;
    QSortFilterProxyModel* m_proxy = NULL;
};

#endif // FILTERWIDGET_H
