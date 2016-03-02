#include "mergelistwidgetitemwidget.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "optionpushbutton.h"

MergeListWidgetItemWidget::MergeListWidgetItemWidget(MergeItem *mergeItem) :
    QWidget(nullptr),
    m_mergeItem(mergeItem)
{
    QLabel* label = new QLabel(mergeItem->label(), this);
    QWidget* controlWidget = nullptr;

    controlWidget = makeButton();

    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    layout->addWidget(controlWidget);
    layout->addWidget(label);
    layout->setContentsMargins(0, 0, 0, 0);
    controlWidget->setFixedWidth(controlWidget->height());
}

QWidget* MergeListWidgetItemWidget::makeButton()
{
    OptionPushButton* button = new OptionPushButton(this);

    // setup the button
    switch (m_mergeItem->type())
    {
    case MergeItem::Add:
        button->setOptions( {
                                OptionPushButton::Option(":/icons/icons/plus44.png", tr("Add to master project")),
                                OptionPushButton::Option(":/icons/icons/cross56.png", tr("Don't add to master project"))
                            } );
        break;
    case MergeItem::Remove:
        button->setOptions( {
                                OptionPushButton::Option(":/icons/icons/anchor25.png", tr("Keep in master project")),
                                OptionPushButton::Option(":/icons/icons/minus38.png", tr("Remove from master project"))
                            } );
        break;
    case MergeItem::Modify:
        button->setOptions( {
                                OptionPushButton::Option(":/icons/icons/three91.png", tr("Edit details ..."))
                            });
        break;
    }

    // make connections
    switch (m_mergeItem->type())
    {
    case MergeItem::Add:
    case MergeItem::Remove:
        connect(button, &OptionPushButton::currentIndexChanged, [this](int index)
        {
            if (index == 0)
            {
                m_mergeItem->setAction(MergeItem::AddAction);
            }
            else if (index == 1)
            {
                m_mergeItem->setAction(MergeItem::RemoveAction);
            }
            else
            {
                Q_UNREACHABLE();
            }
        });
        break;
    case MergeItem::Modify:
        connect(button, &QPushButton::clicked, [this]()
        {
            emit clicked(m_mergeItem);
        });
        break;
    }

    return button;
}












