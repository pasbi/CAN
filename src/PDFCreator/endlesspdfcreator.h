#ifndef ENDLESSPDFCREATOR_H
#define ENDLESSPDFCREATOR_H

#include "page.h"
#include <QThread>
#include "configurable.h"
#include "Database/EventDatabase/setlist.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Attachments/pdfattachment.h"

class EndlessPDFCreator : public QThread
{
    DECL_CONFIG( EndlessPDFCreator )
    Q_OBJECT

public:
    EndlessPDFCreator( QSizeF baseSizeMM, Setlist* setlist, const QString& filename );
    ~EndlessPDFCreator();

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
    void newPage(Page::Flags flags, int i = -1 );

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


private:
    QList<Page*> m_pages;
    int m_currentIndex;
    QSizeF m_baseSizeMM;
    Page *currentPage();
    Setlist* m_setlist;
    const QString& m_filename;

    /**
     * @brief save saves the pdf at given filename.
     *  After calling this function, the PDFCreator is unable to paint
     *  on the pdf again.
     * @param filename
     */
    void save( const QString& filename );

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
    void paintTitle();
    void paintHeadline(const QString &label);
    bool paintSong(const Song *song);
    void paintAttachment(Attachment *attachment );
    void paintPDFAttachment(PDFAttachment *attachment );
    void paintChordPatternAttachment(ChordPatternAttachment *attachment );
    void insertTableOfContentsStub();
    void paintSetlist();
    void paintTableOfContents();
    void alignSongs( int mode );
    void optimizeForDuplex();
    void decoratePageNumbers();
    void drawContinueOnNextPageMark();
    int lengthOfSong( int start );

    double m_additionalTopMargin = 0;
    int m_tableOfContentsPage = -1;
    QStringList m_tableOfContents;

    // margins in painter-units
    double leftMargin() const { return 15; }
    double rightMargin() const { return 10; }
    double topMargin() const { return 15 + m_additionalTopMargin; }
    double bottomMargin() const { return 15 + 25; } // bottom line is 15 below the end of the page
public:
    static QMap<QString, QString> dictionary();

};

#endif // ENDLESSPDFCREATOR_H
