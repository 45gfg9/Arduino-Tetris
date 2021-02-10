#ifndef __PAINTABLE__
#define __PAINTABLE__

#include "defs.h"

struct Paintable {
  virtual ~Paintable() = default;
  virtual void paint() const = 0;
};

#endif
