/**
 * extras.hpp
 *
 * Authors collection of macros, small functions and small classes to make
 * everything a bit easier.
 *
 */

#ifndef ERRAY_EXTRAS_HPP
#define ERRAY_EXTRAS_HPP

#include <cxxabi.h>  //de-mangle
#include <iostream>

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
 * message
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

// ****************************************************************************
// *                              Tripple Struct                              *
// ****************************************************************************

/**
 * @brief      Struct to hold 3 values
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

}  // namespace cj

#endif  // ERRAY_EXTRAS_HPP
