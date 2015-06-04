#ifndef POPPLER_H
#define POPPLER_H

#include <Qt>

#ifdef Q_OS_LINUX
#include "../../poppler/qt5/poppler-qt5.h"
#define HAVE_POPPLER
#elif defined Q_OS_WIN32

#if 0
//#include "../.. /poppler-0.24.5-win32/include/poppler-qt5/poppler-qt5.h"
#define HAVE_POPPLER
#endif
#else
#error unsupported OS
#endif

#endif // POPPLER_H
