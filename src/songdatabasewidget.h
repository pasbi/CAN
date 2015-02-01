#ifndef SONGDATABASEWIDGET_H
#define SONGDATABASEWIDGET_H

#include <QWidget>
#include "Database/SongDatabase/songdatabase.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"


namespace Ui {
class SongDatabaseWidget;
}

class SongDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongDatabaseWidget(QWidget *parent = 0);
    ~SongDatabaseWidget();
    void setSongDatabase( SongDatabase * songDatabase );

private:
    Ui::SongDatabaseWidget *ui;
    SongDatabase* m_songDatabase;
    SongDatabaseSortProxy m_sortFilterProxy;
};

#endif // SONGDATABASEWIDGET_H
