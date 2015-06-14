#ifndef SETLISTVIEW_H
#define SETLISTVIEW_H

#include <QTableView>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include "Database/EventDatabase/setlist.h"
#include <QMenu>
#include <QItemDelegate>


class SetlistView : public QTableView
{
    Q_OBJECT
public:
    explicit SetlistView(QWidget *parent = 0);
    ~SetlistView();

    Setlist* model() const;
    void setModel(Setlist* setlist);
    void paste(const QMimeData* mime);

    QList<SetlistItem*> selectedItems() const;


signals:
    void clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void showContextMenu(QPoint pos);

    void my_on_actionNewSetlistItem_triggered();
    void my_on_actionDeleteSetlistItem_triggered();
    void my_on_actionCopySetlistItem_triggered();
    void my_on_actionPasteSetlistItem_triggered();
    void updateCellWidgets();

private:
    void setUpContextMenu(QMenu* menu, QPoint pos);
    QAction* m_actionNewSetlistItem;
    QAction* m_actionDeleteSetlistItem;
    QAction* m_actionCopySetlistItem;
    QAction* m_actionPasteSetlistItem;

public:
    static const QString ITEMS_MIMEDATA_FORMAT;


};

#endif // SETLISTVIEW_H
