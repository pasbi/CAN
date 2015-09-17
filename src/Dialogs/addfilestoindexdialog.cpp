#include "addfilestoindexdialog.h"
#include "ui_addfilestoindexdialog.h"
#include <QFileDialog>
#include <QCheckBox>

DEFN_CONFIG( AddFilesToIndexDialog, "AddFilesToIndexDialog" );

CONFIGURABLE_ADD_ITEM_HIDDEN( AddFilesToIndexDialog, defaultpath, QDir::homePath() );
CONFIGURABLE_ADD_ITEM_HIDDEN( AddFilesToIndexDialog, acceptedEndings, QVariantMap() );

CONFIGURABLE_ADD_ITEM( AddFilesToIndexDialog,
                       endings,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Endings"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "All Endings that may be included"),
                       (QStringList() << "mp3" << "ogg" << "aif" << "aiff" << "pdf").join(QChar(0x2C)), /* 0x2C = , macro does not allow to write it!*/
                       ConfigurableItemOptions::LineEditOptions("") );

AddFilesToIndexDialog::AddFilesToIndexDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFilesToIndexDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setText( config["defaultpath"].toString() );

    QStringList endings = config["endings"].toString().split(",");
    for (QString& ending: endings)
    {
        ending = ending.remove(QRegExp("\\W"));
    }
    endings = endings.filter(QRegExp(".+"));

    setEndings(endings);
}

bool AddFilesToIndexDialog::acceptEnding(const QString &ending) const
{
    return config["acceptedEndings"].toMap().value(ending, QVariant(false)).toBool();
}

void AddFilesToIndexDialog::setAcceptEnding(const QString &ending, bool accept)
{
    QVariantMap map = config["acceptedEndings"].toMap();
    map.insert(ending, accept);
    config.set("acceptedEndings", map);
}
void AddFilesToIndexDialog::setEndings(const QStringList &endings)
{
    for (const QString& ending : endings)
    {
        QCheckBox* checkBox = new QCheckBox(this);
        checkBox->setText(tr("*.%1").arg(ending));
        checkBox->setChecked(acceptEnding(ending));
        ui->endingCheckBoxesLayout->addWidget(checkBox);
        m_checkBoxes.insert(ending, checkBox);
    }
}

AddFilesToIndexDialog::~AddFilesToIndexDialog()
{
    for (const QString& ending : m_checkBoxes.keys())
    {
        setAcceptEnding(ending, m_checkBoxes[ending]->isChecked());
    }
    delete ui;
}

void AddFilesToIndexDialog::on_buttonOpenFileDialog_clicked()
{
    QFileDialog::Options options = 0;
    QString filename = QFileDialog::getExistingDirectory( this,
                                                          tr("Directory ..."),
                                                          ui->lineEdit->text(),
                                                          options
                                                          );

    if (!filename.isEmpty())
    {
        ui->lineEdit->setText( filename );
    }
}

QString AddFilesToIndexDialog::path() const
{
    return ui->lineEdit->text();
}

QMap<QString, bool> AddFilesToIndexDialog::acceptedEndings() const
{
    QMap<QString, bool> acceptedEndings;
    const QVariantMap map = config["acceptedEndings"].toMap();
    for (const QString& ending : map.keys())
    {
        acceptedEndings.insert(ending, map[ending].toBool());
    }
    return acceptedEndings;
}
