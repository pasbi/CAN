#include "songtableviewcontainer.h"
#include "ui_songtableviewcontainer.h"
#include <QLineEdit>
#include <QPushButton>

SongTableViewContainer::SongTableViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongTableViewContainer)
{
    ui->setupUi(this);
    ui->comboBox->lineEdit()->setPlaceholderText(tr("Filter"));
    ui->comboBox->setInsertPolicy(QComboBox::InsertAtBottom);

    connect(ui->pushButton, &QPushButton::clicked, [this]()
    {
        ui->comboBox->setCurrentIndex(-1);
        ui->tableView->proxyModel()->setFilterWildcard("");
    });
}

SongTableViewContainer::~SongTableViewContainer()
{
    delete ui;
}

SongTableView* SongTableViewContainer::songTableView() const
{
    return ui->tableView;
}

void SongTableViewContainer::setModel(SongDatabaseSortProxy *model)
{
    ui->tableView->setModel(model);
    connect(ui->comboBox, SIGNAL(editTextChanged(QString)), model, SLOT(setFilterWildcard(QString)));
}
