#include "pdfattachment.h"
#include "PDFCreator/pdfcreator.h"

DEFN_CREATABLE_NAME(PDFAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "PDF Attachment"))

PDFAttachment::PDFAttachment() :
    IndexedFileAttachment()
{
    setName( tr("PDF Attachment") );
}

void PDFAttachment::open()
{
#ifdef HAVE_POPPLER
    delete m_document;
    m_document = nullptr;

    if (filename().isEmpty())
    {
        // if filename is empty, dont try to open it.
    }
    else
    {
        m_document = Poppler::Document::load( filename() );
        if (m_document)
        {
            m_document->setRenderHint(Poppler::Document::TextAntialiasing);
        }

    }
#endif
}

void PDFAttachment::paint(AbstractRenderer *pdfCreator)
{
#ifdef HAVE_POPPLER
    QPainter* painter = pdfCreator->currentPage()->painter();
    // ensure that there is the right loaded
    open();
    Poppler::Document* doc = document();
    if (doc)
    {
        Poppler::Document::RenderBackend backendBefore = doc->renderBackend();
        doc->setRenderBackend( Poppler::Document::ArthurBackend );  // the other backend will not work.
        for (int i = 0; i < doc->numPages(); ++i)
        {
            if (i != 0)
            {
                pdfCreator->newPage( Page::NothingSpecial );
            }
            painter->save();
            QSizeF pageSize = doc->page(i)->pageSizeF();
            QSizeF targetSize = pdfCreator->currentPage()->sizePainter();
            double fx = targetSize.width() / pageSize.width();
            double fy = targetSize.height() / pageSize.height();

            double resolution = preference<double>("PDFResolution");
            double f = qMin(fx, fy) * 72.0 / resolution;

            painter->scale( f, f );

            doc->page(i)->renderToPainter( painter, resolution, resolution );
            painter->restore();
        }
        doc->setRenderBackend( backendBefore );
    }
#else
    Q_UNUSED( pdfCreator );
#endif
}
