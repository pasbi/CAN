#ifndef GLOBAL_H
#define GLOBAL_H

#define private public
#define protected public

#include "../../Util/src/util.h"

#define WARNING qWarning() << QString("at %1:%2, in %3: ").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)

#endif // GLOBAL_H
