#include "setlistitemselector.h"

#include "Project/project.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSettings>
#include "application.h"
#include "Database/SongDatabase/songdatabase.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"

SetlistItemSelector::SetlistItemSelector(QWidget *parent) :
    QDialog(parent)
{
    SongTableView* songTableView = new SongTableView( this );
    songTableView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    QPushButton* button = new QPushButton( this );
    button->setText(tr("Close"));
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( songTableView );
    layout->addWidget( button, Qt::AlignRight );
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setShortcut(QKeySequence("Esc"));
    QHBoxLayout* hlayout = new QHBoxLayout( this );
    hlayout->addStretch();
    hlayout->addWidget(button);
    layout->addLayout(hlayout);
    this->setLayout( layout );
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
