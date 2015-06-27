#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include "page.h"
#include <QThread>
#include "configurable.h"
#include "Database/EventDatabase/setlist.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Attachments/pdfattachment.h"
#include "orphantsetlist.h"
#include "Dialogs/exportpdfdialog.h"


class PDFCreator : public QThread
{
    DECL_CONFIG( PDFCreator )
    Q_OBJECT

public:
    PDFCreator(QSizeF baseSizeMM, Setlist* setlist, const QString& dirname );
    ~PDFCreator();

    QPainter &currentPainter();
    /**
     * @brief currentSizeInMM
     * @return the size of the current page in mm
     */
    QSizeF currentSizeInMM() const;

    /**
     * @brief currentSizePainter
     * @return the size of the current page in QPainter units.
     */
    QSizeF currentSizePainter() const;

    /**
     * @brief newPage insert a new page at index i (at the end if i<0) and
     *  activates it.
     * @param i the index of the new page.
     */
    void newPage( Page::Flags flags, const QString &title, int i);


    /**
     * @brief activatePage activate page i
     * @param i
     */
    void activatePage( int i );

    /**
     * @brief currentIndex returns the current index
     * @return
     */
    int currentIndex() const;


    /**
     * @brief run since run ivokes save, run can only be called once
     *  on a PDFCreator.
     */
    virtual void run();

    QRectF pageRect() const;
    QRectF pageRectMargins() const;

    bool isEndlessPage() const;

    const Page* currentPage() const;

    static void paintChordPatternAttachment(AbstractChordPatternAttachment *attachment, const QString& path);


private:
    QList<Page*> m_pages;
    int m_currentIndex;
    QSizeF m_baseSizeMM;
    Page *currentPage();
    Setlist* m_setlist;
    const QString& m_filename;      // if each song gets one pdf, this will be the dirname (skipping *.pdf)

    /**
     * @brief save saves the pdf at given filename.
     *  After calling this function, the PDFCreator is unable to paint
     *  on the pdf again.
     * @param filename
     */
    void save(QString filenameOrDirname );

    /////////////////////////////////////
    ////
    ///   progress controll
    //
    ////////////////////////////////////
public:
    /**
     * @brief numberOfSteps creating this pdf takes
     *  numberOfSteps() steps.
     * @return
     */
    int numberOfSteps() const;
private:
    int m_currentStep;
    void notifyCurrentTaskChanged( const QString& message );
signals:
    void progress(int);
    void currentTaskChanged(QString);



    ////////////////////////////////////
    ////
    ///  actual pdf-paint functions
    //
    ///////////////////////////////////
private:
    // define a convienience struct to abbreviate QList<QList<QPage>> and handle the title
    struct Document
    {
        Document( const QString& title = "" ) : title(title) {}

        QString title;
        QList<Page*> pages;
    };

    void paintTitle();
    void paintHeadline(const QString &label);
    bool paintSong(const Song *song);
    void paintAttachment(Attachment *attachment );
    void paintPDFAttachment(PDFAttachment *attachment );
    void paintChordPatternAttachment(AbstractChordPatternAttachment *attachment );
    void insertTableOfContentsStub();
    void paintSetlist();
    void paintTableOfContents();
    void alignSongs( int mode );
    void optimizeForDuplex();
    void decoratePageNumbers();
    void drawContinueOnNextPageMark();
    void paintAndSaveDocument(const Document& document, const QString &title, const QString& dirname );
    int lengthOfSong( int start );

    /**
     * @brief printAttachment returns wheter the @code attachment shall be printed.
     * @param attachment
     * @return
     */
    bool printAttachment( const Attachment* attachment );

    double m_additionalTopMargin = 0;
    int m_tableOfContentsPage = -1;
    QStringList m_tableOfContents;

    // margins in painter-units
    double leftMargin() const { return 35; }
    double rightMargin() const { return 10; }
    double topMargin() const { return 15 + m_additionalTopMargin; }
    double bottomMargin() const { return 15 + 25; } // bottom line is 15 below the end of the page

    ExportPDFDialog* m_exportPDFDialog = NULL; // is required from so many member functions

public:
    static const int ALIGN_SONGS__SEPARATE_PAGES;
    static QMap<QString, QString> dictionary();

    static QString setlistFilename(QWidget* parent, Setlist* setlist , bool separatePages);
    static void exportSetlist(Setlist *setlist, QWidget *widgetParent);



};

#endif // PDFCREATOR_H
