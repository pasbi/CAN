#include "durationeditor.h"
#include <QLocale>
#include <QRegExpValidator>
#include <QTime>
#include <QKeyEvent>

DurationEditor::DurationEditor(QWidget* parent) :
    QLineEdit(parent)
{
    setInputMask(QString("00:00%1").arg(QLocale().decimalPoint()) + "000;-");
    setValidator(new QRegExpValidator(QRegExp(QString("([0-5]|-)([0-9]|-):([0-5]|-)([0-9]|-)%1([0-9]|-){3,3}").arg(QLocale().decimalPoint())), this));
}

QString code()
{
    return QString("mm:ss%1zzz").arg(QLocale().decimalPoint());
}

void DurationEditor::setTime(const QTime& time)
{
    setText(time.toString(code()));
}

QTime DurationEditor::time() const
{
    return QTime::fromString("00:" + text().replace("-", "0"));
}


void DurationEditor::increase()
{
    if (!time().isValid())
    {
        setTime(QTime(0, 0, 0, 0));
    }
    else
    {
        setTime(time().addSecs(1));
    }
}

void DurationEditor::decrease()
{
    if (time() < QTime(0, 0, 1, 0))
    {
        setTime(QTime());
    }
    else
    {
        setTime(time().addSecs(-1));
    }
}

void DurationEditor::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up)
    {
        increase();
    }
    else if (e->key() == Qt::Key_Down)
    {
        decrease();
    }
    else
    {
        QLineEdit::keyPressEvent(e);
    }
}

void DurationEditor::wheelEvent(QWheelEvent *e)
{
    if (e->angleDelta().y() > 0)
    {
        increase();
    }
    else if (e->angleDelta().y() < 0)
    {
        decrease();
    }
}
