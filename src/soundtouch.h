#ifndef SOUNDTOUCH_H
#define SOUNDTOUCH_H

#include <Qt>

#ifdef Q_OS_LINUX
#include <soundtouch/SoundTouch.h>
#define HAVE_SOUNDTOUCH
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#elif defined Q_OS_WIN32

#if 1
#include "../../soundtouch/include/SoundTouch.h"
#define HAVE_SOUNDTOUCH
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
}
#endif

#else
#error unsupported OS
#endif

#endif // SOUNDTOUCH_H
