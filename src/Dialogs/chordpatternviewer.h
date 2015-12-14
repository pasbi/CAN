#ifndef CHORDPATTERNVIEWER_H
#define CHORDPATTERNVIEWER_H

#include <QDialog>
#include <QTemporaryFile>
#include "configurable.h"

namespace Ui {
class ChordPatternViewer;
}

class AbstractChordPatternAttachment;
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

    double m_pos;
    bool m_atEnd = false;
    QTimer* m_playTimer;

    void scrollDown();
    void scrollUp();
    void initializeAudioPlayerWidget();



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
    void on_buttonAutoZoom_clicked();
    void on_buttonColumnCount_toggled(bool checked);


protected:
    void resizeEvent(QResizeEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
};



#endif // CHORDPATTERNVIEWER_H
