#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>

class VolumeSlider : public QSlider
{
    Q_OBJECT
public:
    VolumeSlider(QWidget* parent = nullptr);
    int value() const;

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

    void drawRange(QPainter& painter, int start, int width, int pos);

private:
    bool m_muted;

public:
    void setMuted(bool muted);
    bool isMuted() const;

signals:
    void wasMuted(bool muted);
};

#endif // VOLUMESLIDER_H
