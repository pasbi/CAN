#ifndef PDFCREATOR_H
#define PDFCREATOR_H


#include <QThread>
#include <QPainter>
#include "configurable.h"
#include "page.h"

class Setlist;
class AbstractChordPatternAttachment;
class PDFAttachment;
class ExportPDFDialog;
class Song;
class Attachment;
class PDFCreator : public QThread
{
    DECL_CONFIG( PDFCreator )
    Q_OBJECT

public:
    PDFCreator(QSizeF baseSizeMM, Setlist* setlist, const QString& dirname );
    ~PDFCreator();

    /**
     * @brief newPage insert a new page at index i (at the end if i<0) and
     *  activates it.
     * @param i the index of the new page.
     */
    void newPage( Page::Flags flags, const QString &title = "", int index = -1);


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
     * @brief run since run calls save, run can only be called once
     *  on a PDFCreator.
     */
    virtual void run();

    bool isEndlessPage() const;

    Page *currentPage() const;

    static void paintChordPatternAttachment(AbstractChordPatternAttachment *attachment, const QString& path);


public:
    QList<Page*> m_pages;
    int m_currentIndex;
    QSizeF m_baseSizeMM;
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


    bool paintSong(const Song *song);
    void paintSetlist();
    void paintTableOfContents();
    void alignSongs( int mode );
    void optimizeForDuplex();
    void decoratePageNumbers();
    void paintAndSaveDocument(const Document& document, const QString &title, const QString& dirname );
    int lengthOfSong( int start );


    int m_tableOfContentsPage = -1;
    QStringList m_tableOfContents;

    ExportPDFDialog* m_exportPDFDialog = nullptr; // is required from so many member functions


public:
    static const QString HYPHEN;
    static void exportSetlist(Setlist *setlist, QWidget *widgetParent);
    static bool pageBreak(const QStringList & lines, const int currentLine, const double heightLeft, const QPainter *painter );
    static void drawContinueOnNextPageMark(const Page *page, QPainter* painter);

private:
    static const int ALIGN_SONGS__SEPARATE_PAGES;
    static QMap<QString, QString> dictionary();
    static QString setlistFilename(QWidget* parent, Setlist* setlist , bool separatePages);
    static void paintHeadline(Page *page, const QString &label);



};

#endif // PDFCREATOR_H
