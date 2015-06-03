#ifndef POPPLER_H
#define POPPLER_H

#include <Qt>

#ifdef Q_OS_LINUX
#include "../../poppler/qt5/poppler-qt5.h"
#elif defined Q_OS_WIN32
...
#else
#error unsupported OS
#endif

#endif // POPPLER_H
