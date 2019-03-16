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
 * xrray/core.hpp
 *
 * This is the outer header file for the core headers bundle from the errey
 * module, implementing 1 , 2 and 3 dimensional arrays using expression
 * templates.
 *
 * Within this header bundle the main classes for Xrrays are defined as well as
 * all the class, overloads and functions for xrray expressions.
 */

#ifndef XRRAY_HPP
#define XRRAY_HPP

#include <cmath>
#include <complex>
#include <iostream>
#include <utility>
#include <xrray/extras.hpp>

namespace xrray {

using std::cout;
using std::endl;

// ****************************************************************************
// *                              Prototypes Main                             *
// ****************************************************************************

template <typename E, typename T>
class ErrExpr;

template <typename T>
class Xrray;

template <typename T>
class Window;

// ****************************************************************************
// *                          Prototypes Expressions                          *
// ****************************************************************************

template <typename Elhs, typename Erhs, typename T>
class XrraySum;

template <typename T, typename Erhs>
class XrrayScalSum;

template <typename Elhs, typename Erhs, typename T>
XrraySum<Elhs, Erhs, T> operator+(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
XrrayScalSum<T, Erhs> operator+(T const u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
XrrayScalSum<T, Erhs> operator+(ErrExpr<Erhs, T> const &v, T const u);

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
class XrrayMul;

template <typename T, typename Erhs>
class XrrayScalMul;

template <typename Elhs, typename Erhs, typename T>
XrrayMul<Elhs, Erhs, T> operator*(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
XrrayScalMul<T, Erhs> operator*(T const u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
XrrayScalMul<T, Erhs> operator*(ErrExpr<Erhs, T> const &v, T const u);

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
class XrrayPow;

template <typename T, typename Erhs>
class XrrayScalPow;

template <typename T, typename Erhs>
class XrrayPowScal;

template <typename Elhs, typename Erhs, typename T>
XrrayPow<Elhs, Erhs, T> pow(ErrExpr<Elhs, T> const &u,
                            ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
XrrayScalPow<T, Erhs> pow(T const u, ErrExpr<Erhs, T> const &v);

template <typename T, typename Erhs>
XrrayPowScal<T, Erhs> pow(ErrExpr<Erhs, T> const &v, T const u);

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
class XrrayMM;

template <typename Elhs, typename Erhs, typename T>
XrrayMM<Elhs, Erhs, T> mm(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v);

/*----------------------------------------------------------------------------*/

template <typename E, typename T, typename Funct>
class XrrayElemWise;

template <typename T, typename E>
class Slice;

/*----------------------------------------------------------------------------*/

template <typename T, typename E>
class Transpose12;

template <typename T, typename E>
class Transpose23;

template <typename T, typename E>
class Transpose13;

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
std::ostream &operator<<(std::ostream &stream, const xrray::ErrExpr<E, T> &err);

/*----------------------------------------------------------------------------*/

template <typename T = double>
Xrray<T> empty(const index_t i = 1, const index_t j = 1, const index_t k = 1);

template <typename T = double>
Xrray<T> ones(const index_t i = 1, const index_t j = 1, const index_t k = 1);

template <typename T = double>
Xrray<T> zeros(const index_t i = 1, const index_t j = 1, const index_t k = 1);

template <typename T = double>
Xrray<T> eye(const index_t i = 1, index_t j = 0);

template <typename T = double>
Xrray<T> linspace(const index_t first = 0, const index_t last = 1,
                  const index_t N = 50);

template <typename T = double, index_t step = 1, index_t start = 0>
Xrray<T> enumerate(const index_t i, const index_t j = 1, const index_t k = 1);

}  // namespace xrray

// ****************************************************************************
// *                   Including Sub-Headers for core bundle                  *
// ****************************************************************************

#include <xrray/core/base.hpp>
#include <xrray/core/expr.hpp>
#include <xrray/core/funct.hpp>

// ****************************************************************************
// *                         Easy access type aliases                         *
// ****************************************************************************

#ifdef XRRAY_TYPEDEF_ON

using xrray::Window;
using xrray::Xrray;

typedef xrray::Xrray<double> dxrray;
typedef xrray::Xrray<std::complex<double>> cxrray;

#endif  // XRRAY_TYPEDEF_ON

#endif  // XRRAY_HPP
