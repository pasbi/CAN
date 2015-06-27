#ifndef CHORDPATTERNATTACHMENTVIEW_H
#define CHORDPATTERNATTACHMENTVIEW_H

#include "attachmentview.h"
#include <QToolBar>
#include "Attachments/ChordPatternAttachment/chord.h"
#include "configurable.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "chordpatternedit.h"


namespace Ui {
class ChordPatternAttachmentView;
}

class ChordPatternAttachmentView : public AttachmentView
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternAttachmentView)
    DECL_CONFIG( ChordPatternAttachmentView )
public:
    explicit ChordPatternAttachmentView(QWidget *parent = 0);
    ~ChordPatternAttachmentView();

    void restoreOptions(const QByteArray &options) { Q_UNUSED(options); }
    QByteArray options() const { return QByteArray(); }

    static void highlightTextEdit(ChordPatternEdit *edit , const QString &text);

protected:
    void polish();

private:
    Ui::ChordPatternAttachmentView *ui;
    QToolBar* m_toolBar;

private slots:
    void updateText();
    void textEdited();

    void on_buttonUp_clicked();
    void on_buttonDown_clicked();
    void on_buttonView_clicked();
};

#endif // CHORDPATTERNATTACHMENTVIEW_H
