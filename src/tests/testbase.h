#ifndef TESTBASE_H
#define TESTBASE_H

#include <QList>
#include <QObject>

#include <QtTest/QTest>
#include <QtTest/QtTest>

class TestBase;
template<class T> TestBase* createTest()
{
    return new T();
}

class TestBase : public QObject
{
public:
    explicit TestBase();
    virtual ~TestBase();

    static void run();

protected:
    template<class T> static void registerTest()
    {
        m_tests.append(&createTest<T>);
    }

private:
    static QList<TestBase*(*)()> m_tests;

    template<class T> friend struct RegisterTest;

};



template<class T> struct RegisterTest
{
    RegisterTest()
    {
        qDebug() << "register test";
        TestBase::registerTest<T>();
    }
};

#define DECL_TEST(CLASS) static RegisterTest<CLASS> reg;
#define DEFN_TEST(CLASS) RegisterTest<CLASS> CLASS::reg;


#endif // TESTBASE_H
