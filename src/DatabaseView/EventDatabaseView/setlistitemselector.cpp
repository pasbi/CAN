#include "setlistitemselector.h"
#include "ui_setlistitemselector.h"
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
    buttonBox->setStandardButtons( QDialogButtonBox::Ok );
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( stc );
    layout->addWidget( buttonBox );
    this->setLayout( layout );
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(hide()));
}

SetlistItemSelector::~SetlistItemSelector()
{
}
