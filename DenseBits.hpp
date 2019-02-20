// A collection of dense bit representations, ByteOfBits for 8 bits and
// DenseBits for N bits. DenseBitsH uses heap allocation, DenseBits uses stack
// allocation.

#ifndef BITSETS_HPP
#define BITSETS_HPP

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "BitsSetTable256.hpp" //lookup table

using std::copy;
using std::cout;
using std::endl;
using std::invalid_argument;

namespace cj {

static const unsigned char ONE = 1;
static const unsigned char MAX = 255;
static const uint_fast32_t MASK = 7;

// Class to access each bit stored in a byte separately, methods:
// test(), flip(), high(), low(), print(), count()
class ByteOfBits {
  unsigned char byte = 0;

public:
  // functions to get val in bit^th position
  const bool test(const unsigned char bit) {
    switch (bit) {
    case 0:
      return (byte >> 0) & ONE;
    case 1:
      return (byte >> 1) & ONE;
    case 2:
      return (byte >> 2) & ONE;
    case 3:
      return (byte >> 3) & ONE;
    case 4:
      return (byte >> 4) & ONE;
    case 5:
      return (byte >> 5) & ONE;
    case 6:
      return (byte >> 6) & ONE;
    case 7:
      return (byte >> 7) & ONE;
    default:
      throw invalid_argument("Fallen through switch in test()");
    }
  }

  // functions to set val in bit^th position to 1
  void high(const unsigned char bit) {
    switch (bit) {
    case 0:
      byte |= (ONE << 0);
      return;
    case 1:
      byte |= (ONE << 1);
      return;
    case 2:
      byte |= (ONE << 2);
      return;
    case 3:
      byte |= (ONE << 3);
      return;
    case 4:
      byte |= (ONE << 4);
      return;
    case 5:
      byte |= (ONE << 5);
      return;
    case 6:
      byte |= (ONE << 6);
      return;
    case 7:
      byte |= (ONE << 7);
      return;
    default:
      throw invalid_argument("Fallen through switch in high()");
    }
  }

  // functions to set val in bit^th position to 0
  void low(const unsigned char bit) {
    switch (bit) {
    case 0:
      byte &= (MAX ^ (ONE << 0));
      return;
    case 1:
      byte &= (MAX ^ (ONE << 1));
      return;
    case 2:
      byte &= (MAX ^ (ONE << 2));
      return;
    case 3:
      byte &= (MAX ^ (ONE << 3));
      return;
    case 4:
      byte &= (MAX ^ (ONE << 4));
      return;
    case 5:
      byte &= (MAX ^ (ONE << 5));
      return;
    case 6:
      byte &= (MAX ^ (ONE << 6));
      return;
    case 7:
      byte &= (MAX ^ (ONE << 0));
      return;
    default:
      throw invalid_argument("Fallen through switch in low()");
    }
  }

  // functions to swap val in bit^th position
  void flip(const unsigned char bit) {
    switch (bit) {
    case 0:
      byte ^= (ONE << 0);
      return;
    case 1:
      byte ^= (ONE << 1);
      return;
    case 2:
      byte ^= (ONE << 2);
      return;
    case 3:
      byte ^= (ONE << 3);
      return;
    case 4:
      byte ^= (ONE << 4);
      return;
    case 5:
      byte ^= (ONE << 5);
      return;
    case 6:
      byte ^= (ONE << 6);
      return;
    case 7:
      byte ^= (ONE << 7);
      return;
    default:
      throw invalid_argument("Fallen through switch in flip()");
    }
  }

  // function to lookup how many bits are set to one in byte in the lookup table
  inline unsigned char count(void) { return cj::BitsSetTable256[byte]; }

  // function to print the byte on one line one bit at a time
  void print(bool endline) {
    for (int i = 0; i < 8; ++i) {
      cout << test(i) << ", ";
    }
    if (endline)
      cout << endl;
    return;
  }

  inline unsigned char data(void) { return byte; }
};

// Class to chain ByteOfBits together to achieve any length bit string, methods:
// test(), flip(), high(), low(), print(), count(), print_all().
// Actual size is always a multiple of 8 bits.
// Allocated on heap
template <uint_fast32_t size> class DenseBitsH {
public:
  static const uint_fast32_t length = (size >> 3) + ((size & MASK) != 0);
  ByteOfBits *set = nullptr;

  // functions to get val in bit^th position
  inline bool test(uint_fast32_t bit) {
    bit %= size;
    return set[(bit % size) >> 3].test(bit & MASK);
  }

  // functions to set val in bit^th position to 1
  inline void high(uint_fast32_t bit) {
    bit %= size;
    set[bit >> 3].high(bit & MASK);
  }

  // functions to set val in bit^th position to 0
  inline void low(uint_fast32_t bit) {
    bit %= size;
    set[bit >> 3].low(bit & MASK);
  }

  // functions to swap val in bit^th position
  inline void flip(uint_fast32_t bit) {
    bit %= size;
    set[bit >> 3].flip(bit & MASK);
  }

  // function to see how many bits are set to 1 in set
  inline uint_fast32_t count(void) {
    uint_fast32_t count = 0;
    for (uint_fast32_t i = 0; i < length; ++i) {
      count += set[i].count();
    }
    return count;
  }

  // function to print the set on one line one bit at a time
  // Just print bits up to size
  void print(void) {
    for (uint_fast32_t i = 0; i < size; ++i) {
      cout << set[i >> 3].test(i & MASK);
    }
    cout << " endl" << endl;
  }

  // function to print the set on one line one bit at a time
  // prints all bits
  void print_all(void) {
    for (uint_fast32_t i = 0; i < length * 8; ++i) {
      cout << set[i >> 3].test(i & MASK);
    }
    cout << " endl" << endl;
  }

  // constructor
  DenseBitsH<size>(void) {
    if ((size <= 8) != 0) {
      cout << "For Sets this small use ByteOfBits" << endl;
    }
    set = new ByteOfBits[length];
    // cout << "construct" << endl;
    return;
  }

  // de-constructor
  ~DenseBitsH<size>(void) {
    delete[] set;
    set = nullptr;
    // cout << "delete" << endl;
    return;
  }

  // copy constructor for functions
  DenseBitsH<size>(DenseBitsH const &other) {
    // cout << "copy" << endl;
    if (this != &other) {
      delete[] set;
      set = nullptr;
      set = new ByteOfBits[length];
      *this = other;
    }

    return;
  }

  // assignment operator
  DenseBitsH<size> &operator=(const DenseBitsH &other) {
    // cout << "assignment" << endl;
    if (this != &other) {
      std::copy(other.set, other.set + length, set);
    }
    return *this;
  }

  // move operator
  DenseBitsH<size> &operator=(DenseBitsH &&other) noexcept {
    // cout << "move operator" << endl;
    if (this != &other) {
      delete[] set;
      set = nullptr;
      set = other.set;
      other.set = nullptr;
    }
    // cout << "Move ends" << endl;
    return *this;
  }
};

// Class to chain ByteOfBits together to achieve any length bit string, methods:
// test(), flip(), high(), low(), print(), count(), print_all().
// Actual size is always a multiple of 8 bits.
// Allocated on stack.
template <uint_fast32_t size> class DenseBits {
public:
  static const uint_fast32_t length = (size >> 3) + ((size & MASK) != 0);
  ByteOfBits set[length];

  // functions to get val in bit^th position
  inline bool test(uint_fast32_t bit) {
    bit %= size;
    return set[bit >> 3].test(bit & MASK);
  }

  // functions to set val in bit^th position to 1
  inline void high(uint_fast32_t bit) {
    bit %= size;
    set[bit >> 3].high(bit & MASK);
  }

  // functions to set val in bit^th position to 0
  inline void low(uint_fast32_t bit) {
    bit %= size;
    set[bit >> 3].low(bit & MASK);
  }

  // functions to swap val in bit^th position
  inline void flip(uint_fast32_t bit) {
    bit %= size;
    set[bit >> 3].flip(bit & MASK);
  }

  // function to see how many bits are set to 1 in set
  inline uint_fast32_t count(void) {
    uint_fast32_t count = 0;
    for (uint_fast32_t i = 0; i < length; ++i) {
      count += set[i].count();
    }
    return count;
  }

  // function to print the set on one line one bit at a time
  // Just print bits up to size
  void print(void) {
    for (uint_fast32_t i = 0; i < size; ++i) {
      cout << set[i >> 3].test(i & MASK);
    }
    cout << " endl" << endl;
  }

  // function to print the set on one line one bit at a time
  // prints all bits
  void print_all(void) {
    for (uint_fast32_t i = 0; i < length * 8; ++i) {
      cout << set[i >> 3].test(i & MASK);
    }
    cout << " endl" << endl;
  }
};

} // namespace cj

#endif // BITSETS_HPP//