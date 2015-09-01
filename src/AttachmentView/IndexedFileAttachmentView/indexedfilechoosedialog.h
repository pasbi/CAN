#ifndef INDEXEDFILECHOOSEDIALOG_H
#define INDEXEDFILECHOOSEDIALOG_H

#include <QDialog>
#include <QItemSelection>

namespace Ui {
class IndexedFileChooseDialog;
}

class Song;
class IndexedFileChooseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit IndexedFileChooseDialog( const Song* song, const QByteArray& currentHash, const QStringList& endings, QWidget *parent = 0);
    ~IndexedFileChooseDialog();


    QByteArray hash() const { return m_hash; }

private slots:
    void on_lineEdit_editingFinished();
    void onSelectionChanged(QItemSelection before, QItemSelection deselected);
    void on_pushButtonOpenFileDialog_clicked();
    void clearFilename();

public slots:
    void accept();
    void reject();

private:
    Ui::IndexedFileChooseDialog* ui;
    const Song* const m_song;
    QByteArray m_hash;
    const QStringList m_acceptedEndings;

    QStringList m_filenames;
    QStringList gatherFiles();

    void setFilename(const QString& filename);
    QString currentFilename();


};

#endif // INDEXEDFILECHOOSEDIALOG_H
