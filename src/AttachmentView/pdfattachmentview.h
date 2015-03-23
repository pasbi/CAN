#ifndef PDFATTACHMENTVIEW_H
#define PDFATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"
#include "attachmentview.h"
#include <poppler/qt5/poppler-qt5.h>
#include <QSignalSpy>




namespace Ui {
class PDFAttachmentView;
}

class PDFAttachmentView : public IndexedFileAttachmentView
{
    Q_OBJECT
    DECL_CREATABLE(PDFAttachmentView);

public:
    explicit PDFAttachmentView(QWidget *parent = 0);
    ~PDFAttachmentView();


    void restoreOptions(const QByteArray &options);
    QByteArray options() const;

protected:
    void polish();
    bool eventFilter(QObject *o, QEvent *e);
    void resizeEvent(QResizeEvent *e);

protected slots:
    void open();

private slots:
    void on_buttonZoomIn_clicked();
    void on_buttonZoomOut_clicked();
    void on_spinBoxScale_editingFinished();
    void on_buttonPreviousPage_clicked();
    void on_buttonNextPage_clicked();
    void on_spinBoxPage_valueChanged(int arg1);


private:
    Ui::PDFAttachmentView *ui;
    QWidget* m_pdfWidget = NULL;
    double m_zoom = 1;

    int m_currentPage = 0;
    void handlePageControlEnabled();

    QPixmap currentPixmap;
    QByteArray m_hash;  // hash of the document the rendering was made from.

    int numPages() const;

};

#endif // PDFATTACHMENTVIEW_H
