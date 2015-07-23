#ifndef GIT_H
#define GIT_H

#include <Qt>

#ifdef Q_OS_LINUX
#include <git2.h>
#elif defined Q_OS_WIN32
#include "../../libgit2-0.22.2/include/git2.h"
#else
#error unsupported OS
#endif

#endif // GIT_H
