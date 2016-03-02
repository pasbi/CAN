#include "mergelistwidgetitemwidget.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

MergeListWidgetItemWidget::MergeListWidgetItemWidget(MergeItem *mergeItem) :
    QWidget(nullptr),
    m_mergeItem(mergeItem)
{
    QLabel* label = new QLabel(mergeItem->label(), this);
    QWidget* controlWidget = nullptr;

    switch (mergeItem->type()) {
    case MergeItem::Add:
    case MergeItem::Remove:
        controlWidget = makeComboBox(mergeItem->type());
        break;
    case MergeItem::Modify:
        controlWidget = makePushButton();
        break;
    default:
        Q_UNREACHABLE();
    }

    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    layout->addWidget(label);
    layout->addWidget(controlWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    controlWidget->setFixedWidth(250);
}

QWidget* createCellWidget(MergeItem* item)
{
    QComboBox* box = new QComboBox();

    if (item->type() == MergeItem::Add)
    {
        box->addItems( { QWidget::tr("Add to master project"),
                         QWidget::tr("Do not add to master project") } );
    }
    else if (item->type() == MergeItem::Remove)
    {
        box->addItems( { QWidget::tr("Keep in master project"),
                         QWidget::tr("Remove from master project") } );
    }

    auto on_indexChanged = [item](int index)
    {
        if (index == 0)
        {
            item->setAction(MergeItem::AddAction);
        }
        else
        {
            item->setAction(MergeItem::RemoveAction);
        }
    };
    QObject::connect(box, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), on_indexChanged);
    on_indexChanged(box->currentIndex());

    return box;
}

QComboBox* MergeListWidgetItemWidget::makeComboBox(MergeItem::Type type)
{
    QComboBox* box = new QComboBox(this);
    switch (type)
    {
    case MergeItem::Add:
        box->addItem( tr("Add to master project") );
        box->addItem( tr("Do not add to master project") );
        break;
    case MergeItem::Remove:
        box->addItem( tr("Keep in master project") );
        box->addItem( tr("Remove from master project") );
        break;
    case MergeItem::Modify:
    default:
        Q_UNREACHABLE();
        return nullptr;
    }

    connect(box, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index)
    {
        MergeItem::Action action;
        if (index == 0)
        {
            action = MergeItem::AddAction;
        }
        else if (index == 1)
        {
            action = MergeItem::RemoveAction;
        }
        else
        {
            Q_UNREACHABLE();
        }
        m_mergeItem->setAction(action);
        emit indexChanged(action);
    });

    m_mergeItem->setAction(m_mergeItem->action());
    return box;
}

QPushButton* MergeListWidgetItemWidget::makePushButton()
{
    QPushButton* button = new QPushButton(tr("Details ..."), this);

    connect(button, &QPushButton::clicked, [this]()
    {
        emit clicked();
    });

    return button;
}
