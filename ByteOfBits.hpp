#ifndef BYTEOFBITS_HPP
#define BYTEOFBITS_HPP

#include <cstdint>  //uint_t
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::invalid_argument;
using std::uint8_t;

namespace cj {

class ByteOfBits {
 public:
  uint8_t byte = 0;

  // function to set bit to val
  inline void set(const uint8_t bit, const uint8_t val) {
    if (val != 0 && val != 1) throw invalid_argument("val must be 0 or 1");
    if (bit > 7) throw invalid_argument("must be 0<=x<=7");
    if (val == ((byte >> bit) & 1)) {  // ((byte & (1 << bit)) >> bit))
      return;
    } else {
      byte = byte ^ (1 << bit);
      return;
    }
  }

  // functions to get val in bit
  inline bool get(const uint8_t bit) {
    if (bit > 7) throw invalid_argument("must be 0<=x<=7");
    return (byte >> bit) & 1;
  }

  inline void print(void) {
    for (int i = 0; i < 8; ++i) {
      cout << get(i) << ", ";
    }
    cout << endl;
    return;
  }
};

}  // namespace cj
#endif  // BYTEOFBITS_HPP//