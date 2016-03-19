#include "setlistitemcombobox.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QScrollBar>

#include "global.h"

#include <QTimer>

SetlistItemComboBox::SetlistItemComboBox(QWidget *parent) :
    QLineEdit(parent),
    m_popupWidget(new QListWidget())
{
    m_popupWidget->setWindowFlags(Qt::ToolTip);
    m_popupWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_popupWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_popupWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_popupWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(m_popupWidget->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updatePopupSize()));
    m_popupWidget->setMouseTracking(true);
    connect(m_popupWidget, &QListWidget::itemEntered, [](QListWidgetItem* item){
        item->setSelected(true);
    });

    m_popupWidget->installEventFilter(this);
    installEventFilter(this);
}

SetlistItemComboBox::~SetlistItemComboBox()
{
    delete m_popupWidget;
    m_popupWidget = nullptr;
}

void SetlistItemComboBox::addItems(const QStringList &texts)
{
    m_items << texts;
    m_sortedItems = m_items;
    m_indexMap.clear();
    for (int i = 0; i < m_items.length(); ++i)
    {
        m_indexMap << i;
    }
}

void SetlistItemComboBox::sortItems()
{
    QString text = this->text();
    QMap<int, QString> items;
    QMap<int, int> indexes;
    for (int i = m_sortedItems.length() - 1; i >= 0; --i)
    {
        const QString& item = m_sortedItems[i];
        int key;
        if (item == text)
        {
            key = 1;
        }
        else if (item.startsWith(text, Qt::CaseSensitive))
        {
            key = 2;
        }
        else if (item.startsWith(text, Qt::CaseInsensitive))
        {
            key = 3;
        }
        else if (item.contains(text, Qt::CaseSensitive))
        {
            key = 4;
        }
        else if (item.contains(text, Qt::CaseInsensitive))
        {
            key = 5;
        }
        else
        {
            key = 6;
        }
        items.insertMulti(key, item);
        indexes.insertMulti(key, m_indexMap[i]);
    }
    m_sortedItems = items.values();
    m_indexMap = indexes.values();
}

void SetlistItemComboBox::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Up:
        selectItem(SelectPreviousItem);
        break;
    case Qt::Key_Down:
        selectItem(SelectNextItem);
        break;
    default:
        QLineEdit::keyPressEvent(e);
        updatePopup();
        break;
    }
}

QModelIndexList SetlistItemComboBox::visibleIndexes() const
{
    int first = m_popupWidget->indexAt(QPoint(5, 5)).row();
    int last  = m_popupWidget->indexAt(QPoint(5, m_popupWidget->viewport()->height() - 5)).row();

    if (first < 0)
    {
        first = 0;
    }
    if (last < 0)
    {
        last = m_popupWidget->count() - 1;
    }

    QModelIndexList list;
    for (int row = first; row <= last; ++row)
    {
        list << m_popupWidget->model()->index(row, 0);
    }
    return list;
}

void SetlistItemComboBox::updatePopupSize()
{
    int height = 0;
    int width = 0;

    for (const QModelIndex& index : visibleIndexes())
    {
        height += m_popupWidget->sizeHintForIndex(index).height();
        width = qMax(width, m_popupWidget->sizeHintForIndex(index).width());
    }

    height = qMin(height, 150); //TODO preferences

    width += 20;
    m_popupWidget->setFixedSize(width, height);
}

void SetlistItemComboBox::showPopup()
{
    m_popupWidget->show();

    m_popupWidget->move(mapToGlobal(QPoint(0, height())));
    QModelIndex index = m_popupWidget->model()->index(0, 0);
    m_popupWidget->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    updatePopup();

    selectItem(SelectFirsItem);
}

void SetlistItemComboBox::updatePopup()
{
    sortItems();
    m_popupWidget->clear();
    m_popupWidget->addItems(m_sortedItems);
    updatePopupSize();
    selectItem(SelectFirsItem);
}

void SetlistItemComboBox::selectItem(ItemSelectionMode mode)
{
    QModelIndexList indexes = m_popupWidget->selectionModel()->selectedRows();
    QModelIndex index;
    int row;
    if (indexes.isEmpty() || mode == SelectFirsItem)
    {
        row = 0;
    }
    else
    {
        index = indexes.first();
        row = index.row();
        if (mode == SelectNextItem)
        {
            row++;
        }
        else
        {
            row--;
        }
    }
    row %= m_popupWidget->count();
    row += m_popupWidget->count();
    row %= m_popupWidget->count();
    index = m_popupWidget->model()->index(row, 0);

    m_popupWidget->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    m_popupWidget->scrollTo(index);
    updatePopupSize();
}

void SetlistItemComboBox::setCurrentIndex(int index)
{
    setText(m_items[index]);
}

int SetlistItemComboBox::currentIndex() const
{
    QModelIndexList indexList = m_popupWidget->selectionModel()->selectedIndexes();
    if (!indexList.isEmpty())
    {
        int row = indexList.first().row();
        qDebug() << "current index: " << row;
        return m_indexMap[row];
    }
    else
    {
        qDebug() << "current index: null";
        return m_indexMap[0];
    }
}

void SetlistItemComboBox::showEvent(QShowEvent *e)
{
    showPopup();
    QLineEdit::showEvent(e);
}

bool SetlistItemComboBox::eventFilter(QObject *o, QEvent *e)
{
    if (o == this && e->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(e)->key() == Qt::Key_Return)
    {
        e->accept();
        return true;
    }
    else
    {
        return QLineEdit::eventFilter(o, e);
    }
}
