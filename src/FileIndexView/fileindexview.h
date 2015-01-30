#ifndef INDEXVIEW_H
#define INDEXVIEW_H

#include <QTreeView>
#include "FileIndex/fileindex.h"

class FileIndexView : public QTreeView
{
    Q_OBJECT
public:
    explicit FileIndexView(QWidget *parent = 0);
    FileIndex* model() const { return static_cast<FileIndex*>( QTreeView::model() ); }

    QString currentPath() const;

};

#endif // INDEXVIEW_H
