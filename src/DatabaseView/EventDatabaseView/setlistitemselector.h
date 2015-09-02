#ifndef SETLISTITEMSELECTOR_H
#define SETLISTITEMSELECTOR_H

#include <QDialog>

/**
 * @brief The SetlistItemSelector class is a widget to drag and drop the songs into a setlist.
 */
class SetlistItemSelector : public QDialog
{
    Q_OBJECT

public:
    explicit SetlistItemSelector(QWidget *parent = 0);

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
};

#endif // SETLISTITEMSELECTOR_H
