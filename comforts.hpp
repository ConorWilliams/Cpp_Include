/**
 * comforts.hpp
 *
 * Copyright (c) 2019, C. J. Williams
 * All rights reserved.
 *
 * A collection of macros, small functions and small classes to make everything
 * a bit easier.
 */

#ifndef COMFORTS_HPP
#define COMFORTS_HPP

#include <cxxabi.h>  //de-mangle
#include <time.h>
#include <cstdint>  //type defs
#include <iostream>
#include <utility>
#include "gsl/gsl_rng.h"

namespace cj {

using std::cout;
using std::endl;

typedef unsigned long long int ull;

#ifdef DEBUGCOUT
/**
 * @brief      Macro to cout debug data, enable with "#define DEBUGCOUT" macro
 *
 * @param      string  The string to cout
 */
#define dcout(string) cout << string << endl
#else
#define dcout(string) ((void)0)
#endif  // NDBGCOUT

/**
 * @brief      macro like assert with custom error message and better debug
 *             message
 *
 * @param      CONDITIONAL  conditional statement to evaluate
 * @param      MESSAGE      The message to add to the output
 *
 */
#ifdef CHECK_ON
#define ASSERT(CONDITIONAL, MESSAGE)                                           \
    if (!(CONDITIONAL)) {                                                      \
        cout << "/*---------------------ERROR---------------------*/" << endl; \
        cout << "In: '" << __FILE__ << "'" << endl;                            \
        cout << "On line: " << __LINE__ << endl;                               \
        cout << "Condition '" << #CONDITIONAL << "' false" << endl;            \
        cout << "Message: " << MESSAGE << std::endl;                           \
        cout << "/*--------------------Exiting--------------------*/" << endl; \
        std::exit(0);                                                          \
    }                                                                          \
    ((void)0)
#else
#define ASSERT(x, message) ((void)0)
#endif

/*----------------------------------------------------------------------------*/

/**
 * @brief      cout the type of any input
 *
 * @param      in    the input whose type name to cout
 *
 * @tparam     T     template parameter
 */
template <typename T>
void id(T const &in) {
    int status = 0;
    char *demangled = abi::__cxa_demangle(typeid(in).name(), 0, 0, &status);
    std::cout << demangled << std::endl;
    free(demangled);
    return;
}

/*----------------------------------------------------------------------------*/

/**
 * @brief      performs a Knuth shuffle on a input of length N indexable with
 *             the [] operator
 *
 * @param      cards  The cards to shuffle
 * @param[in]  N      number of elements in cards
 * @param[in]  rng    The rng to use
 *
 * @tparam     T      type of cards
 */
template <class T>
void knuth_shuffle(T &cards, uint_fast32_t N, gsl_rng *rng = nullptr) {
    if (rng == nullptr) {
        rng = gsl_rng_alloc(gsl_rng_taus2);
        gsl_rng_set(rng, time(NULL));

        for (unsigned i = 0; i < N - 1; ++i) {
            std::swap(cards[i], cards[i + gsl_rng_uniform_int(rng, N - i)]);
        }

        gsl_rng_free(rng);

    } else {
        for (unsigned i = 0; i < N - 1; ++i) {
            std::swap(cards[i], cards[i + gsl_rng_uniform_int(rng, N - i)]);
        }
    }

    return;
}

// ****************************************************************************
// *                              Tripple Struct                              *
// ****************************************************************************

/**
 * @brief      Struct to hold 3 integer values
 */
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
 * @brief      overloading cout << to print Tripples
 *
 * @param      stream  The input stream
 * @param[in]  trip    The tripple to print
 *
 * @return     returns the stream
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
    Pair(const Pair<T> &in) : i{in.i}, j{in.j} {}
};

}  // namespace cj

#endif  // COMFORTS_HPP
