#ifndef PDFCONFIG_H
#define PDFCONFIG_H

#include "pdfcreator.h"
#include <QPagedPaintDevice>

QList<QPagedPaintDevice::PageSize> pageSizes()
{
    return QList<QPagedPaintDevice::PageSize>()
        << QPagedPaintDevice::A0
        << QPagedPaintDevice::A1
        << QPagedPaintDevice::A2
        << QPagedPaintDevice::A3
        << QPagedPaintDevice::A4
        << QPagedPaintDevice::A5
        << QPagedPaintDevice::A6
        << QPagedPaintDevice::A7
        << QPagedPaintDevice::A8
        << QPagedPaintDevice::A9
        << QPagedPaintDevice::B0
        << QPagedPaintDevice::B1
        << QPagedPaintDevice::B2
        << QPagedPaintDevice::B3
        << QPagedPaintDevice::B4
        << QPagedPaintDevice::B5
        << QPagedPaintDevice::B6
        << QPagedPaintDevice::B7
        << QPagedPaintDevice::B8
        << QPagedPaintDevice::B9
        << QPagedPaintDevice::B10
        << QPagedPaintDevice::C5E
        << QPagedPaintDevice::Comm10E
        << QPagedPaintDevice::DLE
        << QPagedPaintDevice::Executive
        << QPagedPaintDevice::Folio
        << QPagedPaintDevice::Ledger
        << QPagedPaintDevice::Legal
        << QPagedPaintDevice::Letter
        << QPagedPaintDevice::Tabloid
        << QPagedPaintDevice::Custom ;
}

int pageSizeToInt( QPagedPaintDevice::PageSize s )
{
    return pageSizes().indexOf( s );
}

QPagedPaintDevice::PageSize intToPageSize( int s )
{

    if (s < 0 || s >= pageSizes().length())
    {
        return QPagedPaintDevice::Custom;
    }
    else
    {
        return pageSizes()[s];
    }
}

CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFTitlePattern,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Title"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/PDFTitlePattern"),
                       "Setlist\n\n-- {EventTitle} --\n\n{Begin}",
                       ConfigurableItemOptions::TextEditOptions( QT_TRANSLATE_NOOP("ConfigurableItem", "Pattern") )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       SongTitle,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Song Title"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/SongTitle"),
                       true,
                       ConfigurableItemOptions::CheckboxOptions()
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       SongTitlePattern,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Song Title"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/SongTitlePattern"),
                       "{Title} -- {Artist}",
                       ConfigurableItemOptions::LineEditOptions( QT_TRANSLATE_NOOP("ConfigurableItem", "Pattern") )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       LineSpacing,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Line spacing"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/LineSpacing"),
                       1.3,
                       ConfigurableItemOptions::DoubleSpinBoxOptions( 0, 10, 0.01, "" )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       ChordLineSpacing,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Chord Line spacing"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/ChordLineSpacing"),
                       1,
                       ConfigurableItemOptions::DoubleSpinBoxOptions( 0, 10, 0.01, "" )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFSize,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Page size"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/PageSize"),
                       pageSizeToInt( QPdfWriter::A4 ),
                       ConfigurableItemOptions::ComboBoxOptions( QStringList() <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A0")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A1")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A2")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A3")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A4")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A5")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A6")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A7")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A8")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A9")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B0")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B1")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B2")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B3")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B4")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B5")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B6")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B7")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B8")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B9")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B10")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "C5E")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Comm10E")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "DLE")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Executive")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Folio")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Ledger")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Legal")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Letter")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Tabloid")
                                                                    )
                     );

const int PDFCreator::ALIGN_SONGS__SEPARATE_PAGES = 5;
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       AlignSongs,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Align songs: "),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/AlignSongs"),
                       0,
                       ConfigurableItemOptions::ComboBoxOptions( QStringList() << QT_TRANSLATE_NOOP("ConfigurableItem", "No alignment")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Odd pages")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Even pages")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Duplex")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Endless")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Separate pages"))
                       );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       TableOfContents,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Table of Contents"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/TableOfContents"),
                       true,
                       ConfigurableItemOptions::CheckboxOptions()
                     );

CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PageNumbers,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Page Numbers"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/PageNumbers"),
                       true,
                       ConfigurableItemOptions::CheckboxOptions()
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFResolution,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Resolution"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/Resolution"),
                       72,
                       ConfigurableItemOptions::AdvancedDoubleSliderOptions(0, 400, 1, QT_TRANSLATE_NOOP("ConfigurableItem", "dpi") )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       ContiuneOnNextPageMark,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Show to-be-continued-hint"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/PDFCreator/ContinueOnNextPageMark"),
                       true,
                       ConfigurableItemOptions::CheckboxOptions()
                     );

CONFIGURABLE_ADD_ITEM_HIDDEN( PDFCreator, DefaultPDFSavePath, QDir::homePath() );


#endif // PDFCONFIG_H
