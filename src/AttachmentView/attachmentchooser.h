#ifndef ATTACHMENTCHOOSER_H
#define ATTACHMENTCHOOSER_H

#include <QWidget>
#include <QMap>

namespace Ui {
class AttachmentChooser;
}

class Song;
class Attachment;
class AttachmentHistory;
class AttachmentChooser : public QWidget
{
    Q_OBJECT

public:
    explicit AttachmentChooser(QWidget *parent = 0);
    ~AttachmentChooser();
    int currentAttachmentIndex() const;
    Attachment* currentAttachment() const;

public slots:
    void setSong( Song* song );
    Song* song() const { return m_currentSong; }
    void setAttachment(int index , bool affectHistory = true);
    void updateAttachmentView();
    void renameCurrentAttachment();

private slots:
    void editTags();
    void focusAttachment( const Attachment* a );
    void on_buttonDelete_clicked();

    void on_buttonForward_clicked();
    void on_buttonBackward_clicked();
    void updateHistoryButtons();

private:
    Ui::AttachmentChooser *ui;
    Song* m_currentSong;
    Attachment* m_currentAttachment = nullptr;
    QMap<Song*, int> m_lastOpenedIndex;
    QAction* m_editTagAction;
    AttachmentHistory* m_attachmentHistory;

protected:
    bool eventFilter(QObject *o, QEvent *e);



};

#endif // ATTACHMENTCHOOSER_H
