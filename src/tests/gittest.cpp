#include "gittest.h"
#include "Project/githandler.h"

DEFN_TEST(GitTest)

#define DATETIME_TO_STRING(dateTime) dateTime.toString("dd.mm.yyyy, hh:mm:ss.zzz")

#define PATH_PREFIX "../../../../"
const QString GitTest::FILENAME = "test.can";
const QString GitTest::URL = /*
                            PATH_PREFIX"testRepo";
                             /*/
                            "https://github.com/oVooVo/Test.git";
                             //*/

void removeMilliseconds(qint64& timestamp)
{
    timestamp = timestamp - (timestamp % 1000);
}

#define GIT_VERIFY(call) { int error = (call); QVERIFY2(error == 0, QString("Errorcode: %1").arg(error).toStdString().c_str()); }


GitTest::GitTest()
{
}

QString GitTest::password()
{
    // it is very likely for me to forget removing the password before commiting.
    // Hence the password is stored in a file outside the repository. You must create
    // this file manually or write the password into code.

    QFile passwordFile(PATH_PREFIX"passwordfile");
    bool canOpen = passwordFile.open(QIODevice::ReadOnly);
    Q_ASSERT(canOpen);
    QString password = passwordFile.readAll();
    if (password.endsWith("\n"))
    {
        password = password.left(password.length() - 1);
    }
    return password;
}

QString GitTest::username()
{
    return "oVooVo";
}

void GitTest::download(GitHandler &git, const QString &url, const QString &path)
{
    qint64 before = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QVERIFY( git.download(url, "test.can", path, username(), password()) );
    qint64 after = QDateTime::currentDateTime().toMSecsSinceEpoch();

    QFileInfo fileInfo(path);

    QVERIFY(fileInfo.exists());

    qint64 creationTimeStamp = fileInfo.created().toMSecsSinceEpoch();
    // file timestamps have second-precision. So we cannot rely on milliseconds.
    removeMilliseconds(creationTimeStamp);
    removeMilliseconds(before);
    removeMilliseconds(after);
    QVERIFY(before <= creationTimeStamp && creationTimeStamp <= after);
}

void GitTest::clone()
{
    QTemporaryDir tempDir;
    QString tempDirPath = tempDir.path();

    GitHandler git;

    git_repository* repository = nullptr;
    QVERIFY( git.clone(repository, URL, tempDirPath, username(), password()) );

    QFileInfo cloned(tempDirPath);
    QVERIFY(cloned.exists());
    QVERIFY(cloned.isDir());
    QVERIFY(QDir(tempDirPath).entryList(QDir::Dirs | QDir::Hidden).contains(".git"));
    QVERIFY(QDir(tempDirPath).entryList(QDir::Files).contains(FILENAME));

    tempDir.remove();
}

void GitTest::download()
{
    GitHandler git;
    download(git, URL, QDir(PATH_PREFIX).absoluteFilePath(FILENAME));
}

void modifyFile(const QString& filename)
{
    QFile file(filename);
    Q_ASSERT(file.open(QIODevice::ReadWrite | QIODevice::Append));
    file.write( QString(QDateTime::currentDateTime().toString() + "\n").toUtf8() );
    file.close();
}

void GitTest::commitAndPush()
{
    GitHandler git;
    QTemporaryDir dir;

    qDebug() << dir.path();

    git_repository* repo = nullptr;
    QVERIFY(git.clone(repo, URL, dir.path(), username(), password()));

    modifyFile(QDir(dir.path()).absoluteFilePath(FILENAME));

    QVERIFY(git.commit(repo, FILENAME, "testAuthor", "test@email.com", "test-commit"));
    QVERIFY(git.push(repo, username(), password()));

    qDebug() << "pushed!";

}

