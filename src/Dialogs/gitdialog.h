#ifndef GITDIALOG_H
#define GITDIALOG_H

#include <QDialog>

namespace Ui {
class GitDialog;
}

class GitHandler;
class GitDialog : public QDialog
{
    Q_OBJECT

private:
    enum Mode { Download, Sync };
    enum Phase { Clone, Push };
    explicit GitDialog(Mode mode, GitHandler *git, QWidget *parent = 0);

public:
    ~GitDialog();

    static void sync(GitHandler* git, QWidget* parent);
    static bool download(GitHandler* git, QString &filename, QWidget* parent);


private slots:
    void download();
    void updateObjectsLabel(uint current, uint total);
    void updateBytesLabel(qint64 bytes);
    void on_openFileDialog_clicked();

    void on_cancelTransferButton_clicked();

private:
    Ui::GitDialog *ui;
    QString m_objectsText;
    QString m_bytesText;
    GitHandler* m_git;
    Mode m_mode;
    Phase m_phase;
    void info(const QString& message);

    int m_numProgressDots;
    QString progressDots();

    bool replaceFile(const QString& victim, const QString& newFile);
};

#endif // GITDIALOG_H
