// implementation of a FIFO queue using  a cyclic doubly linked list, allowing
// for a rotate function to easily move first to last.

#ifndef CYQUE_HPP
#define CYQUE_HPP

#include <utility>

namespace cj {

// Cyque class methods: push(), pop(), pop_push(), first(), last(), size()
template <typename T> class Cyque {
private:
  // struct to act as lists nodes
  struct node {
    T data;
    node *next = nullptr;
    node *prev = nullptr;

    node(T &&in) : data{std::move(in)} {}
  };

  node *m_first = nullptr;
  node *m_last = nullptr;
  unsigned long m_size = 0;

public:
  // copy construct members into Cyque at back of queue
  inline void push(T in) {
    emplace(std::move(in));
    return;
  }

  // emplace member into Cyque at back of queue
  void emplace(T &&in) {
    node *place = new node{std::move(in)};

    if (m_size == 0) {
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

    ++m_size;
    return;
  }

  // delete the first (next accessible) element
  void pop(void) {
    --m_size;

    m_last->next = m_first->next;
    (m_first->next)->prev = m_last;

    node *tmp = m_first;
    m_first = m_first->next;

    delete tmp;
    return;
  }

  // move the first element to the back of the queue, very efficient
  void pop_push() {
    m_last = m_last->next;
    m_first = m_first->next;

    return;
  }

  // return ref to data in first node
  inline T &first(void) { return m_first->data; }

  // return ref to data in last node
  inline T &last(void) { return m_last->data; }

  // return number of elements in queue
  inline unsigned long size(void) { return m_size; }

  // delete all data in queue
  ~Cyque() {
    while (m_size > 0) {
      pop();
    }
  }
};

} // namespace cj

#endif // CYQUE_HPP