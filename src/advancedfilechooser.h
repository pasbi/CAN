#ifndef ADVANCEDFILECHOOSER_H
#define ADVANCEDFILECHOOSER_H

#include <QWidget>
#include "FileIndex/fileindex.h"

namespace Ui {
class AdvancedFileChooser;
}

class AdvancedFileChooser : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedFileChooser(QWidget *parent = 0);
    ~AdvancedFileChooser();

    void setFilter(const QStringList & filter);
    void setIndex( const FileIndex* fileIndex );

public slots:
    void setHash( const QByteArray & hash );

private:
    Ui::AdvancedFileChooser *ui;
    QStringList m_filter;
    const FileIndex* m_fileIndex;
    QStringList m_filenames;
    void updateComboBox();
    QByteArray m_hash;


};

#endif // ADVANCEDFILECHOOSER_H
