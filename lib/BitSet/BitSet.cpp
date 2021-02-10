#include <BitSet.h>
#include <math.h>

BitSet::BitSet(unsigned int size)
    : _size(size), _bits(new char[(size_t)ceil((double)size / _VB)]) {}

BitSet::~BitSet() {
  delete[] _bits;
}

bool BitSet::get(unsigned int index) const {
  if (index >= _size)
    return false;

  int idx = index / _VB;
  int bit = index % _VB;

  return _bits[idx] & (1 << bit);
}

void BitSet::_set(unsigned int index, bool value) {
  int idx = index / _VB;
  int bit = index % _VB;

  if (value) {
    _bits[idx] |= (1 << bit);
  } else {
    _bits[idx] &= ~(1 << bit);
  }
}

inline void BitSet::set(unsigned int index) {
  _set(index, true);
}

inline void BitSet::clear(unsigned int index) {
  _set(index, false);
}
