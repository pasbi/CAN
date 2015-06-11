#ifndef HUD_H
#define HUD_H

#include <QLabel>
#include <QTimer>

class HUD : public QLabel
{
    Q_OBJECT
public:
    explicit HUD(QWidget *parent = 0);
    void setText( const QString& text );

protected:
//    void paintEvent(QPaintEvent *e);

public slots:
    void show();
    void replace();

private slots:
    void updateTransparency();

private:
    int m_i;
    int m_duration;
    QTimer* m_timer;
    QString m_text;


};

#endif // HUD_H
