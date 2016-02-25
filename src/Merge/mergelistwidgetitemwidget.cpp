#include "mergelistwidgetitemwidget.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

MergeListWidgetItemWidget::MergeListWidgetItemWidget(MergeItemBase& mergeItem) :
    QWidget(nullptr),
    m_mergeItem(mergeItem)
{
    QLabel* label = new QLabel(mergeItem.label(), this);
    QWidget* controlWidget = nullptr;

    switch (mergeItem.origin()) {
    case MergeItemBase::MasterProject:
    case MergeItemBase::SlaveProject:
        controlWidget = makeComboBox(mergeItem.origin());
        break;
    case MergeItemBase::BothProjects:
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

QWidget* createCellWidget(MergeItemBase* item)
{
    QComboBox* box = new QComboBox();

    if (item->origin() == MergeItemBase::SlaveProject)
    {
        box->addItems( { QWidget::tr("Add to master project"),
                         QWidget::tr("Do not add to master project") } );
    }
    else if (item->origin() == MergeItemBase::MasterProject)
    {
        box->addItems( { QWidget::tr("Keep in master project"),
                         QWidget::tr("Remove from master project") } );
    }

    auto on_indexChanged = [item](int index)
    {
        if (index == 0)
        {
            item->setAction(MergeItemBase::AddItemAction);
        }
        else
        {
            item->setAction(MergeItemBase::DeleteItemAction);
        }
    };
    QObject::connect(box, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), on_indexChanged);
    on_indexChanged(box->currentIndex());

    return box;
}

QComboBox* MergeListWidgetItemWidget::makeComboBox(MergeItemBase::Origin origin)
{
    QComboBox* box = new QComboBox(this);
    switch (origin)
    {
    case MergeItemBase::SlaveProject:
        box->addItem( tr("Add to master project") );
        box->addItem( tr("Do not add to master project") );
        break;
    case MergeItemBase::MasterProject:
        box->addItem( tr("Keep in master project") );
        box->addItem( tr("Remove from master project") );
        break;
    case MergeItemBase::BothProjects:
        // see makePushButton
    case MergeItemBase::NoProject:
        Q_UNREACHABLE();
    }

    connect(box, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index)
    {
        if (index == 0)
        {
            m_mergeItem.setAction(MergeItemBase::AddItemAction);
            emit indexChanged(MergeItemBase::AddItemAction);
        }
        else
        {
            m_mergeItem.setAction(MergeItemBase::DeleteItemAction);
            emit indexChanged(MergeItemBase::DeleteItemAction);
        }
    });

    m_mergeItem.setAction(MergeItemBase::AddItemAction);
    return box;
}

QPushButton* MergeListWidgetItemWidget::makePushButton()
{
    QPushButton* button = new QPushButton(tr("Details ..."), this);

    connect(button, &QPushButton::clicked, [this]()
    {
        qDebug() << "open detail dialog ....";
        emit clicked();
    });

    return button;
}
