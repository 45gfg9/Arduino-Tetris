#ifndef __FIXEDSTACK__
#define __FIXEDSTACK__

#include <stddef.h>

// yys: template <typename E, int size>
template <typename E>
class FixedStack
{
    const size_t _size;

    E *_elementArray;
    size_t _tos;

public:
    FixedStack(size_t size);
    ~FixedStack();

    bool push(const E &e);
    bool pop(E &e);

    bool isEmpty() const { return _tos == 0; }
    bool isFull() const { return _tos == _size; }
};

#endif
