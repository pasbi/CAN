#ifndef GITTEST_H
#define GITTEST_H

#include "testbase.h"

class GitTest : public TestBase
{
    Q_OBJECT
    DECL_TEST(GitTest)
public:
    explicit GitTest();

private slots:
    void clone();

};

#endif // GITTEST_H
