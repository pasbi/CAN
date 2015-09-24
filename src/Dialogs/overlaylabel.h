#ifndef HUDLABEL_H
#define HUDLABEL_H

#include <QLabel>

class OverlayDecorator;
class OverlayLabel : public QLabel
{
    Q_OBJECT
public:
    explicit OverlayLabel(QWidget *parent = 0);

    void setLinePos(int pos);
    void setOverlayText(const QString& text);

protected:
    void paintEvent(QPaintEvent *e);

private:
    int m_linePos;
    OverlayDecorator* m_overlayDecorator;

};

#endif // HUDLABEL_H
