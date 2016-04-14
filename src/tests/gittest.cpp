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


