#include <BitSet.h>
#include <math.h>

template <typename V>
BitSet<V>::BitSet(int size)
    : _size(size), _bits(new V[ceil((double)size / _VB)]) {}

template <typename V>
BitSet<V>::~BitSet() {
  delete[] _bits;
}

template <typename V>
bool BitSet<V>::get(int index) const {
  if (index >= _size)
    return false;

  int idx = index / _VB;
  int bit = index % _VB;

  return _bits[idx] & ((V)1 << bit);
}

template <typename V>
void BitSet<V>::_set(int index, bool value) {
  int idx = index / _VB;
  int bit = index % _VB;

  if (value) {
    _bits[idx] |= ((V)1 << bit);
  } else {
    _bits[idx] &= ~((V)1 << bit);
  }
}

template <typename V>
inline void BitSet<V>::set(int index) {
  _set(index, true);
}

template <typename V>
inline void BitSet<V>::clear(int index) {
  _set(index, false);
}
