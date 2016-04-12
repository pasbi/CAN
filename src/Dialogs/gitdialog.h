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
    enum Phase { Clone, Push, Finished };
    enum Mode { Download, Sync };
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
    void addCredentials();
    void on_cancelButton_clicked();
    void updateButtonStatus();
    void on_nextButton_clicked();
    void on_backButton_clicked();
    void on_usernameComboBox_currentTextChanged(const QString &username);

    void on_deleteUserButton_clicked();

private:
    bool clone(git_repository *&repository, const QString& tempDirPath, const QString& url);
    bool push(git_repository* repository);

    QString username() const;
    QString password() const;

private:
    Ui::GitDialog *ui;
    QString m_objectsText;
    QString m_bytesText;
    GitHandler* m_git;
    Phase m_phase;
    Mode m_mode;
    void info(const QString& message);
    int m_numProgressDots;
    QString progressDots();

    // only required for sync.
    QString m_url;
    QString m_filename;
    QString m_masterFilename;
    Project* m_masterProject;

    void initUsernameComboBox();
    bool replaceFile(const QString& victim, const QString& newFile);
};

#endif // GITDIALOG_H
