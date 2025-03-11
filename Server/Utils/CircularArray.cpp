#include <iostream>
#include <vector>

template <typename T>
class CircularArray
{
protected:
  int head;
  int capacity;
  int size;

public:
  std::vector<T> buffer;
  int tail;

  CircularArray(int cap) : capacity(cap), size(0), head(0), tail(0)
  {
    buffer.resize(capacity);
  }

  bool isFull() const
  {
    return size == capacity;
  }

  T getTailItem() const
  {
    int lastTail = (tail - 1) % capacity;
    if (lastTail < 0)
    {
      lastTail += capacity;
    }

    return buffer[lastTail];
  }

  void changeCapacity(int newCapacity, T paddingValue)
  {
    int oldCapacity = capacity;

    capacity = newCapacity;
    buffer.resize(newCapacity);

    for (int i = oldCapacity; i < newCapacity; i++)
    {
      buffer[i] = paddingValue;
    }
  }

  void push(T value)
  {
    if (isFull())
    {
      head = (head + 1) % capacity;
    }
    else
    {
      size++;
    }

    buffer[tail] = value;
    tail = (tail + 1) % capacity;
  }
};