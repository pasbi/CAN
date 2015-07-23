#ifndef GITUTIL_H
#define GITUTIL_H

#include "global.h"


#define CSTR( qstring ) ((qstring).toStdString().c_str())

bool check_git_call( int errorcode, int line, const char* file);

#define CHECK_GIT( call ) \
    check_git_call( call, __LINE__, __FILE__ )

#define CHECK_IS_REPOSITORY() \
    if (!isGitRepository())                         \
    {                                               \
        qWarning() << "File "                       \
                   << QString("%1:%2").arg(__FILE__).arg(__LINE__) \
                   << "No Git repository.";         \
        return false;                               \
    }


#endif // GITUTIL_H
