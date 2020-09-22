#ifndef __PAINTABLE__
#define __PAINTABLE__

#include "defs.h"

class Paintable
{
protected:
    Painter *painter;

public:
    virtual void paint() const = 0;
};

#endif
