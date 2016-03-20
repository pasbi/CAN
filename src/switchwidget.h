#ifndef SWITCHWIDGET_H
#define SWITCHWIDGET_H

#include <QWidget>

class SwitchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchWidget(QWidget *parent = 0);
    int currentIndex() const;

public slots:
    void setCurrentIndex(int index);

signals:
    void currentIndexChanged(int);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);

private:
    int m_currentIndex;
    QString m_songLabel;
    QString m_eventLabel;

};

#endif // SWITCHWIDGET_H
