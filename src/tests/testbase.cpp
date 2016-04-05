#include "testbase.h"

QList<TestBase*(*)(void)> TestBase::m_tests;

TestBase::TestBase()
{
}

TestBase::~TestBase()
{
}

void TestBase::run()
{
    for (TestBase*(*f)(void) : m_tests)
    {
        TestBase* test = f();
        QTest::qExec(test);
        delete test;
    }
}
