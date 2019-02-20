#ifndef CYQUE_HPP
#define CYQUE_HPP

#include <iostream>
#include <vector>

#include "DenseBits.hpp"

using std::cout;
using std::endl;

static const unsigned BUFFER_SIZE = 10;

namespace cj {

template <typename T> class Cyque {
public:
  struct item {
    T data;
    item *next = nullptr;
    item *prev = nullptr;

    item(T &&in) : T{std::move(in)} {}

    ~item() {
      data.~T();
      next = nullptr;
      prev = nullptr;
    }
  };

  item *m_first = nullptr;
  item *m_last = nullptr;
  unsigned long size = 0;

  inline void push(T in) { emplace(std::move(in)); }

  void emplace(T &&in) {
    cout << "em-placing" << endl;

    item *place = static_cast<item *>(::operator new(sizeof(item)));

    place->data = std::move(in);

    if (size == 0) {

      place->next = place;
      place->prev = place;

      m_last = place;
      m_first = place;

    } else {
      place->next = m_first;
      place->prev = m_last;

      m_last->next = place;
      m_first->prev = place;

      m_last = place;
    }

    ++size;

    place = nullptr;
  }

  void pop(void) {
    --size;

    m_last->next = m_first->next;
    (m_first->next)->prev = m_last;
    item *tmp = m_first;
    m_first = m_first->next;

    tmp->~item();
    ::operator delete(tmp);
  }

  inline T &first(void) { return m_first->data; }

  Cyque() = default;

  ~Cyque() {
    cout << "C destruct" << endl;

    while (size > 0) {
      pop();
    }

    /*

    item *current = m_first;
    item *next = nullptr;

    for (unsigned i = 0; i < size; ++i) {
      next = current->next;
      current->~item();
      ::operator delete(current);
      current = next;
    }
    */
  }
};

} // namespace cj

#endif // CYQUE_HPP

/*

char *place = static_cast<char *>(::operator new(sizeof(Foo)));

Foo *f1 = new (place) Foo;

f1->~Foo();

::operator delete(place);

*/