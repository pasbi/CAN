#ifndef INDEXVIEW_H
#define INDEXVIEW_H

#include <QTableView>
#include "FileIndex/fileindex.h"

class FileIndexView : public QTableView
{
    Q_OBJECT
public:
    explicit FileIndexView(QWidget *parent = 0);
    FileIndex* model() const;

    QString currentPath() const;


};

#endif // INDEXVIEW_H
