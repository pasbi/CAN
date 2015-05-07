#include "setlistitemselector.h"
#include "DatabaseView/SongDatabaseView/songtableviewcontainer.h"
#include "application.h"
#include "project.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>

SetlistItemSelector::SetlistItemSelector(QWidget *parent) :
    QDialog(parent)
{
    SongTableViewContainer* stc = new SongTableViewContainer( this );
    stc->songTableView()->setSelectionMode( QAbstractItemView::ExtendedSelection );
    stc->setModel( app().project()->songDatabaseProxy() );
    QDialogButtonBox* buttonBox = new QDialogButtonBox( this );
    buttonBox->setStandardButtons( QDialogButtonBox::Close );
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( stc );
    layout->addWidget( buttonBox );
    this->setLayout( layout );
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(hide()));
}

SetlistItemSelector::~SetlistItemSelector()
{
}
