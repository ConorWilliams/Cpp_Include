/**
 * Copyright (c) 2019, C. J. Williams
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   (1) Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *   (2) Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *   (3) Neither the name of the author nor the names of any contributors may be
 *       used to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL C. J. WILLIAMS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * xrray/extras.hpp
 *
 * C. J. Williams' collection of macros, small functions, typedefs and small
 * classes to make everything a bit easier.
 */

#ifndef XRRAY_EXTRAS_HPP
#define XRRAY_EXTRAS_HPP

#include <cxxabi.h>  //de-mangle
#include <iostream>

namespace xrray {

using std::cout;
using std::endl;

typedef unsigned long int index_t;

#ifdef XRRAY_DEBUG_ON
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
#ifdef XRRAY_CHECK_ON
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
 * @brief      cout the class/type name of any input
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
// *                              Triple Struct                              *
// ****************************************************************************

/**
 * @brief      Struct to hold 3 values
 */
struct Triple {
    index_t i, j, k;

    Triple(const index_t _i, const index_t _j, const index_t _k)
        : i(_i), j(_j), k(_k) {}
    Triple(const Triple &other) : i(other.i), j(other.j), k(other.k) {}

    bool operator==(const Triple &rhs) const {
        if (i == rhs.i && j == rhs.j && k == rhs.k) {
            return true;
        } else {
            return false;
        }
    }
};

/**
 * @brief      overloading cout << to print Triples
 *
 * @param      stream  The input stream
 * @param[in]  trip    The tripple to print
 *
 * @return     returns the stream
 */
std::ostream &operator<<(std::ostream &stream, const Triple &trip) {
    std::cout << trip.i << " " << trip.j << " " << trip.k;
    return stream;
}

}  // namespace xrray

#endif  // XRRAY_EXTRAS_HPP
