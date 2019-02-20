#ifndef SHUFFLE_HPP
#define SHUFFLE_HPP

#include "gsl/gsl_rng.h"
#include <cstdint> //type defs
#include <iostream>
#include <time.h>
#include <utility>

namespace cj {

// Simple Knuth shuffle
template <class T>
void knuth_shuffle(T &cards, uint_fast32_t N,
                   uint_fast32_t NUMBER_OF_SHUFFLES = 1) {
  gsl_rng *rng;
  rng = gsl_rng_alloc(gsl_rng_taus2);
  gsl_rng_set(rng, time(NULL));

  for (unsigned j = 0; j < NUMBER_OF_SHUFFLES; ++j) {
    for (unsigned i = 0; i < N - 1; ++i) {
      std::swap(cards[i], cards[i + gsl_rng_uniform_int(rng, N - i)]);
    }
  }

  gsl_rng_free(rng);

  return;
}

// Simple container to hold a pair of values can be used with {i,j}
template <class T> class Pair {
public:
  T i;
  T j;

  /*Pair& operator=(const T& other) {
    if (&other != this) {
      i = other[0];
      j = other[1];
    }
    return *this;
  }*/

  Pair &operator=(const T (&tuple)[2]) {
    i = tuple[0];
    j = tuple[1];

    return *this;
  }

  Pair() = default;
  Pair(const T (&tuple)[2]) {
    i = tuple[0];
    j = tuple[1];
  }
};

} // namespace cj

#endif // SHUFFLE_HPP
