#include "attachmentmergewidgetbase.h"
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>

AttachmentMergeWidgetBase::AttachmentMergeWidgetBase(MergeItem* mergeItem, QWidget *parent) :
    QWidget(parent),
    m_mergeItem(mergeItem)
{
    m_layout = new QVBoxLayout(this);
}

void AttachmentMergeWidgetBase::addEditorWidget(QWidget *masterWidget, QWidget *slaveWidget, const QString &label)
{
    QHBoxLayout* layout = new QHBoxLayout();

    masterWidget->setParent(this);
    slaveWidget->setParent(this);

    QRadioButton* masterButton = new QRadioButton(this);
    QRadioButton* slaveButton = new QRadioButton(this);
    QButtonGroup* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(masterButton);
    buttonGroup->addButton(slaveButton);

    masterButton->setText(label);
    slaveButton->setText(label);

    QPushButton* resetMasterEdit = new QPushButton("R", this);
    resetMasterEdit->setFixedWidth(resetMasterEdit->height());
    QPushButton* resetSlaveEdit = new QPushButton("R", this);
    resetSlaveEdit->setFixedWidth(resetSlaveEdit->height());

    QHBoxLayout* layoutMaster = new QHBoxLayout();
    QHBoxLayout* layoutSlave = new QHBoxLayout();

    Q_ASSERT(QString(masterWidget->metaObject()->className()) == QString(slaveWidget->metaObject()->className()));
    int widgetHeight = masterWidget->sizeHint().height();
    if (widgetHeight > 100)
    {
        layoutMaster->addWidget(masterButton);
        layoutMaster->addWidget(resetMasterEdit);
        QVBoxLayout* vLayoutMaster = new QVBoxLayout();
        vLayoutMaster->addLayout(layoutMaster);
        vLayoutMaster->addWidget(masterWidget);

        layoutSlave->addWidget(slaveButton);
        layoutSlave->addWidget(resetSlaveEdit);
        QVBoxLayout* vLayoutSlave = new QVBoxLayout();
        vLayoutSlave->addLayout(layoutSlave);
        vLayoutSlave->addWidget(slaveWidget);

        layout->addLayout(vLayoutMaster);
        layout->addLayout(vLayoutSlave);
    }
    else
    {
        layoutMaster->addWidget(masterButton);
        layoutMaster->addWidget(masterWidget);
        layoutMaster->addWidget(resetMasterEdit);

        layoutSlave->addWidget(slaveButton);
        layoutSlave->addWidget(slaveWidget);
        layoutSlave->addWidget(resetSlaveEdit);

        layout->addLayout(layoutMaster);
        layout->addLayout(layoutSlave);
    }

    m_layout->addLayout(layout);
}
