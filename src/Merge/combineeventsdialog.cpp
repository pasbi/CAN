#include "combineeventsdialog.h"

#include <QRadioButton>

#include "databasemergerbase.h"
#include "ui_combineeventsdialog.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/setlist.h"

CombineEventsDialog::CombineEventsDialog(DatabaseMergerBase *databaseMerger, MergeItem *mergItem, QWidget *parent) :
    CombineDatabaseItemsDialog(databaseMerger, mergItem, parent),
    ui(new Ui::CombineEventsDialog)
{
    QWidget* widget = new QWidget(this);
    ui->setupUi(widget);
    setDetailsPage(widget, tr("&Setlist"));

    mergeItem()->insertModifyDetail(MergeItem::ModifyDetail("setlist"));

    connect(ui->masterButton, &QRadioButton::clicked, [this]()
    {
        mergeItem()->updateModifyDetails({ MergeItem::ModifyDetail("setlist", MergeItem::UseMaster) });
    });

    connect(ui->slaveButton, &QRadioButton::clicked, [this]()
    {
        mergeItem()->updateModifyDetails({ MergeItem::ModifyDetail("setlist", MergeItem::UseSlave) });
    });

    switch (mergeItem()->modifyDetails()["setlist"].decision()) {
    case MergeItem::UseMaster:
        ui->masterButton->setChecked(true);
        break;
    case MergeItem::UseSlave:
        ui->slaveButton->setChecked(true);
        break;
    default:
        Q_UNREACHABLE();
    }

    ui->masterList->setModel(static_cast<Event*>(mergeItem()->master())->setlist());
    ui->slaveList->setModel(static_cast<Event*>(mergeItem()->slave())->setlist());
}

CombineEventsDialog::~CombineEventsDialog()
{
}
