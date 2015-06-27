#ifndef CHORDPATTERNPROXYATTACHMENTVIEW_H
#define CHORDPATTERNPROXYATTACHMENTVIEW_H

#include "attachmentview.h"

#include <QWidget>

namespace Ui {
class ChordPatternProxyAttachmentView;
}

class ChordPatternAttachment;
class ChordPatternProxyAttachmentView : public AttachmentView
{
    Q_OBJECT
    DECL_CREATABLE( ChordPatternProxyAttachmentView )
public:
    explicit ChordPatternProxyAttachmentView(QWidget *parent = 0);
    ~ChordPatternProxyAttachmentView();

    void polish();

private slots:
    void on_buttonUp_clicked();
    void on_buttonDown_clicked();
    void on_buttonView_clicked();
    void updateText();


    void on_buttonOriginal_clicked();

private:
    Ui::ChordPatternProxyAttachmentView *ui;
};

#endif // CHORDPATTERNPROXYATTACHMENTVIEW_H
