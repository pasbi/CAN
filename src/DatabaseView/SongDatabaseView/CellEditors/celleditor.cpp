#include "celleditor.h"
#include "creatable.h"
#include <QHBoxLayout>
#include "global.h"
#include <QApplication>

CellEditor::CellEditor(QWidget *parent) :
    QWidget(parent),
    m_solitaryWidget( NULL )
{
}

void CellEditor::setSolitaryWidget(QWidget *widget)
{
    assert(m_solitaryWidget == NULL);
    assert(widget != NULL);

    m_solitaryWidget = widget;
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    setFocusProxy(widget);  // delegate focus to widget
    widget->installEventFilter(this);
}

void CellEditor::setCurrentData(const QVariant &data)
{
    m_currentData = data;
}

void CellEditor::setStyleOption( QStyleOptionViewItem option )
{
    m_styleOption = option;
}

void CellEditor::setIndex(const QModelIndex & index)
{
    m_index = index;
}

void CellEditor::setModel( DatabaseSortProxy<Song>* songDatabase )
{
    m_model = songDatabase;
}


bool CellEditor::eventFilter(QObject *o, QEvent *e)
{
    // The actual editor receives the focus out signal, but the tableview listens to a signal of this.
    // forward it with a new owner (this)
    if (o == m_solitaryWidget && e->type() == QEvent::FocusOut)
    {
        QApplication::sendEvent( this, e );
    }
    return QWidget::eventFilter(o, e);
}
