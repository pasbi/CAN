#ifndef SETLISTVIEW_H
#define SETLISTVIEW_H

#include "DatabaseView/databaseview.h"

class Attachment;
class Setlist;
class SetlistItem;
class Song;
class SetlistView : public DatabaseView<SetlistItem>
{
    Q_OBJECT
public:
    explicit SetlistView(QWidget *parent = 0);
    ~SetlistView();

    Setlist* model() const;
    void setModel(Setlist* setlist);

    QList<SetlistItem*> selectedItems() const;

public slots:
    void select(QModelIndexList indexes);
    void setFilterTag( const QString& tag );


signals:
    void mousePress();

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void showContextMenu(QPoint pos);
    void updateCellWidgets();

private:
    void setUpContextMenu(QMenu* menu, QPoint pos);

    QString m_filterTag;

    bool attachmentIsIgnored( const Attachment* attachment ) const;
    QWidget* createSongCellWidget(const Song *song);

};

#endif // SETLISTVIEW_H
