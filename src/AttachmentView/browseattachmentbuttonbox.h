#ifndef BROWSEATTACHMENTBUTTONBOX_H
#define BROWSEATTACHMENTBUTTONBOX_H

#include <QWidget>
#include <QMap>

namespace Ui {
class BrowseAttachmentButtonBox;
}

class Song;
class QMenu;
class Attachment;
class QToolButton;
class BrowseAttachmentButtonBox : public QWidget
{
    Q_OBJECT

public:
    explicit BrowseAttachmentButtonBox(QWidget *parent = 0);
    ~BrowseAttachmentButtonBox();

    void setSong(const Song* song);
    void setAttachment(int index);

signals:
    void attachmentSelected(int);

private:
    void clear();
    Ui::BrowseAttachmentButtonBox *ui;

    const Song* m_currentSong;

    const Attachment *defaultAttachment(const QString& type);

    QMap<QString, QMap<const Song*, const Attachment*>> m_defaultAttachment;
    QMap<QString, QList<QAction*>> m_attachmentActions;
    QMap<QAction*, Attachment*> m_attachmentMap;
    QMap<QString, QMenu*> m_menus;
    QMap<QString, QToolButton*> m_toolButtons;

};

#endif // BROWSEATTACHMENTBUTTONBOX_H
