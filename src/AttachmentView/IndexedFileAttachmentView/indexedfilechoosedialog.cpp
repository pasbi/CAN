#include "indexedfilechoosedialog.h"
#include "ui_indexedfilechoosedialog.h"
#include "application.h"
#include <qmath.h>
#include <QFileDialog>
#include <QMessageBox>

IndexedFileChooseDialog::IndexedFileChooseDialog(const Song *song, const QByteArray &currentHash, const QStringList &endings, QWidget *parent) :
    QDialog(parent),
    ui( new Ui::IndexedFileChooseDialog ),
    m_song( song ),
    m_hash( currentHash ),
    m_acceptedEndings( endings ),
    m_filenames( gatherFiles() )
{
    ui->setupUi(this);
    int i = 0;
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    for (const QString & filename : m_filenames )
    {
        ui->tableWidget->insertRow( i );

        QTableWidgetItem* filepathItem = new QTableWidgetItem( filename );
        filepathItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        QTableWidgetItem* filenameItem = new QTableWidgetItem( QFileInfo(filename).baseName() );
        filenameItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

        ui->tableWidget->setItem( i, 0, filenameItem );
        ui->tableWidget->setItem( i, 1, filepathItem );
        i++;
    }
    ui->tableWidget->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
    connect( ui->tableWidget->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(onSelectionChanged(QItemSelection, QItemSelection))  );

    setFilename( currentFilename() );

    connect( ui->pushButtonClearEdit, SIGNAL(clicked()), this, SLOT(clearFilename()) );
}

IndexedFileChooseDialog::~IndexedFileChooseDialog()
{
    delete ui;
}

bool rankingGreaterThan( const QPair<QString, double>& a, const QPair<QString, double>& b )
{
    return a.second > b.second;
}

#define utf( S ) QString::fromUtf8( S )
QString normalizeString( QString s )
{
    const QStringList replaceBySpace = QStringList( {".", ":", ";", "-", "#",
                                                     "~", "*", "+", "_", ",",
                                                     "`", "´", "=", "/", "\\",
                                                     "!", "\"", "§", "$", "%", "@" });
    const QMap<QString, QString> replaceMap = QMap<QString, QString>( { { utf("ä"), "ae" },
                                                                        { utf("ö"), "oe" },
                                                                        { utf("ü"), "ue" },
                                                                        { utf("ß"), "ss" },
                                                                        { utf("é"), "e" },
                                                                        { utf("è"), "e" },
                                                                        { utf("ô"), "o" }       //TODO cover more chars
                                                                      });
    for (const QString & token : replaceBySpace)
    {
        s.replace(token, " ");
    }
    for (const QString & token : replaceMap.keys())
    {
        s.replace( token, replaceMap[token] );
    }
    return s.simplified();


}
#undef utf8


double rank( const QString & candidate, const QMap<QString, QString> & attributes, const QStringList& endings )
{
    QFileInfo fileInfo( candidate );
    if (!endings.contains(fileInfo.suffix()))
    {
        return 0;
    }

    QString candidateWithoutEnding = QString("%1/%2").arg(fileInfo.path(), fileInfo.baseName());

    QStringList levels = candidateWithoutEnding.split("/", QString::SkipEmptyParts);
    QList<QStringList> levelss;
    int i = 0;
    for (const QString& level : levels)
    {
        levelss << normalizeString(level).split(" ", QString::SkipEmptyParts);
        i++;
    }


    QStringList attributeString;
    for (const QString & attribute : attributes.values())
    {
        attributeString << normalizeString(attribute).split(" ", QString::SkipEmptyParts);
    }

    double rank = 0;
    for (int i = 0; i < levelss.length(); ++i)
    {
        double factor = qPow(0.7, i);
        double count = 0;
        for (const QString& token : levelss[i])
        {
            if (attributeString.contains( token ))
            {
                if (token.length() == 1)
                {
                    count += 0.1;
                }
                else if (token.length() == 2)
                {
                    count += 0.2;
                }
                else if (token.length() == 3)
                {
                    count += 0.8;
                }
                else
                {
                    count += 1;
                }
            }
        }
        rank += factor * count;
    }
    return rank;
}

QStringList filter( const QStringList &             candidates,
                    const QMap<QString, QString> &  attributes,
                    const QStringList &             endings )
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
    for (const QPair<QString, double> & p : ranking)
    {
        result << p.first;
    }

    return result;
}

QStringList IndexedFileChooseDialog::gatherFiles()
{
    return filter( app().fileIndex().filenames(),
                   m_song->stringAttributes(),
                   m_acceptedEndings );
}

void IndexedFileChooseDialog::setFilename(const QString &filename)
{
    ui->tableWidget->blockSignals(true);
    if (filename.isEmpty())
    {
        m_hash.clear();
        ui->tableWidget->selectionModel()->clear();
    }
    else
    {
        QByteArray hash = app().fileIndex().hash( filename );
        if (hash.isEmpty())
        {
            QMessageBox::warning( this,
                                  tr("File not indexed"),
                                  tr("The file you want to use is not indexed.\n"
                                     "Index it before you can use it."),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton );
            return;
        }
        m_hash = hash;

        ui->lineEdit->setText( filename );

        int row = m_filenames.indexOf( filename );
        if (row < 0)
        {
            ui->tableWidget->selectionModel()->clear();
        }
        else
        {
            ui->tableWidget->selectRow( row );
        }

    }
    ui->tableWidget->blockSignals(false);
}

QString IndexedFileChooseDialog::currentFilename()
{
    return app().fileIndex().filename( m_hash );
}

void IndexedFileChooseDialog::on_pushButtonOpenFileDialog_clicked()
{
    QString filename = currentFilename();
    if (filename.isEmpty())
    {
        filename = QDir::homePath();
    }

    QString filter = "(";
    for (const QString& ending : m_acceptedEndings)
    {
        filter.append(QString("*.%1 ").arg(ending));
    }
    if (filter.endsWith(" "))
    {
        filter = filter.left( filter.length() - 1 );
    }
    filter.append(")");

    filename = QFileDialog::getOpenFileName( this, tr("Open File ..."), filename, filter );
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        setFilename( filename );
    }
}



void IndexedFileChooseDialog::on_lineEdit_editingFinished()
{
    QString filename = ui->lineEdit->text();
    if (QFileInfo(filename).isReadable() || filename.isEmpty())
    {
        setFilename( filename );
    }
    else
    {
        QMessageBox::warning( this,
                              tr("File is not readable."),
                              tr("The file is not readable.\n"),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        m_hash.clear();
    }
}

void IndexedFileChooseDialog::reject()
{
    m_hash.clear();
    QDialog::reject();
}

void IndexedFileChooseDialog::accept()
{
    if (!ui->lineEdit->text().isEmpty() && m_hash.isEmpty())
    {
        return;
    }
    else
    {
        QDialog::accept();
    }
}

void IndexedFileChooseDialog::onSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED( deselected );
    int row = -1;
    QModelIndexList rows = selected.indexes();
    if (!rows.isEmpty())
    {
        row = rows.first().row();
    }

    if (row >= 0)
    {
        setFilename( m_filenames[row] );
    }
    else
    {
        setFilename( "" );
    }
}

void IndexedFileChooseDialog::clearFilename()
{
    setFilename( "" );
    ui->lineEdit->clear();
    ui->tableWidget->selectionModel()->clearSelection();
}
