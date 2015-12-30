#ifndef SETLISTVIEW_H
#define SETLISTVIEW_H

#include "DatabaseView/databaseview.h"

class Attachment;
template<typename T> class Database;
class SetlistItem;
class Song;
class SetlistView : public DatabaseView<SetlistItem>
{
    Q_OBJECT
public:
    explicit SetlistView(QWidget *parent = 0);

    void setModel(Database<SetlistItem>* setlist);

    QList<SetlistItem*> selectedItems() const;

public slots:
    void select(QModelIndexList indexes);


private slots:
    void updateCellWidgets();

private:
    bool attachmentIsIgnored( const Attachment* attachment ) const;
    QWidget* createSongCellWidget(const Song *song);

protected:
    QAbstractItemDelegate* itemDelegate(const QModelIndex &index) const;

};

#endif // SETLISTVIEW_H
