#include "gitdialog.h"

#include <QMessageBox>
#include <QDateTime>
#include <QTimer>

#include "ui_gitdialog.h"
#include "Project/githandler.h"
#include "global.h"
#include "git2.h"
#include "Merge/mergedialog.h"
#include "global.h"

#define CSTR(qstring) (qstring.toStdString().c_str())
#define EX_ASSERT( expr ) { int code = (expr); Q_ASSERT(code); Q_UNUSED(code); }
#define GIT_ASSERT( expr ) { int code = (expr); Q_ASSERT( code == 0 ); }

QString password()
{
    // it is very likely for me to forget removing the password before commiting.
    // Hence the password is stored in a file outside the repository. You must create
    // this file manually or write the password into code.

    QFile passwordFile("../../../../passwordfile");
    bool canOpen = passwordFile.open(QIODevice::ReadOnly);
    Q_ASSERT(canOpen);
    QString password = passwordFile.readAll();
    if (password.endsWith("\n"))
    {
        password = password.left(password.length() - 1);
    }
    return password;
}


GitDialog::GitDialog(GitHandler *git, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GitDialog),
    m_git(git),
    m_phase(Clone),
    m_numProgressDots(-1)
{
    ui->setupUi(this);

    connect(m_git, SIGNAL(bytesTransfered(qint64)), this, SLOT(updateBytesLabel(qint64)));
    connect(m_git, SIGNAL(objectsTransfered(uint,uint)), this, SLOT(updateObjectsLabel(uint,uint)));
    updateBytesLabel(0);
    updateObjectsLabel(0, 0);
    ui->statusLabel->setText("");

    ui->urlEdit->setText("https://github.com/oVooVo/Test");
    ui->filenameEdit->setText("test.can");
    ui->saveAsEdit->setText("/home/pascal/testfile.can");
    ui->usernameEdit->setText("oVooVo");
    ui->passwordEdit->setText(password());

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(download()));
}

GitDialog::GitDialog(GitHandler *git, const QString& url, const QString& filename, const QString& masterFilename, Project* masterProject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GitDialog),
    m_git(git),
    m_phase(Clone),
    m_numProgressDots(-1),
    m_url(url),
    m_filename(filename),
    m_masterFilename(masterFilename),
    m_masterProject(masterProject)
{
    ui->setupUi(this);

    connect(m_git, SIGNAL(bytesTransfered(qint64)), this, SLOT(updateBytesLabel(qint64)));
    connect(m_git, SIGNAL(objectsTransfered(uint,uint)), this, SLOT(updateObjectsLabel(uint,uint)));
    updateBytesLabel(0);
    updateObjectsLabel(0, 0);
    ui->statusLabel->setText("");
    ui->stackedWidget->setCurrentIndex(1);

    QTimer::singleShot(1000, this, SLOT(sync()));
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

    if (oldBytes != stats->received_bytes)
    {
        emit static_cast<const GitHandler::Payload*>(payload)->git->bytesTransfered(stats->received_bytes);
        oldBytes = stats->received_bytes;
    }

    if (oldCurrent != stats->received_objects || oldTotal != stats->total_objects)
    {
        emit static_cast<const GitHandler::Payload*>(payload)->git->objectsTransfered(stats->received_objects, stats->total_objects);
        oldCurrent = stats->received_objects;
        oldTotal = stats->total_objects;
    }

    return 0;
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

bool GitDialog::clone(git_repository* &repository, const QString& tempDirPath, const QString& username, const QString& password, const QString& url)
{
    git_clone_options options = GIT_CLONE_OPTIONS_INIT;

    options.fetch_opts.callbacks.credentials = credential_cb;
    options.fetch_opts.callbacks.transfer_progress = transferProgress_cb;

    GitHandler::Payload payload(m_git, username, password);
    options.fetch_opts.callbacks.payload = &payload;

    // clone the repo to a temp dir
    qDebug() << "start clone: " << url << "--> " << tempDirPath;
    m_git->startClone(repository, url, tempDirPath, &options);

    m_numProgressDots = -1;
    while (!m_git->isFinished() && !m_git->isAborted())
    {
        qDebug() << "clone loop";
        qApp->processEvents();
        ui->statusLabel->setText(tr("Downloading ") + progressDots());
    }
    m_git->killWorker();

    bool success = true;

    if (m_git->error())
    {
        ui->statusLabel->setText(tr("Cannot download ") + url);
        qDebug() << "clone failed.";
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
    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    callbacks.credentials = credential_cb;
    GitHandler::Payload payload(m_git, "oVooVo", password());
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
    while (!m_git->isFinished() && !m_git->isAborted())
    {
        qApp->processEvents();
        ui->statusLabel->setText(tr("Uploading ") + progressDots());
    }
    m_git->killWorker();


    bool success = true;

    if (m_git->error())
    {
        ui->statusLabel->setText(tr("Cannot upload ") + m_url);
        success = false;
    }

    if (success && m_git->isAborted())
    {
        ui->statusLabel->setText(tr("Aborted by user"));
        success = false;
    }

    git_remote_free(remote);
    remote = nullptr;

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
    ui->stackedWidget->setCurrentIndex(1);



    QTemporaryDir dir;
    git_repository* repository = nullptr;

    if (clone(repository, dir.path(), ui->usernameEdit->text(), ui->passwordEdit->text(), url))
    {
        // copy file to correct place
        QString absoluteSourceFilepath = QDir(dir.path()).absoluteFilePath(filename);

        if (!replaceFile(saveAs, absoluteSourceFilepath))
        {
            ui->statusLabel->setText(tr("Cannot overwrite ") + saveAs);
        }
        else
        {
            ui->statusLabel->setText(tr("Download finished."));
            ui->buttonOk->setEnabled(true);
        }
    }

    git_repository_free(repository);
    repository = nullptr;
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

}

bool GitDialog::download(GitHandler *git, QString& url, QString& filename, QString& saveFilename, QWidget *parent)
{
    GitDialog dialog(git, parent);
    connect(dialog.ui->cancelTransferButton, SIGNAL(clicked()), git, SLOT(abort()));
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
    connect(dialog.ui->cancelTransferButton, SIGNAL(clicked()), git, SLOT(abort()));
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

void GitDialog::on_cancelTransferButton_clicked()
{
    m_git->abort();
    reject();
}

void GitDialog::sync()
{
    QTemporaryDir dir;
    git_repository* repository = nullptr;

    if (clone(repository, dir.path(), "oVooVo", password(), m_url))
    {
        QString slaveFilename = QDir(dir.path()).absoluteFilePath(m_filename);
        Q_ASSERT(QFileInfo(slaveFilename).isReadable());
        if (MergeDialog::performMerge(m_masterProject, slaveFilename, this))
        {
            EX_ASSERT(replaceFile(slaveFilename, m_masterFilename));
            EX_ASSERT( m_git->commit(repository, m_filename, "author", "author@email.com", "Commit-message") );

            m_phase = Push;
            if (push(repository))
            {
                if (!replaceFile(m_masterFilename, slaveFilename))
                {
                    ui->statusLabel->setText(tr("Cannot overwrite ") + m_masterFilename);
                }
                else
                {
                    ui->statusLabel->setText(tr("Download finished."));
                    ui->buttonOk->setEnabled(true);
                }
            }
        }
    }

    git_repository_free(repository);
    repository = nullptr;
}
