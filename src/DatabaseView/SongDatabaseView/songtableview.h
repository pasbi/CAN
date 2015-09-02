#ifndef SONGTABLE_H
#define SONGTABLE_H

#include "DatabaseView/databaseview.h"

class Song;
class SongAttributeDelegate;
class SongTableView : public DatabaseView<Song>
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void fakeFocusOutEvent();

private:
    SongAttributeDelegate* m_delegate;
    static Qt::DropAction dropAction( QDropEvent* event );

};

#endif // SONGTABLE_H
