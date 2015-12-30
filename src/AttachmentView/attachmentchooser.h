#ifndef ATTACHMENTCHOOSER_H
#define ATTACHMENTCHOOSER_H

#include <QWidget>
#include <QMap>

namespace Ui {
class AttachmentChooser;
}

class Song;
class Attachment;
class AttachmentView;
class AttachmentChooser : public QWidget
{
    Q_OBJECT

public:
    explicit AttachmentChooser(QWidget *parent = 0);
    ~AttachmentChooser();
    int currentAttachmentIndex() const;
    Attachment* currentAttachment() const;
    AttachmentView* currentAttachmentView() const;

public slots:
    void setSong( Song* song );
    Song* song() const { return m_currentSong; }
    void setAttachment(int index);
    void updateAttachmentView();
    void renameCurrentAttachment();

private slots:
    void focusAttachment( const Attachment* a );
    void on_buttonDelete_clicked();

private:
    Ui::AttachmentChooser *ui;
    Song* m_currentSong;
    Attachment* m_currentAttachment = nullptr;
    QMap<Song*, int> m_lastOpenedIndex;

protected:
    bool eventFilter(QObject *o, QEvent *e);



};

#endif // ATTACHMENTCHOOSER_H
