#ifndef FILEINDEXVIEWCONTAINER_H
#define FILEINDEXVIEWCONTAINER_H

#include <QWidget>
#include "FileIndex/fileindex.h"

namespace Ui {
class FileIndexViewContainer;
}

class FileIndexViewContainer : public QWidget
{
    Q_OBJECT

public:
    explicit FileIndexViewContainer(QWidget *parent = 0);
    ~FileIndexViewContainer();
    void setModel( FileIndex* model );

private:
    Ui::FileIndexViewContainer *ui;

private slots:
    void addFiles();
    void addFolder();
    void remove();

};

#endif // FILEINDEXVIEWCONTAINER_H
