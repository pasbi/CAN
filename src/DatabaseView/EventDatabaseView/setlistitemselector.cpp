#include "setlistitemselector.h"

#include "Project/project.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSettings>
#include "Database/SongDatabase/songdatabase.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "application.h"

SetlistItemSelector::SetlistItemSelector(QWidget *parent) :
    QDialog(parent)
{
    SongTableView* songTableView = new SongTableView( this );
    songTableView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    songTableView->setDetailedView(true);

    QVBoxLayout* vlayout = new QVBoxLayout();

    QHBoxLayout* hlayout = new QHBoxLayout();
    QPushButton* button = new QPushButton();
    button->setText(tr("Close"));
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setShortcut(QKeySequence("Esc"));
    hlayout->addStretch();
    hlayout->addWidget(button);

    vlayout->addWidget( songTableView );
    vlayout->addLayout(hlayout);
    setLayout(vlayout);

    connect(button, SIGNAL(clicked()), this, SLOT(hide()));

}

void SetlistItemSelector::showEvent(QShowEvent *e)
{
    restoreGeometry( QSettings().value("SetlistItemSelector_Geometry").toByteArray() );
    app().project()->setCommandFocalizesAffiliatedView( false );
    QDialog::showEvent(e);
}

void SetlistItemSelector::hideEvent(QHideEvent *e)
{
    QSettings().setValue( "SetlistItemSelector_Geometry", saveGeometry() );
    app().project()->setCommandFocalizesAffiliatedView( true );
    QDialog::hideEvent(e);
}
