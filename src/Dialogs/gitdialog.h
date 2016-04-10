#ifndef GITDIALOG_H
#define GITDIALOG_H

#include <QDialog>
#include <QString>

class Project;

namespace Ui {
class GitDialog;
}

class GitHandler;
class git_repository;
class GitDialog : public QDialog
{
    Q_OBJECT

private:
    enum Phase { Clone, Push };
    GitDialog(GitHandler *git, QWidget *parent = 0);
    GitDialog(GitHandler *git, const QString& url, const QString& filename, const QString& masterFilename, Project* masterProject, QWidget *parent = 0);

public:
    ~GitDialog();

    static bool sync(GitHandler* git, const QString& url, const QString& filename, const QString& masterFilename, Project* masterProject, QWidget* parent);
    static bool download(GitHandler *git, QString& url, QString& filename, QString& saveFilename, QWidget *parent);


private slots:
    void download();
    void sync();
    void updateObjectsLabel(uint current, uint total);
    void updateBytesLabel(qint64 bytes);
    void on_openFileDialog_clicked();

    void on_cancelTransferButton_clicked();

private:
    bool clone(git_repository *&repository, const QString& tempDirPath, const QString& username, const QString& password, const QString& url);
    bool push(git_repository* repository);

private:
    Ui::GitDialog *ui;
    QString m_objectsText;
    QString m_bytesText;
    GitHandler* m_git;
    Phase m_phase;
    void info(const QString& message);
    int m_numProgressDots;
    QString progressDots();

    // only required for sync.
    QString m_url;
    QString m_filename;
    QString m_masterFilename;
    Project* m_masterProject;


    bool replaceFile(const QString& victim, const QString& newFile);
};

#endif // GITDIALOG_H
