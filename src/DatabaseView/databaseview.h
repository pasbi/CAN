#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QTableView>

class DatabaseView : public QTableView
{
    Q_OBJECT
public:
    explicit DatabaseView(QWidget *parent = 0);

    QModelIndex indexUnderCursor() const;

protected:
    virtual void setUpContextMenu( QMenu* menu ) = 0;

private slots:
    void showContextMenu( QPoint pos );



};

#endif // DATABASEVIEW_H
