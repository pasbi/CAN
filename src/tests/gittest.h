#ifndef GITTEST_H
#define GITTEST_H

#include "testbase.h"

class GitHandler;

class GitTest : public TestBase
{
    Q_OBJECT
    DECL_TEST(GitTest)
public:
    explicit GitTest();

private slots:


    // helpers
private:
    static const QString URL;
    static const QString FILENAME;


    static QString password();
    static QString username();

};

#endif // GITTEST_H
