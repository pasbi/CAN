#include "combinesongsdialog.h"

#include <QHBoxLayout>

#include "mergeitem.h"
#include "Database/databaseitem.h"
#include "mergelistwidget.h"


CombineSongsDialog::CombineSongsDialog(DatabaseMergerBase* merger, MergeItem* mergeItem, QWidget* parent) :
    CombineDatabaseItemsDialog(merger, mergeItem, parent),
    m_mergeListWidget(new MergeListWidget(this))
{
    DatabaseMergerBase* childMerger = databaseMerger()->child(mergeItem);
    Q_ASSERT(childMerger);
    m_mergeListWidget->setDatabaseMerger( childMerger );

    setDetailsPage(m_mergeListWidget, tr("&Attachments"));
}

CombineSongsDialog::~CombineSongsDialog()
{
}
