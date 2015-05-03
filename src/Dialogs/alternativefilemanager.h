#ifndef ALTERNATIVEFILEMANAGER_H
#define ALTERNATIVEFILEMANAGER_H

#include <QDialog>
#include "FileIndex/fileindex.h"
#include "Attachments/indexedfileattachment.h"
#include <QTableWidgetItem>

namespace Ui {
class AlternativeFileManager;
}

class AlternativeFileManager : public QDialog
{
    Q_OBJECT

public:
    explicit AlternativeFileManager( IndexedFileAttachment* attachment, QWidget *parent = 0 );
    ~AlternativeFileManager();

private:
    Ui::AlternativeFileManager *ui;
    IndexedFileAttachment* m_attachment;
    void setupTable();
};

#endif // ALTERNATIVEFILEMANAGER_H
