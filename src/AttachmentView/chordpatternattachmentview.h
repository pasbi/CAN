#ifndef CHORDPATTERNATTACHMENTVIEW_H
#define CHORDPATTERNATTACHMENTVIEW_H

#include "attachmentview.h"
#include <QToolBar>
#include "Attachments/ChordPatternAttachment/chord.h"
#include "configurable.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"


namespace Ui {
class ChordPatternAttachmentView;
}

class ChordPatternAttachmentView : public AttachmentView
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternAttachmentView)
    DECL_CONFIG( ChordPatternAttachmentView );
public:
    explicit ChordPatternAttachmentView(QWidget *parent = 0);
    ~ChordPatternAttachmentView();

    void restoreOptions(const QByteArray &options) { Q_UNUSED(options); }
    QByteArray options() const { return QByteArray(); }

protected:
    void polish();

private:
    Ui::ChordPatternAttachmentView *ui;
    QToolBar* m_toolBar;
    Chord::MinorPolicy m_minorPolicy;
    Chord::EnharmonicPolicy m_enharmonicPolicy;

private slots:
    void updateText();
    void textEdited();

};

#endif // CHORDPATTERNATTACHMENTVIEW_H
