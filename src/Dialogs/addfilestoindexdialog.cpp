//#include "addfilestoindexdialog.h"
//#include "ui_addfilestoindexdialog.h"
//#include <QFileDialog>
//#include <QCheckBox>

//DEFN_CONFIG( AddFilesToIndexDialog, "AddFilesToIndexDialog" );

//CONFIGURABLE_ADD_ITEM_HIDDEN( AddFilesToIndexDialog, defaultpath, QDir::homePath() );

//AddFilesToIndexDialog::AddFilesToIndexDialog(QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::AddFilesToIndexDialog)
//{
//    ui->setupUi(this);

//    ui->lineEdit->setText( config["defaultpath"].toString() );
//}

//AddFilesToIndexDialog::~AddFilesToIndexDialog()
//{
//    for (const QString& ending : m_checkBoxes.keys())
//    {
//        setAcceptEnding(ending, m_checkBoxes[ending]->isChecked());
//    }
//    delete ui;
//}

//void AddFilesToIndexDialog::on_buttonOpenFileDialog_clicked()
//{
//    QFileDialog::Options options = 0;
//    QString filename = QFileDialog::getExistingDirectory( this,
//                                                          tr("Directory ..."),
//                                                          ui->lineEdit->text(),
//                                                          options
//                                                          );

//    if (!filename.isEmpty())
//    {
//        ui->lineEdit->setText( filename );
//    }
//}

//QString AddFilesToIndexDialog::path() const
//{
//    return ui->lineEdit->text();
//}

//QMap<QString, bool> AddFilesToIndexDialog::acceptedEndings() const
//{
//    QMap<QString, bool> acceptedEndings;
//    const QVariantMap map = config["acceptedEndings"].toMap();
//    for (const QString& ending : map.keys())
//    {
//        acceptedEndings.insert(ending, map[ending].toBool());
//    }
//    return acceptedEndings;
//}
