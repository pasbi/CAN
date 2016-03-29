#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include "abstractrenderer.h"

class PDFCreator : public AbstractRenderer
{
    Q_OBJECT

public:
    PDFCreator(QSizeF baseSizeMM, Setlist* setlist, const QString& dirname );
    ~PDFCreator();

    void paintAndSaveDocument(const Document& document, const QString &title, const QString& dirname );

    /**
     * @brief save saves the pdf at given filename.
     *  After calling this function, the AbstractRenderer is unable to paint
     *  on the pdf again.
     * @param filename
     */
    void save(QString filenameOrDirname );

    static void exportSetlist(Setlist *setlist, QWidget *widgetParent);

protected:
    void run();

private:
    static QString setlistFilename(QWidget* parent, Setlist* setlist , bool separatePages);


};

#endif // PDFCREATOR_H
