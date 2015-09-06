#include "setlistitemselector.h"

#include "Project/project.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
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
    QDialogButtonBox* buttonBox = new QDialogButtonBox( this );
    buttonBox->setStandardButtons( QDialogButtonBox::Close );
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( songTableView );
    layout->addWidget( buttonBox );
    this->setLayout( layout );
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(hide()));
}

void SetlistItemSelector::showEvent(QShowEvent *e)
{
    //TODO we have an abstraction of QSettings!
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
