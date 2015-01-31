#ifndef CHORDPATTERNATTACHMENTVIEW_H
#define CHORDPATTERNATTACHMENTVIEW_H

#include "attachmentview.h"

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
};

#endif // CHORDPATTERNATTACHMENTVIEW_H
