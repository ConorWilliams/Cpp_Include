/**
 * erray_core.hpp
 *
 * The following code (part of the erray module) is Copyright © 2019 C. J.
 * Williams, all rights reserved.
 *
 * This is the outer header file for the core headers bundle from the errey
 * module, implementing 1 , 2 and 3 dimensional arrays using expression
 * templates.
 *
 * Within this header bundle the main classes for Errays are defined as well as
 * all the class, overloads and functions for erray expressions.
 *
 * erray expressions build an "expression tree", that enable lazy
 * evaluation of expressions upon assignment to an Erray eliminating temporaries
 * and repeated looping over Errays.
 *
 * Erray is a contraction of expression-array.
 *
 * An Erray is considered an erray expression.
 *
 * Erray expressions support slicing through .slice() for rvalues and through
 * .window() for lvalues.
 *
 * Errays can be printed through an overload of std::cout.
 *
 * operations: +, -, *, /, pow(), %, , cross(), mm(), sqrt(), transpose(),
 *             reshape()
 *
 * functions: sum(), min(), max(), avg(), swap(windows)
 *
 * initialise: empty(), zeros() ones(), linspace(), enumerate(), random(),
 * random_int()
 *
 * Erray operations/assignments are - where appropriate - compatible with
 * "scalars" i.e entities of the erray expression base type, T.
 *
 * Errays expressions are optimised to work with trivial/base types however the
 * module handles arbitrary base types and can be used as an effective container
 * for any type.
 *
 * All shape checking can be disabled through the "#define NDEBUG" macro.
 * All debug printing can be enabled through the "#define DEBUGCOUT" macro.
 *
 */

/**
 * TODO: add comments to base.hpp and switch to dcout()
 * TODO: swap shape checking to custom macro and add macro bound checking,
 *       remove unused includes
 * TODO: implement missing stuff
 *
 * TODO: build fft header bundle
 */

#ifndef ERRAY_HPP
#define ERRAY_HPP

#include <cassert>
#include <cmath>
#include <complex>
#include <erray/extras.hpp>
#include <iostream>
#include <utility>

namespace cj {
namespace erray {

using std::cout;
using std::endl;

// ****************************************************************************
// *                              Prototypes Main                             *
// ****************************************************************************

template <typename E, typename T>
class ErrExpr;

template <typename T>
class Erray;

template <typename T>
class Window;

// ****************************************************************************
// *                          Prototypes Expressions                          *
// ****************************************************************************

template <typename Elhs, typename Erhs, typename T>
class ErraySum;

template <typename T, typename Erhs>
class ErrayScalSum;

template <typename Elhs, typename Erhs, typename T>
ErraySum<Elhs, Erhs, T> operator+(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalSum<T, Erhs> operator+(T const u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalSum<T, Erhs> operator+(ErrExpr<Erhs, T> const &v, T const u);

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
class ErrayMul;

template <typename T, typename Erhs>
class ErrayScalMul;

template <typename Elhs, typename Erhs, typename T>
ErrayMul<Elhs, Erhs, T> operator*(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalMul<T, Erhs> operator*(T const u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalMul<T, Erhs> operator*(ErrExpr<Erhs, T> const &v, T const u);

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
class ErrayPow;

template <typename T, typename Erhs>
class ErrayScalPow;

template <typename T, typename Erhs>
class ErrayPowScal;

template <typename Elhs, typename Erhs, typename T>
ErrayPow<Elhs, Erhs, T> pow(ErrExpr<Elhs, T> const &u,
                            ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalPow<T, Erhs> pow(T const u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayPowScal<T, Erhs> pow(ErrExpr<Erhs, T> const &v, T const u);

/*----------------------------------------------------------------------------*/

template <typename T, typename E>
class Slice;

template <typename T, typename E>
Slice<T, E> slice(ErrExpr<E, T> const &expr, const ull i0, const ull i1);

template <typename T, typename E>
Slice<T, E> slice(ErrExpr<E, T> const &expr, const ull i0, const ull i1,
                  const ull i2, const ull i3);

template <typename T, typename E>
Slice<T, E> slice(ErrExpr<E, T> const &expr, const ull i0, const ull i1,
                  const ull i2, const ull i3, const ull i4, const ull i5);

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
class ErrayMM;

template <typename Elhs, typename Erhs, typename T>
ErrayMM<Elhs, Erhs, T> mm(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v);

/*----------------------------------------------------------------------------*/

template <typename E, typename T, typename Funct>
class ErrayElemWise;

template <typename T, typename E>
class Transpose;

// ****************************************************************************
// *                           Prototypes functions                           *
// ****************************************************************************

template <typename E, typename T>
T sum(ErrExpr<E, T> const &expr);

template <typename E, typename T>
T max(ErrExpr<E, T> const &expr);

template <typename E, typename T>
T min(ErrExpr<E, T> const &expr);

/*----------------------------------------------------------------------------*/

template <typename T>
void swap(Window<T> &left, Window<T> &right);

template <typename T>
void swap(Window<T> &&left, Window<T> &&right);

template <typename T, typename E>
void print(const ErrExpr<E, T> &err);

template <typename E, typename T>
std::ostream &operator<<(std::ostream &stream,
                         const cj::erray::ErrExpr<E, T> &err);

/*----------------------------------------------------------------------------*/

template <typename T = double>
Erray<T> empty(const ull i = 1, const ull j = 1, const ull k = 1);

template <typename T = double>
Erray<T> ones(const ull i = 1, const ull j = 1, const ull k = 1);

template <typename T = double>
Erray<T> zeros(const ull i = 1, const ull j = 1, const ull k = 1);

template <typename T = double>
Erray<T> identity(const ull i = 1, ull j = 0);

template <typename T = double>
Erray<T> linspace(const ull first = 0, const ull last = 1, const ull N = 50);

template <typename T = double, ull step = 1, ull start = 0>
Erray<T> enumerate(const ull i, const ull j = 1, const ull k = 1);

}  // namespace erray
}  // namespace cj

// ****************************************************************************
// *                   Including Sub-Headers for core bundle                  *
// ****************************************************************************

#include <erray/core/base.hpp>
#include <erray/core/expr.hpp>
#include <erray/core/funct.hpp>

// ****************************************************************************
// *                         Easy access type aliases                         *
// ****************************************************************************

#ifdef ERRAY_TYPEDEF_ON

using cj::erray::Erray;
typedef cj::erray::Erray<double> derray;
typedef cj::erray::Erray<std::complex<double>> cplxrray;

#endif  // ERRAY_TYPEDEF_ON

#endif  // ERRAY_HPP
