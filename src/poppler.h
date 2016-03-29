#ifndef POPPLER_H
#define POPPLER_H

#include <Qt>

#if 0

#if defined Q_OS_ANDROID
    #include "poppler-qt5.h"
#elif defined Q_OS_LINUX
    #include "poppler-qt5.h"
    #define HAVE_POPPLER
#elif defined Q_OS_WIN32
    #include "../../poppler-0.24.5-win32/include/poppler-qt5/poppler-qt5.h"
    #define HAVE_POPPLER
#else
    #error unsupported OS
#endif

#endif

#endif // POPPLER_H
