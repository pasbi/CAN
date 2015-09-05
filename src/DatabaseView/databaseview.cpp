#include "databaseview.h"
#include <QPainter>
#include <QTimer>
#include <QAbstractAnimation>
#include <QHeaderView>

DatabaseViewBase::DatabaseViewBase(QWidget* parent) :
    QTableView(parent),
    m_hud( new HudDecorator(this, 1000, this) )
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    verticalHeader()->hide();
    horizontalHeader()->show();
    horizontalHeader()->setSortIndicatorShown( true );
    horizontalHeader()->setSectionsClickable( true );
    setSortingEnabled( true );

    setHorizontalScrollMode( QTableView::ScrollPerPixel );
    setVerticalScrollMode( QTableView::ScrollPerPixel );
}

void DatabaseViewBase::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
    emit clicked();
}

void DatabaseViewBase::paintEvent(QPaintEvent *e)
{
    QTableView::paintEvent(e);
    QPainter painter(viewport());
    m_hud->paint(painter);
}

void DatabaseViewBase::setFilter(const QString &filter)
{
    m_hud->setText(filter);
}

void DatabaseViewBase::focusOutEvent(QFocusEvent *event)
{
    setFilter("");
    QTableView::focusOutEvent(event);
}

bool isOk(const QString &string)
{
    if (string.length() != 1)
    {
        return false;
    }
    QChar c(string[0]);
    return c.isPrint();
}

void DatabaseViewBase::keyPressEvent(QKeyEvent *event)
{
    QString filter = this->filter();
    if (event->key() == Qt::Key_Escape)
    {
        setFilter("");
    }
    else if (event->key() == Qt::Key_Backspace)
    {
        setFilter(filter.left(filter.length()-1));
    }
    else if (isOk(event->text()))
    {
        setFilter(filter + event->text());
    }
}

void DatabaseViewBase::leaveEvent(QEvent *event)
{
    setFilter("");
    QTableView::leaveEvent(event);
}

void DatabaseViewBase::enterEvent(QEvent *event)
{
    setFocus();
    QTableView::enterEvent(event);
}
