#include "advancedfilechooser.h"
#include "ui_advancedfilechooser.h"
#include <QFileInfo>
#include "application.h"
#include <qmath.h>
#include <QFileDialog>
#include <QMessageBox>
#include "Dialogs/addfileindexsourcedialog.h"

AdvancedFileChooser::AdvancedFileChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvancedFileChooser)
{
    ui->setupUi(this);

    connect( ui->comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int i){
        if (i < 0)
        {
            setHash( QByteArray() );
        }
        else if ( i < m_filenames.length() )
        {
            QString filename = m_filenames[i];
            setHash( app().fileIndex().hash( filename ) );
        }
        else
        {
            ui->comboBox->setCurrentIndex( -1 );
        }
    });
}

AdvancedFileChooser::~AdvancedFileChooser()
{
    delete ui;
}

void AdvancedFileChooser::setHash(const QByteArray &hash)
{
    if (m_hash == hash)
        return;

    ui->comboBox->blockSignals(true);
    m_hash = hash;
    if (m_hash.isEmpty())
    {
        ui->comboBox->setCurrentIndex( -1 );
    }
    else
    {
        QString filename = QFileInfo( app().fileIndex().filename( m_hash ) ).fileName();
        bool comboBoxContainsFilename = false;
        for (int i = 0; i < ui->comboBox->count(); ++i)
        {
            if (filename == ui->comboBox->itemText(i))
            {
                comboBoxContainsFilename = true;
                break;
            }
        }
        if (!comboBoxContainsFilename)
        {
            ui->comboBox->addItem(filename);
        }
        ui->comboBox->setCurrentText( filename );
        emit itemSelected( m_hash );
    }
    ui->comboBox->blockSignals(false);
}

void AdvancedFileChooser::setFilterProperties(const Song *song, const QStringList & endings )
{
    m_song = song;
    m_endings = endings;
    updateComboBox();
}

bool rankingGreaterThan( const QPair<QString, double>& a, const QPair<QString, double>& b )
{
    return a.second > b.second;
}

double rank( const QString & candidate, const QMap<QString, QString> & attributes, const QStringList & endings )
{
    QFileInfo fileInfo( candidate );
    if ( !endings.contains(fileInfo.suffix()))
    {
        // dont propose e.g. pdf files for audio attachment
        return 0;
    }

    double rank = 0;

    // level:          4    3   2    1
    // filename:    /home/user/dir/song.mp3
    // when filename contains  an attribute of length n in level i, add
    // >> sqrt(n) / i <<   to rank,

    QStringList levels = fileInfo.absoluteFilePath().split("/", QString::SkipEmptyParts);
    for (int i = levels.length(); i > 0; --i)
    {
        for (const QString & attribute : attributes)
        {
            if ( levels[i-1].contains(attribute) )
            {
                rank += qLn( attribute.length() ) / i;
            }
        }
    }

    return rank;
}

QStringList filter( const QStringList &             candidates,
                    const QMap<QString, QString> &  attributes,
                    const QStringList &             endings,
                    const int                       maxItems = -1  )
{
    QList<QPair<QString, double>> ranking;
    for ( const QString & candidate : candidates )
    {
        double score = rank(candidate, attributes, endings);
        if (score > 0)
        {
            ranking.append( qMakePair(candidate, score) );
        }
    }

    qSort( ranking.begin(), ranking.end(), rankingGreaterThan );

    QStringList result;
    int i = 0;
    for (const QPair<QString, double> & p : ranking)
    {
        if (i++ == maxItems)
            break;

        result << p.first;
    }

    return result;

}

void AdvancedFileChooser::updateComboBox()
{
    QByteArray hash = m_hash;   // keep hash under all circumstances!

    m_filenames.clear();
    m_filenames = filter( app().fileIndex().filenames(), m_song->stringAttributes(), m_endings, 10 ); //TODO 10 must be configurable

    ui->comboBox->clear();
    for (const QString & f : m_filenames)
    {
        ui->comboBox->addItem( QFileInfo(f).fileName() );
    }

    setHash( hash );
}

void AdvancedFileChooser::on_pushButton_clicked()
{

    QString filename = QDir::homePath();
    if (app().fileIndex().contains( m_hash ))
    {
        filename = app().fileIndex().filename( m_hash );
    }

    QStringList filters;
    for (const QString & ending : m_endings)
    {
        filters.append( QString("*.%1").arg(ending) );
    }
    QString filter = QString("(%1)").arg( filters.join(" ") );

    filename = QFileDialog::getOpenFileName( this,
                                             tr("Select File"),
                                             filename,
                                             filter                );
    if (filename.isEmpty())
    {
        // user canceled
        return;
    }

    if (!app().fileIndex().contains(filename))
    {

        QMessageBox box( this );
        box.setWindowTitle(tr("File is not indexed."));
        box.setText(QString(tr("File %1 is not indexed.\n"
                               "Only indexed files can be used.\n")).arg(filename));

        QAbstractButton* addFolder =    box.addButton( tr("Add folder"), QMessageBox::AcceptRole  );
                                        box.addButton( tr("Cancel"),     QMessageBox::RejectRole  );

        box.exec();

        if ( box.clickedButton() == addFolder )
        {
            QString path = QFileInfo( filename ).dir().path();

            AddFileIndexSourceDialog dialog( this );
            dialog.setDirectory( path );
            dialog.setOptions( QFileDialog::ShowDirsOnly );
            dialog.setFileMode( QFileDialog::Directory );

            path = dialog.selectedFiles().first();
            if (dialog.exec() != QDialog::Accepted)
            {
                // user canceled
                return;
            }

            if (path.isEmpty())
            {
                // user canceled
                return;
            }
            app().fileIndex().addSource( path, dialog.filter() );
        }
    }

    assert( app().fileIndex().contains(filename) );
    qDebug() << "set filename = " << filename << " >hash = " << app().fileIndex().hash( filename );
    setHash( app().fileIndex().hash( filename ) );
}



