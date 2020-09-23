#include "FixedStack.h"

template <typename E>
FixedStack<E>::FixedStack(size_t size) : _size(size), _tos(0)
{
    _elementArray = new E[size];
}

template <typename E>
FixedStack<E>::~FixedStack()
{
    delete[] _elementArray;
    _elementArray = nullptr;
}

template <typename E>
bool FixedStack<E>::push(const E &e)
{
    if (isFull())
        return false;

    _elementArray[_tos++] = e;
    return true;
}

template <typename E>
bool FixedStack<E>::pop(E &e)
{
    if (isEmpty())
        return false;

    e = _elementArray[--_tos];
    return true;
}
