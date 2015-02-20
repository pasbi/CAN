#include "advancedfilechooser.h"
#include "ui_advancedfilechooser.h"
#include <QFileInfo>

AdvancedFileChooser::AdvancedFileChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvancedFileChooser)
{
    ui->setupUi(this);
}

AdvancedFileChooser::~AdvancedFileChooser()
{
    delete ui;
}

void AdvancedFileChooser::setHash(const QByteArray &hash)
{
    assert( m_fileIndex );
    m_hash = hash;
    QString filename = m_fileIndex->filename( m_hash );
    ui->comboBox->setCurrentText( QFileInfo(filename).fileName() );
}

void AdvancedFileChooser::setFilter(const QStringList &filter)
{
    m_filter = filter;
    updateComboBox();
}

void AdvancedFileChooser::setIndex(const FileIndex *fileIndex)
{
    m_fileIndex = fileIndex;
    updateComboBox();
}

void AdvancedFileChooser::updateComboBox()
{
    m_filenames.clear();
    qDebug() << m_fileIndex->filenames(QStringList("*"));
    if (m_fileIndex)
    {
        m_filenames = m_fileIndex->filenames( m_filter );
    }

    ui->comboBox->clear();
    for (const QString & f : m_filenames)
    {
        ui->comboBox->addItem( QFileInfo(f).fileName() );
    }
}
