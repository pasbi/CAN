#ifndef CHORDPATTERNVIEWER_H
#define CHORDPATTERNVIEWER_H

#include <QDialog>
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include <QTemporaryFile>
#include "poppler.h"
#include "configurable.h"
#include "hud.h"

namespace Ui {
class ChordPatternViewer;
}

class ChordPatternViewer : public QDialog
{
    Q_OBJECT
    DECL_CONFIG( ChordPatternViewer )

private: // you should use the static method `displayChordPatternAttachment()`
    explicit ChordPatternViewer(ChordPatternAttachment *attachment, QWidget *parent = 0);
    ~ChordPatternViewer();

public:
    static void displayChordPatternAttachment(ChordPatternAttachment* attachment , QWidget *parent = 0);
    double scrollDownTempo() { return 0; }
    int pdfWidth();


private:
    Ui::ChordPatternViewer *ui;
    ChordPatternAttachment* m_attachment;
    QTemporaryFile m_pdfFile;
    QPixmap m_pixmap;
    double m_zoom;
    double m_speed;
    HUD* m_hud;


private slots:
    void on_buttonZoomOut_clicked();
    void on_buttonZoomIn_clicked();
    void applyZoom();
    void applySpeed();


    void on_buttonFaster_clicked();
    void on_buttonSlower_clicked();

protected:
    void resizeEvent(QResizeEvent *e);
};



#endif // CHORDPATTERNVIEWER_H
