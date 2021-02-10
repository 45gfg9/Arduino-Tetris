#ifndef __BITSET_H__
#define __BITSET_H__

#include <stddef.h>

template <typename V = long>
class BitSet {
  static const size_t _VB = sizeof(V) * 8;

  unsigned int const _size;
  V *const _bits;

  void _set(int index, bool value);

public:
  BitSet(int size);
  ~BitSet();
  int size() const { return _size; }
  bool get(int index) const;
  void set(int index);
  void clear(int index);
};

#endif
