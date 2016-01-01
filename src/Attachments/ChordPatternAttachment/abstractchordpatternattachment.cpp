#include "abstractchordpatternattachment.h"
#include <QPainter>
#include "PDFCreator/pdfcreator.h"
#include "chord.h"

AbstractChordPatternAttachment::AbstractChordPatternAttachment() :
    Attachment()
{
}



static void configurePainter( QPainter* painter )
{
    QFont font = painter->font();
    font.setFamily( "Courier" );
    painter->setFont( font );
}

void AbstractChordPatternAttachment::paint(PDFCreator *pdfCreator)
{
    QPainter* painter = pdfCreator->currentPage()->painter();
    configurePainter( painter );
    QStringList lines = chordPattern().split("\n", QString::KeepEmptyParts);


    double y = pdfCreator->currentPage()->topMargin();
    double height = painter->fontMetrics().height();
    for (int i = 0; i < lines.length(); ++i)
    {
        QString line = lines[i];

        if (pdfCreator->isEndlessPage())
        {
            double spaceLeft = pdfCreator->currentPage()->contentRect().height() - y;

            if (spaceLeft < 0)
            {
                Page* currentPage = pdfCreator->currentPage();
                currentPage->growDownMM( -currentPage->painterUnitsInMM( spaceLeft ) );
            }
        }
        else
        {
            if ( pdfCreator->pageBreak( lines,
                                        i,
                                        pdfCreator->currentPage()->contentRect().bottom() - y,
                                        painter                            ))
            {
                    if (preference<bool>("ContiuneOnNextPageMark"))
                    {
                        PDFCreator::drawContinueOnNextPageMark(pdfCreator->currentPage(), painter);
                    }


                    painter->restore();
                    pdfCreator->newPage( Page::NothingSpecial );
                    painter = pdfCreator->currentPage()->painter();
                    painter->save();
                    configurePainter( painter );
                    y = pdfCreator->currentPage()->topMargin();
            }
        }

        QRegExp regexp( Chord::SPLIT_PATTERN );
        QStringList tokens;
        int lastJ = 0;
        int j = 0;
        while ( (j = regexp.indexIn(line, j)) >= 0 )
        {
            int n;
            n = regexp.matchedLength();
            tokens << line.mid(lastJ, j - lastJ);
            tokens << line.mid(j, n);
            lastJ = j + 1;
            j += n;
        }
        tokens << line.mid(lastJ);

        QStringList unusedA, unusedB;
        bool isChordLine = Chord::parseLine( line , unusedA, unusedB );

        int pos = pdfCreator->currentPage()->leftMargin();
        for (const QString & token : tokens)
        {
            painter->save();

            if (isChordLine && Chord(token).isValid())
            {
                QFont font = painter->font();
                font.setBold( true );
                painter->setFont( font );
            }
            int w = painter->fontMetrics().width( token );
            painter->drawText( QRectF( pos, y, pos + w, height ), token );
            pos += w;

            painter->restore();
        }

        if (isChordLine)
        {
            y += height * preference<double>("ChordLineSpacing");
        }
        else
        {
            y += height * preference<double>("LineSpacing");
        }
    }
}
