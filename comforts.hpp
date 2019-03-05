#ifndef SHUFFLE_HPP
#define SHUFFLE_HPP

#include <cxxabi.h>
#include <time.h>
#include <cstdint>  //type defs
#include <iostream>
#include <utility>
#include "gsl/gsl_rng.h"

/**
 * Macro to return name of a class
 * @param  A any class or type
 * @return   cout the class name
 */
int STATUS = 0;
char *DEMANGLED = nullptr;
#define id(input)                                                         \
                                                                          \
    DEMANGLED = abi::__cxa_demangle(typeid(input).name(), 0, 0, &STATUS); \
    std::cout << DEMANGLED << std::endl;                                  \
    free(DEMANGLED);

namespace cj {

/**
 * Simple Knuth shuffle
 */
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

// ****************************************************************************
// *                              Tripple Struct                              *
// ****************************************************************************

typedef unsigned long long int ull;

struct Tripple {
    ull i, j, k;

    Tripple(const ull _i, const ull _j, const ull _k) : i(_i), j(_j), k(_k) {}
    Tripple(const Tripple &other) : i(other.i), j(other.j), k(other.k) {}

    bool operator==(const Tripple &rhs) const {
        if (i == rhs.i && j == rhs.j && k == rhs.k) {
            return true;
        } else {
            return false;
        }
    }
};

/**
 * Overloading iostream cout << to auto print Tripples
 */

std::ostream &operator<<(std::ostream &stream, const cj::Tripple &trip) {
    std::cout << trip.i << " " << trip.j << " " << trip.k;
    return stream;
}

// ****************************************************************************
// *                                Pair Struct                               *
// ****************************************************************************

/**
 * Simple container to hold a pair of values can be used with {i,j}
 */
template <class T>
class Pair {
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

}  // namespace cj

#endif  // SHUFFLE_HPP
