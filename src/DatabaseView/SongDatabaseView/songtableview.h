#ifndef SONGTABLE_H
#define SONGTABLE_H

#include "DatabaseView/databaseview.h"

class Song;
class SongTableView : public DatabaseView<Song>
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

    void setDetailedView(bool detailed);
    void setHideInactives(bool hide);

protected:
    void resizeEvent(QResizeEvent *event);
    void hideEvent(QHideEvent* event);

private slots:
    void updateColumnWidths();
    void restoreColumnWidths();
    void storeColumnWidths();

private:
    bool m_detailsShown;
};


#endif // SONGTABLE_H
