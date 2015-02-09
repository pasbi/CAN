#ifndef CHORDPATTERNATTACHMENTVIEW_H
#define CHORDPATTERNATTACHMENTVIEW_H

#include "attachmentview.h"
#include <QToolBar>
#include "Attachments/ChordPatternAttachment/chord.h"

namespace Ui {
class ChordPatternAttachmentView;
}

class ChordPatternAttachmentView : public AttachmentView, public Creatable
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternAttachmentView);
public:
    explicit ChordPatternAttachmentView(QWidget *parent = 0);
    ~ChordPatternAttachmentView();

protected:
    void connectWithAttachment() {}

private:
    Ui::ChordPatternAttachmentView *ui;
    QToolBar* m_toolBar;
    Chord::MinorPolicy m_minorPolicy;
    Chord::EnharmonicPolicy m_enharmonicPolicy;

private slots:
    void updateText();
};

#endif // CHORDPATTERNATTACHMENTVIEW_H
