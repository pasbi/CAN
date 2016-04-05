#include "gittest.h"
#include "Project/githandler.h"

DEFN_TEST(GitTest)

GitTest::GitTest()
{
}

void GitTest::clone()
{
    GitHandler git;
    QString url = "";
    QString path = "";

    git_repository* repository = nullptr;
//FIXME    QVERIFY( git.clone(repository, url, path) );
}

