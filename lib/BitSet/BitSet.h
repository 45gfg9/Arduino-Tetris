#ifndef __BITSET_H__
#define __BITSET_H__

#include <stddef.h>

class BitSet {
  static const size_t _VB = sizeof(char) * 8;

  unsigned int const _size;
  char *const _bits;

  void _set(unsigned int index, bool value);

public:
  BitSet(unsigned int size);
  ~BitSet();
  unsigned int size() const { return _size; }
  bool get(unsigned int index) const;
  void set(unsigned int index);
  void clear(unsigned int index);
};

#endif
