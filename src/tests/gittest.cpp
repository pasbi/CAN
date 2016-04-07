#include "gittest.h"
#include "Project/githandler.h"

DEFN_TEST(GitTest)

#define DATETIME_TO_STRING(dateTime) dateTime.toString("dd.mm.yyyy, hh:mm:ss.zzz")

#define PATH_PREFIX "../../../../"
const QString GitTest::FILENAME = "test.can";
const QString GitTest::URL = PATH_PREFIX"testRepo";

void removeMilliseconds(qint64& timestamp)
{
    timestamp = timestamp - (timestamp % 1000);
}

#define GIT_VERIFY(call) { int error = (call); QVERIFY2(error == 0, QString("Errorcode: %1").arg(error).toStdString().c_str()); }


GitTest::GitTest()
{
}

void GitTest::download(GitHandler &git, const QString &url, const QString &path)
{
    qint64 before = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QVERIFY( git.download(url, "test.can", path) );
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

    QVERIFY(QFile(URL).exists());

    git_repository* repository = nullptr;
    GIT_VERIFY( git.clone(repository, URL, tempDirPath) == 0 );

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

    git_repository* repo = nullptr;
    QVERIFY(git.clone(repo, URL, dir.path()));

    modifyFile(QDir(dir.path()).absoluteFilePath(FILENAME));

    QVERIFY(git.commit(repo, FILENAME, "test-commit"));

    QVERIFY(git.push(repo, "username", "password"));

}

