#include "unittests.h"

#include "creatabletest.h"

#define execTest(TESTNAME) \
{ \
    TESTNAME t; \
    QTest::qExec(&t); \
}

namespace UnitTests {

void runTests()
{
    execTest(CreatableTest);
}

}

