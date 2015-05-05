#include "combobox.h"
#include <QLineEdit>
#include <QDebug>
#include <QPainter>

ComboBox::ComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void ComboBox::paintEvent(QPaintEvent *e)
{
    QComboBox::paintEvent(e);
    if (currentIndex() < 0)
    {
        QPainter painter(this);
        QTextOption option;
        option.setAlignment( Qt::AlignVCenter );
        QRect r = rect();
        r.setLeft( r.left() + 7 );
        painter.drawText( r, m_text, option );
    }
}

void ComboBox::setInvalidText(const QString &text)
{
    m_text = text;
}
