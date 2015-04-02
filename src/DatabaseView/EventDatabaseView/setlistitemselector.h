#ifndef SETLISTITEMSELECTOR_H
#define SETLISTITEMSELECTOR_H

#include <QDialog>
#include "Database/SongDatabase/songdatabase.h"


class SetlistItemSelector : public QDialog
{
    Q_OBJECT

public:
    explicit SetlistItemSelector(QWidget *parent = 0);
    ~SetlistItemSelector();

};

#endif // SETLISTITEMSELECTOR_H
