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
    explicit ChordPatternViewer(AbstractChordPatternAttachment *attachment, QWidget *parent = 0);
    ~ChordPatternViewer();

public:
    static void displayChordPatternAttachment(AbstractChordPatternAttachment *attachment , QWidget *parent = 0);
    double scrollDownTempo() { return m_speed; }
    int pdfWidth();

public slots:
    void update();


private:
    Ui::ChordPatternViewer *ui;
    AbstractChordPatternAttachment* m_attachment;
    QTemporaryFile m_pdfFile;
    QPixmap m_pixmap;
    double m_zoom;
    double m_speed;
    HUD* m_hud;

    double m_pos;
    bool m_atEnd = false;
    QTimer* m_playTimer;

    void scrollDown();
    void scrollUp();


private slots:
    void on_buttonZoomOut_clicked();
    void on_buttonZoomIn_clicked();
    void applyZoom();
    void applySpeed();


    void on_buttonFaster_clicked();
    void on_buttonSlower_clicked();
    void on_pushButtonPauseJumpToBegin_clicked();
    void on_playTimerTimeout();
    void on_pushButtonPlay_toggled(bool checked);
    void on_buttonEnableLine_clicked(bool checked);


protected:
    void resizeEvent(QResizeEvent *e);
    bool eventFilter(QObject *o, QEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
};



#endif // CHORDPATTERNVIEWER_H
