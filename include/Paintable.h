#ifndef __PAINTABLE__
#define __PAINTABLE__

#include "defs.h"

class Paintable
{
protected:
    ;

public:
    virtual ~Paintable() = default;
    virtual void paint() const = 0;
};

#endif
