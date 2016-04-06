#include "gittest.h"
#include "Project/githandler.h"

DEFN_TEST(GitTest)

#define GIT_VERIFY(call) { int error = (call); QVERIFY2(error == 0, QString("Errorcode: %1").arg(error).toStdString().c_str()); }

#define PATH_PREFIX "../../../../"

GitTest::GitTest()
{
}

void GitTest::clone()
{
    QVERIFY(QFile(PATH_PREFIX"testRepo").exists());

    GitHandler git;
    QString url = PATH_PREFIX"testRepo";
    QString path = PATH_PREFIX"testClone";

    git_repository* repository = nullptr;
    GIT_VERIFY( git.clone(repository, url, path) == 0 );

    QFileInfo cloned(path);
    QVERIFY(cloned.exists());
    QVERIFY(cloned.isDir());
    QVERIFY(QDir(path).entryList(QDir::Dirs | QDir::Hidden).contains(".git"));
    QVERIFY(QDir(path).entryList(QDir::Files).contains("test.can"));
}

