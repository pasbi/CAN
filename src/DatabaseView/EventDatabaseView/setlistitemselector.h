#ifndef SETLISTITEMSELECTOR_H
#define SETLISTITEMSELECTOR_H

#include <QDialog>
#include "Database/SongDatabase/songdatabase.h"

/**
 * @brief The SetlistItemSelector class is a widget to drag and drop the songs into a setlist.
 */
class SetlistItemSelector : public QDialog
{
    Q_OBJECT

public:
    explicit SetlistItemSelector(QWidget *parent = 0);
    ~SetlistItemSelector();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
};

#endif // SETLISTITEMSELECTOR_H
