#include "advancedfilechooser.h"
#include "ui_advancedfilechooser.h"
#include <QFileInfo>
#include "application.h"
#include <qmath.h>

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
        else
        {
            QString filename = m_filenames[i];
            setHash( app().fileIndex().hash( filename ) );
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

    m_hash = hash;
    QString filename = app().fileIndex().filename( m_hash );
    ui->comboBox->setCurrentText( QFileInfo(filename).fileName() );
    emit itemSelected( m_hash );
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

    blockSignals(true);
    setHash( hash );
    blockSignals(false);
}
