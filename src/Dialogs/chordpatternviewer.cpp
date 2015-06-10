#include "chordpatternviewer.h"
#include "ui_chordpatternviewer.h"
#include <QMessageBox>
#include <PDFCreator/pdfcreator.h>

ChordPatternViewer::ChordPatternViewer(ChordPatternAttachment* attachment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChordPatternViewer),
    m_attachment( attachment )
{
    ui->setupUi(this);
    m_pdfFile.open();

    PDFCreator::paintChordPatternAttachment( attachment, m_pdfFile.fileName() );

    //TODO load pdf document, show it.
}

ChordPatternViewer::~ChordPatternViewer()
{
    delete ui;
}

void ChordPatternViewer::displayChordPatternAttachment(ChordPatternAttachment *attachment, QWidget* parent)
{
    ChordPatternViewer cpv( attachment, parent );
    cpv.exec();

    if (cpv.scrollDownTempo() != attachment->scrollDownTempo())
    {
        if ( QMessageBox::question( parent,
                                    tr("Apply new tempo?"),
                                    tr("You changed the tempo of this chord pattern.\n"
                                       "Do you want to keep it?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::Yes )
             == QMessageBox::Yes )
        {
            attachment->setScrollDownTempo( cpv.scrollDownTempo() );
        }
    }
}
