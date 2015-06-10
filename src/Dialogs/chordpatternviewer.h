#ifndef CHORDPATTERNVIEWER_H
#define CHORDPATTERNVIEWER_H

#include <QDialog>
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include <QTemporaryFile>

namespace Ui {
class ChordPatternViewer;
}

class ChordPatternViewer : public QDialog
{
    Q_OBJECT

private: // you should use the static method `displayChordPatternAttachment()`
    explicit ChordPatternViewer(ChordPatternAttachment *attachment, QWidget *parent = 0);
    ~ChordPatternViewer();
public:
    static void displayChordPatternAttachment(ChordPatternAttachment* attachment , QWidget *parent = 0);
    double scrollDownTempo() { return 0; }

private:
    Ui::ChordPatternViewer *ui;
    ChordPatternAttachment* m_attachment;
    QTemporaryFile m_pdfFile;
};



#endif // CHORDPATTERNVIEWER_H
