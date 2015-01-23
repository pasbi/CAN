#ifndef CREATABLETEST_H
#define CREATABLETEST_H

#include <QtTest/QTest>

class CreatableTest : public QObject
{
    Q_OBJECT
public:
    explicit CreatableTest(QObject *parent = 0);

private slots:
    // tests go here...

};

#endif // CREATABLETEST_H
