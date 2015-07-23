#include "gitutil.h"
#include "git.h"

// global functions
bool check_git_call( int errorcode, int line, const char* file)
{
    if (errorcode != 0)
    {
        const git_error* error = giterr_last();
        if (error)
        {
            qWarning() << QString("Git call failed at %1:%2").arg(file).arg(line);
            qWarning() << "code: " << errorcode << ", class: " << error->klass << ", message: "<< error->message;
//            assert(false);
        }
        else
        {
            qWarning() << "invalid error.";
//            assert(false);
        }
        return false;
    }
    else
    {
        return true;
    }
}
