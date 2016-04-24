#include "gitdialog.h"

#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QFileDialog>

#include "application.h"
#include "ui_gitdialog.h"
#include "Project/githandler.h"
#include "global.h"
#include "git2.h"
#include "Merge/mergedialog.h"
#include "global.h"

#define CSTR(qstring) (qstring.toStdString().c_str())
#define EX_ASSERT( expr ) { int code = (expr); Q_ASSERT(code); Q_UNUSED(code); }
#define GIT_ASSERT( expr ) { int code = (expr); Q_ASSERT( code == 0 ); Q_UNUSED(code); }

GitDialog::GitDialog(GitHandler *git, Mode mode, const QString& url, const QString& filename, const QString& masterFilename, Project* masterProject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GitDialog),
    m_git(git),
    m_phase(Clone),
    m_mode(mode),
    m_numProgressDots(-1),
    m_url(url),
    m_filename(filename),
    m_masterFilename(masterFilename),
    m_masterProject(masterProject)
{
    ui->setupUi(this);
    connect(m_git, SIGNAL(bytesTransfered(qint64)), this, SLOT(updateBytesLabel(qint64)));
    connect(m_git, SIGNAL(objectsTransfered(uint,uint)), this, SLOT(updateObjectsLabel(uint,uint)));
    connect(this, &GitDialog::gitError, [this](int klass, QString message)
    {
        ui->errorLabel->setText(prettifyGitError(klass, message));
    });
    updateBytesLabel(0);
    updateObjectsLabel(0, 0);

    ui->statusLabel->setText("");
    ui->errorLabel->setText("");
}

GitDialog::GitDialog(GitHandler *git, QWidget *parent) :
    GitDialog(git, Download, "", "", "", nullptr, parent)
{
    setWindowTitle(tr("Download"));
    ui->stackedWidget->setCurrentIndex(0);
    ui->usernameComboBox->lineEdit()->setPlaceholderText(tr("Username"));

    initUsernameComboBox();
    updateButtonStatus();

    ui->commitMessageTextEdit->hide();
    ui->messageLabel->hide();
}

GitDialog::GitDialog(GitHandler *git, const QString& url, const QString& filename, const QString& masterFilename, Project* masterProject, QWidget *parent) :
    GitDialog(git, Sync, url, filename, masterFilename, masterProject, parent )
{
    setWindowTitle(tr("Synchronize"));
    ui->stackedWidget->setCurrentIndex(1);

    initUsernameComboBox();
    updateButtonStatus();
}



GitDialog::~GitDialog()
{
    delete ui;
}

void GitDialog::info(const QString &message)
{
    QMessageBox::information(this, qAppName(), message, QMessageBox::Ok, QMessageBox::Ok);
}

QString GitDialog::progressDots()
{
    static const qint64 interval = 200;
    static const int maxDots = 3;
    static qint64 lastMillis = 0;
    static QString dots;
    qint64 millis = QDateTime::currentMSecsSinceEpoch();
    if (m_numProgressDots < 0)
    {
        lastMillis = millis;
        m_numProgressDots = 0;
        dots = "";
    }
    else if (millis - lastMillis >= interval)
    {
        lastMillis = millis;
        if (m_numProgressDots == maxDots)
        {
            dots = "";
            m_numProgressDots = 0;
        }
        else
        {
            dots += ".";
            m_numProgressDots++;
        }
    }

    return dots;
}

int credential_cb(git_cred **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{
    Q_UNUSED( url );
    Q_UNUSED( username_from_url );
    Q_UNUSED( allowed_types );
    Q_UNUSED( payload );

    QString username = static_cast<const GitHandler::Payload*>(payload)->username;
    QString password = static_cast<const GitHandler::Payload*>(payload)->password;

    GIT_ASSERT( git_cred_userpass_plaintext_new( out, CSTR(username), CSTR(password) ) );
    return 0;
}

int transferProgress_cb(const git_transfer_progress *stats, void *payload)
{
    static unsigned int oldCurrent = 0;
    static unsigned int oldTotal = 0;
    static size_t oldBytes = 0;

    const GitHandler::Payload* payload_ = static_cast<const GitHandler::Payload*>(payload);

    if (oldBytes != stats->received_bytes)
    {
        emit payload_->git->bytesTransfered(stats->received_bytes);
        oldBytes = stats->received_bytes;
    }

    if (oldCurrent != stats->received_objects || oldTotal != stats->total_objects)
    {
        emit payload_->git->objectsTransfered(stats->received_objects, stats->total_objects);
        oldCurrent = stats->received_objects;
        oldTotal = stats->total_objects;
    }

    if (payload_->abortCloneRequested)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int pushTransferProgress_cb(unsigned int current, unsigned int total, size_t bytes, void* payload)
{
    static unsigned int oldCurrent = 0;
    static unsigned int oldTotal = 0;
    static size_t oldBytes = 0;

    if (oldBytes != bytes)
    {
        emit static_cast<const GitHandler::Payload*>(payload)->git->bytesTransfered(bytes);
        oldBytes = bytes;
    }

    if (oldCurrent != current || oldTotal != total)
    {
        emit static_cast<const GitHandler::Payload*>(payload)->git->objectsTransfered(current, total);
        oldCurrent = current;
        oldTotal = total;
    }

    return 0;
}

void display(const git_error* error)
{
    if (error)
    {
        qDebug() << "Git Error: " << error->klass << ": " << error->message;
    }
}

bool GitDialog::clone(git_repository* &repository, const QString& tempDirPath, const QString& url)
{
    git_clone_options options;
    git_clone_init_options(&options, GIT_CLONE_OPTIONS_VERSION);

    GitHandler::Payload payload(m_git, username(), password());
    options.fetch_opts.callbacks.payload = &payload;
    options.fetch_opts.callbacks.credentials = &credential_cb;
    options.fetch_opts.callbacks.transfer_progress = &transferProgress_cb;

    // clone the repo to a temp dir
    m_git->startClone(repository, url, tempDirPath, &options);

    m_numProgressDots = -1;
    while (!m_git->isFinished())
    {
        if (m_git->isAborted())
        {
            payload.abortCloneRequested = true;
        }
        ui->statusLabel->setText(tr("Downloading ") + progressDots());
        lookForErrors();
        qApp->processEvents();
    }
    m_git->killWorker();

    bool success = true;

    if (m_git->error())
    {
        ui->statusLabel->setText(tr("Cannot download %1").arg(url));
        success = false;
    }

    if (success && m_git->isAborted())
    {
        ui->statusLabel->setText(tr("Aborted by user"));
        success = false;
    }
    else if (success)
    {
        Q_ASSERT(repository);
    }

    return success;
}

bool GitDialog::push(git_repository* repository)
{
    //setup callbacks
    git_remote_callbacks callbacks;
    git_remote_init_callbacks(&callbacks, GIT_REMOTE_CALLBACKS_VERSION);
    callbacks.credentials = credential_cb;
    GitHandler::Payload payload(m_git, username(), password());
    callbacks.payload = &payload;
    callbacks.push_transfer_progress = &pushTransferProgress_cb;

    // setup options
    git_push_options options;
    GIT_ASSERT( git_push_init_options( &options, GIT_PUSH_OPTIONS_VERSION ) );
    options.callbacks = callbacks;

    // setup refspecs
    git_strarray refspecs;
    refspecs.count = 1;
    QString qrefspec("refs/heads/master:refs/heads/master");
    char* refspec = new char[qrefspec.length()];
    strcpy(refspec, qrefspec.toStdString().c_str());
    refspecs.strings = &refspec;

    git_remote* remote = nullptr;
    m_git->startPush(repository, remote, &refspecs, &options);

    m_numProgressDots = -1;
    while (!m_git->isFinished())
    {
        if (m_git->isAborted())
        {
            m_git->abortPush(remote);
        }
        updateBytesLabel(0);
        updateObjectsLabel(0, 0);
        ui->statusLabel->setText(tr("Uploading ") + progressDots());
        lookForErrors();
        qApp->processEvents();
    }
    m_git->killWorker();


    bool success = true;

    if (m_git->error())
    {
        ui->statusLabel->setText(tr("Cannot upload %1").arg(m_url));
        success = false;
    }

    if (success && m_git->isAborted())
    {
        ui->statusLabel->setText(tr("Aborted by user"));
        success = false;
    }

    git_remote_free(remote);

    return success;
}

void GitDialog::download()
{
    QString url = ui->urlEdit->text();
    if (url.isEmpty())
    {
        info(tr("URL must not be empty."));
        return;
    }

    QString filename = ui->filenameEdit->text();
    if (filename.isEmpty())
    {
        info(tr("Filename must not be empty."));
        return;
    }

    QString saveAs = ui->saveAsEdit->text();
    if (saveAs.isEmpty())
    {
        info(tr("Please provide a filename for saving."));
        return;
    }

    ui->statusLabel->setText(tr("Start download ..."));
    ui->stackedWidget->setCurrentIndex(2);



    QTemporaryDir dir;
    git_repository* repository = nullptr;

    if (clone(repository, dir.path(), url))
    {
        // copy file to correct place
        QString absoluteSourceFilepath = QDir(dir.path()).absoluteFilePath(filename);

        if (!QFileInfo(absoluteSourceFilepath).exists())
        {
            ui->statusLabel->setText(tr("Cannot find %1 in repository.").arg(filename));
        }
        else if (!replaceFile(saveAs, absoluteSourceFilepath))
        {
            ui->statusLabel->setText(tr("Cannot overwrite %1").arg(saveAs));
        }
        else
        {
            ui->statusLabel->setText(tr("Download finished."));
            m_phase = Finished;
        }
    }

    git_repository_free(repository);
    repository = nullptr;
    updateButtonStatus();
}

void GitDialog::updateBytesLabel(qint64 bytes)
{
    QString text;
    if (m_phase == Clone)
    {
        text = tr("Received bytes: %1");
    }
    else
    {
        text = tr("Sent bytes: %1");
    }
    ui->transferedBytes->setText(text.arg(bytes));
}

void GitDialog::updateObjectsLabel(uint current, uint total)
{
    QString text;
    if (m_phase == Clone)
    {
        text = tr("Received objects: %1/%2");
    }
    else
    {
        text = tr("Sent objects: %1/%2");
    }
    ui->transferedObjects->setText(text.arg(current).arg(total));
}


void GitDialog::on_openFileDialog_clicked()
{
    QString filename = ui->saveAsEdit->text();
    if (filename.isEmpty())
    {
        filename = QDir::homePath();
    }

    filename = QFileDialog::getSaveFileName(this, qAppName(), filename, Application::PROJECT_FILE_FILTER);
    if (filename.isEmpty())
    {
        return;
    }

    ui->saveAsEdit->setText(filename);
}

bool GitDialog::download(GitHandler *git, QString& url, QString& filename, QString& saveFilename, QWidget *parent)
{
    GitDialog dialog(git, parent);
    if (dialog.exec() == QDialog::Accepted)
    {
        saveFilename = dialog.ui->saveAsEdit->text();
        url = dialog.ui->urlEdit->text();
        filename = dialog.ui->filenameEdit->text();
        return !git->error();
    }
    else
    {
        saveFilename = "";
        url = "";
        filename = "";
        return false;
    }
}

bool GitDialog::sync(GitHandler *git, const QString& url, const QString& filename, const QString &masterFilename, Project *masterProject, QWidget *parent)
{
    GitDialog dialog(git, url, filename, masterFilename, masterProject, parent);
    if (dialog.exec() == QDialog::Accepted)
    {
        return !git->error();
    }
    else
    {
        return false;
    }
}

bool GitDialog::replaceFile(const QString& victim, const QString& newFile)
{
    // try to delete victim file if it exists.
    if (QFileInfo(victim).exists())
    {
        if (!QFile(victim).remove())
        {
            qWarning() << "cannot overwrite " << victim;
            return false;
        }
    }

    // check if newFile exists
    QFileInfo newFileInfo(newFile);
    if (!newFileInfo.exists() || !newFileInfo.isFile())
    {
        qWarning() << newFile << "is not a file.";
        return false;
    }

    if (!QFile::copy(newFile, victim))
    {
        qWarning() << "cannot copy" << newFile << "->" << victim;
        return false;
    }

    return true;
}

void GitDialog::on_cancelButton_clicked()
{
    m_git->requestAbort();
    reject();
}

void GitDialog::sync()
{
    QTemporaryDir dir;
    git_repository* repository = nullptr;

    if (clone(repository, dir.path(), m_url))
    {
        QString slaveFilename = QDir(dir.path()).absoluteFilePath(m_filename);
        Q_ASSERT(QFileInfo(slaveFilename).isReadable());
        if (MergeDialog::performMerge(m_masterProject, slaveFilename, this))
        {
            EX_ASSERT(replaceFile(slaveFilename, m_masterFilename));
            EX_ASSERT( m_git->commit(repository, m_filename, username(), userEmail(), commitMessage()) );

            m_phase = Push;
            if (push(repository))
            {
                if (!replaceFile(m_masterFilename, slaveFilename))
                {
                    ui->statusLabel->setText(tr("Cannot overwrite %1").arg(m_masterFilename));
                }
                else
                {
                    ui->statusLabel->setText(tr("Sync finished."));
                    m_phase = Finished;
                }
            }
        }
    }

    git_repository_free(repository);
    repository = nullptr;
    updateButtonStatus();
}

void GitDialog::initUsernameComboBox()
{
    QMap<QString, QString> creds = preference<QMap<QString, QString>>("credentials");
    for (const QString& username : creds.keys())
    {
        ui->usernameComboBox->addItem(username);
    }

    connect(ui->usernameComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), [creds, this](QString username)
    {
        if (creds.keys().contains(username))
        {
            ui->passwordEdit->setText(creds[username]);
            setPreference("lastUsername", username);
        }
    });
    connect(ui->usernameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonStatus()));
}

void GitDialog::addCredentials()
{
    QMap<QString, QString> creds = preference<QMap<QString, QString>>("credentials");
    QString password = ui->passwordEdit->text();
    QString username = ui->usernameComboBox->currentText();
    if (password.isEmpty())
    {
        creds.remove(username);
    }
    else
    {
        creds.insert(username, password);
    }
    setPreference("credentials", creds);
}

void GitDialog::updateButtonStatus()
{
    ui->cancelButton->setText(tr("&Cancel"));
    ui->backButton->setText(tr("&Back"));
    int page = ui->stackedWidget->currentIndex();
    switch (m_mode)
    {
    case Sync:
        ui->backButton->setEnabled(false);
        ui->nextButton->setEnabled(page == 1 || m_phase == Finished);
        if (page == 0)
        {
            ui->nextButton->setText(tr("&Next"));
        }
        else if (page == 1)
        {
            ui->nextButton->setText(tr("&Synchronize"));
        }
        else
        {
            ui->nextButton->setText("&Ok");
        }
        break;
    case Download:
        ui->backButton->setEnabled(page == 1);
        ui->nextButton->setEnabled(page == 0 || page == 1 || m_phase == Finished);
        if (page == 0)
        {
            ui->nextButton->setText(tr("&Next"));
        }
        else if (page == 1)
        {
            ui->nextButton->setText(tr("&Download"));
        }
        else
        {
            ui->nextButton->setText("&Ok");
        }
        break;
    }


    QMap<QString, QString> creds = preference<QMap<QString, QString>>("credentials");
    QString username = ui->usernameComboBox->currentText();
    ui->deleteUserButton->setEnabled(creds.contains(username));
}

void GitDialog::on_nextButton_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(index + 1);
    updateButtonStatus();

    if (index == 1) // old index!
    {
        if (ui->rememberCheckbox->isChecked())
        {
            addCredentials();
        }
        switch (m_mode)
        {
        case Download:
            download();
            break;
        case Sync:
            sync();
            break;
        }

    }
    else if (index == 2) // old index!
    {
        accept();
    }

}

void GitDialog::on_backButton_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    Q_ASSERT(index > 0);
    ui->stackedWidget->setCurrentIndex( index - 1 );
    updateButtonStatus();
}

void GitDialog::on_usernameComboBox_currentTextChanged(const QString &username)
{
    QMap<QString, QString> creds = preference<QMap<QString, QString>>("credentials");

    if (creds.keys().contains(username))
    {
        ui->passwordEdit->setText(creds[username]);
        ui->rememberCheckbox->setChecked(true);
    }
    else
    {
        ui->passwordEdit->setText("");
        ui->rememberCheckbox->setChecked(false);
    }
}

void GitDialog::on_deleteUserButton_clicked()
{
    QMap<QString, QString> creds = preference<QMap<QString, QString>>("credentials");
    QString username = ui->usernameComboBox->currentText();
    creds.remove(username);
    setPreference("credentials", creds);

    ui->usernameComboBox->setCurrentText("");
    ui->passwordEdit->setText("");
}

QString GitDialog::password() const
{
    return ui->passwordEdit->text();
}

QString GitDialog::username() const
{
    return ui->usernameComboBox->currentText();
}

QString GitDialog::commitMessage() const
{
    return ui->commitMessageTextEdit->toPlainText();
}

QString GitDialog::userEmail() const
{
    return "dummy@email.com";
}

void GitDialog::lookForErrors()
{
    const git_error* error = giterr_last();
    if (error)
    {
        qWarning() << "git error: " << error->message << ", " << error->message;
        emit gitError(error->klass, QString(error->message));
    }
    giterr_clear();
}

QString GitDialog::prettifyGitError(int klass, const QString &message)
{
	// actually I dont expect to ever see this function to act. So the following stumb is just fine.
    return QString("%1, %2").arg(klass).arg(message);
}
