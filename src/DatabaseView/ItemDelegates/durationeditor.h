#ifndef DURATIONEDITOR_H
#define DURATIONEDITOR_H

#include <QLineEdit>

class DurationEditor : public QLineEdit
{
    Q_OBJECT
public:
    explicit DurationEditor(QWidget* parent = nullptr);
    void setTime(const QTime& time);
    QTime time() const;

    void increase();
    void decrease();

protected:
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
};

#endif // DURATIONEDITOR_H
