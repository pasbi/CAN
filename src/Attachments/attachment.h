#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include "taggable.h"
#include "creatable.h"

class Attachment : public Taggable, public Creatable
{
public:
    Attachment();
    virtual QString bark() { return "Attachment"; }
};

#endif // ATTACHMENT_H
