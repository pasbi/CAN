#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H


#include <QThread>
#include <QPainter>
#include "page.h"

class Setlist;
class AbstractChordPatternAttachment;
class PDFAttachment;
class ExportPDFDialog;
class Song;
class Attachment;
class AbstractRenderer : public QThread
{
    Q_OBJECT

public:
    AbstractRenderer(QSizeF baseSizeMM, Setlist* setlist, const QString& dirname );
    ~AbstractRenderer();

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
     *  on a AbstractRenderer.
     */
    virtual void run();

    bool isEndlessPage() const;

    Page *currentPage() const;
    int pageCount() const;

    static QImage paintChordPatternAttachment(AbstractChordPatternAttachment *attachment);


public:
    QList<Page*> m_pages;
    int m_currentIndex;
    QSizeF m_baseSizeMM;
    Setlist* m_setlist;
    const QString& m_filename;      // if each song gets one pdf, this will be the dirname (skipping *.pdf)


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
protected:
    void notifyCurrentTaskChanged( const QString& message );
signals:
    void progress(int);
    void currentTaskChanged(QString);



    ////////////////////////////////////
    ////
    ///  actual pdf-paint functions
    //
    ///////////////////////////////////
protected:
    // define a convienience struct to abbreviate QList<QList<QPage>> and handle the title
    struct Document
    {
        Document( const QString& title = "" ) : title(title) {}

        QString title;
        QList<Page*> pages;
    };
private:


    bool paintSong(const Song *song);
    void paintSetlist();
    void paintTableOfContents();
    void alignSongs( int mode );
    void optimizeForDuplex();
    void decoratePageNumbers();
    int lengthOfSong( int start );


    int m_tableOfContentsPage = -1;
    QStringList m_tableOfContents;

    ExportPDFDialog* m_exportPDFDialog = nullptr; // is required from so many member functions


public:
    static const QString HYPHEN;
    static const int ALIGN_SONGS__SEPARATE_PAGES;

    static bool pageBreak(const QStringList & lines, const int currentLine, const double heightLeft, const QPainter *painter );
    static void drawContinueOnNextPageMark(const Page *page, QPainter* painter);

private:
    static QMap<QString, QString> dictionary();
    static void paintHeadline(Page *page, const QString &label);

protected:
    QString labelSetlist(Setlist *setlist);

};

#endif // ABSTRACTRENDERER_H
