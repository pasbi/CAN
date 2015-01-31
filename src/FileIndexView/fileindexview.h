#ifndef INDEXVIEW_H
#define INDEXVIEW_H

#include <QTableView>
#include "FileIndex/fileindex.h"

class FileIndexView : public QTableView
{
    Q_OBJECT
public:
    explicit FileIndexView(QWidget *parent = 0);
    ~FileIndexView();
    FileIndex* model() const;
    void restore();
    void save() const;

    QString currentPath() const;


};

#endif // INDEXVIEW_H
