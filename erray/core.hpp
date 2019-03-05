/**
 * erray_core.hpp
 *
 * Core header file for the errey module implementing 1 , 2 and 3 dimensional
 * arrays using expression templates.
 *
 * Within this header bundle the main classes for Errays are defined as well as
 * all the class, overloads and functions for Erray expressions.
 *
 * Erray is a contraction of expression-array.
 *
 * Errays support slicing through .slice() for rvalues and through .window() for
 * lvalues.
 *
 * Errays can be printed through an overload of std::cout.
 *
 * operations: +, -, *, /, pow(), %, abs(), log(), ln(), log2(), cross(), mm()
 * functions: sum(), min(), max(), avg()
 * initialise: empty(), zeros(), ones(), linspace(), enumerate()
 *
 * Erray operations/assignments are, were appropriate, compatible with "scalars"
 * i.e entities of the Errays base type, T.
 *
 * Errays expressions are optimised to work with trivial/base types however the
 * module handles arbitrary base types and can be used as an effective container
 * for any type.
 *
 * TODO CONVERT ALL T PASSES TO PASS BY VALUE
 *
 */

#ifndef ERRAY_HPP
#define ERRAY_HPP

#include <cassert>
#include <cmath>
#include <comforts.hpp>
#include <iostream>
#include <type_traits>
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
ErrayScalSum<T, Erhs> operator+(T const &u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalSum<T, Erhs> operator+(ErrExpr<Erhs, T> const &v, T const &u);

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
class ErrayMul;

template <typename T, typename Erhs>
class ErrayScalMul;

template <typename Elhs, typename Erhs, typename T>
ErrayMul<Elhs, Erhs, T> operator*(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalMul<T, Erhs> operator*(T const &u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
ErrayScalMul<T, Erhs> operator*(ErrExpr<Erhs, T> const &v, T const &u);

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
Erray<T> empty(const ull i = 1, const ull j = 1, const ull k = 1);

template <typename T>
Erray<T> ones(const ull i = 1, const ull j = 1, const ull k = 1);

template <typename T>
Erray<T> zeros(const ull i = 1, const ull j = 1, const ull k = 1);

template <typename T>
Erray<T> identity(const ull i = 1, ull j = 0);

template <typename T>
Erray<T> linspace(const ull first = 0, const ull last = 1, const ull N = 50);

template <typename T, ull step = 1, ull start = 0>
Erray<T> enumerate(const ull i, const ull j = 1, const ull k = 1);

/*----------------------------------------------------------------------------*/

template <typename T, typename E>
void print(const ErrExpr<E, T> &err);

template <typename E, typename T>
std::ostream &operator<<(std::ostream &stream,
                         const cj::erray::ErrExpr<E, T> &err);

// ****************************************************************************
// *                           Including Sub-Headers                          *
// ****************************************************************************

#include <erray/core/base.hpp>
#include <erray/core/expr.hpp>
#include <erray/core/funct.hpp>

}  // namespace erray

}  // namespace cj

#endif  // ERRAY_HPP
