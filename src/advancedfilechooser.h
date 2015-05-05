#ifndef ADVANCEDFILECHOOSER_H
#define ADVANCEDFILECHOOSER_H

#include <QWidget>
#include "FileIndex/fileindex.h"
#include "Database/SongDatabase/song.h"
#include "configurable.h"


namespace Ui {
class AdvancedFileChooser;
}

class AdvancedFileChooser : public QWidget
{
    Q_OBJECT
    DECL_CONFIG( AdvancedFileChooser )

public:
    explicit AdvancedFileChooser(QWidget *parent = 0);
    ~AdvancedFileChooser();

    void setFilterProperties( const Song* song, const QStringList & endings );
    void updateComboBox();

public slots:
    void setHash( const QByteArray & hash );

private:
    Ui::AdvancedFileChooser *ui;
    QStringList m_endings;
    const Song* m_song;

    QStringList m_filenames;
    QByteArray m_hash;


signals:
    void itemSelected( QByteArray hash);


private slots:
    void on_pushButton_clicked();
};

#endif // ADVANCEDFILECHOOSER_H
