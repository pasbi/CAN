#include "databaseview.h"
#include <QPainter>
#include <QTimer>
#include <QAbstractAnimation>
#include <QHeaderView>
#include <QApplication>
#include "overlaydecorator.h"

DatabaseViewBase::DatabaseViewBase(QWidget* parent) :
    QTableView(parent),
    m_hud( new OverlayDecorator(this, 1000, this) )
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

bool isAscendant(const QObject* candidate)
{
    QObject* object = QApplication::focusWidget();
    while (candidate != object && object)
    {
        object = object->parent();
    }

    return candidate == object;
}

bool cursorOnDatabaseView(const QWidget* widget)
{
    QPoint pos = widget->mapFromGlobal(QCursor::pos());
    if (widget->rect().contains(pos))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DatabaseViewBase::focusOutEvent(QFocusEvent *event)
{
    if (!cursorOnDatabaseView(this) && !isAscendant(this))
    {
        setFilter("");
    }
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
    if (!cursorOnDatabaseView(this) && !isAscendant(this))
    {
        setFilter("");
    }
    QTableView::leaveEvent(event);
}

void DatabaseViewBase::enterEvent(QEvent *event)
{
    setFocus();
    QTableView::enterEvent(event);
}

void DatabaseViewBase::setModel(QAbstractItemModel *model)
{
    QAbstractItemModel* oldModel = QTableView::model();
    if (oldModel)
    {
        disconnect(oldModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),              this, SIGNAL(changed()));
        disconnect(oldModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),    this, SIGNAL(changed()));
        disconnect(oldModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),                  this, SIGNAL(changed()));
        disconnect(oldModel, SIGNAL(rowsInserted(QModelIndex,int,int)),                 this, SIGNAL(changed()));
        disconnect(oldModel, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)), this, SIGNAL(changed()));
        disconnect(oldModel, SIGNAL(columnsRemoved(QModelIndex,int,int)),               this, SIGNAL(changed()));
        disconnect(oldModel, SIGNAL(columnsInserted(QModelIndex,int,int)),              this, SIGNAL(changed()));
        disconnect(oldModel, SIGNAL(modelReset()), this, SIGNAL(changed()));
    }
    QTableView::setModel(model);
    if (model)
    {
        connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),                    this, SIGNAL(changed()));
        connect(model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),          this, SIGNAL(changed()));
        connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)),                        this, SIGNAL(changed()));
        connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),                       this, SIGNAL(changed()));
        connect(model, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)),       this, SIGNAL(changed()));
        connect(model, SIGNAL(columnsRemoved(QModelIndex,int,int)),                     this, SIGNAL(changed()));
        connect(model, SIGNAL(columnsInserted(QModelIndex,int,int)),                    this, SIGNAL(changed()));
        connect(model, SIGNAL(modelReset()),                                            this, SIGNAL(changed()));
    }
}
