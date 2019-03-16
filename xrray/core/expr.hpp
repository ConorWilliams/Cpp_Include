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
 * xrray/core/expr.hpp
 *
 * Part of the xrray module implementing 1, 2 and 3 dimensional arrays using
 * expression templates.
 *
 * Part of the core bundle, see errey_core.hpp for full details.
 *
 * This file contains the implementation for operators and functions that act on
 * xrray expressions and return an xrray expression that can be further
 * compounded.
 */

#ifndef XRRAY_EXPR_HPP
#define XRRAY_EXPR_HPP

#include <xrray/core.hpp>

namespace xrray {

// ****************************************************************************
// *                                 Addition                                 *
// ****************************************************************************

/**
 * @brief      Classes for performing element wise addition, specialised
 *             for expr + expr, scalar + expr and expr + scalar
 *
 * @tparam     Elhs  curiously  recursive type on lhs
 * @tparam     Erhs  curiously  recursive type on rhs
 * @tparam     T     underlying Xrray type
 */
template <typename Elhs, typename Erhs, typename T>
class XrraySum : public ErrExpr<XrraySum<Elhs, Erhs, T>, T> {
    ErrExpr<Elhs, T> const &_u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _u(i, j, k) + _v(i, j, k);
    }

    XrraySum(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v)
        : _u{u}, _v{v} {
        ASSERT(u.shape() == v.shape(), "Shape mismatch in XrraySum");
    }

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

template <typename T, typename Erhs>
class XrrayScalSum : public ErrExpr<XrrayScalSum<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _u + _v(i, j, k);
    }

    XrrayScalSum(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
XrraySum<Elhs, Erhs, T> operator+(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v) {
    return XrraySum<Elhs, Erhs, T>{u, v};
}

template <typename T, typename Erhs>
XrrayScalSum<T, Erhs> operator+(T const u, ErrExpr<Erhs, T> const &v) {
    return XrrayScalSum<T, Erhs>{u, v};
}

template <typename T, typename Erhs>
XrrayScalSum<T, Erhs> operator+(ErrExpr<Erhs, T> const &v, T const u) {
    return XrrayScalSum<T, Erhs>{u, v};
}

// ****************************************************************************
// *                               Multiplication                             *
// ****************************************************************************

/**
 * @brief      Classes for performing element wise multiplication, specialised
 *             for expr * expr, scalar * expr and expr * scalar
 *
 * @tparam     Elhs  curiously  recursive type on lhs
 * @tparam     Erhs  curiously  recursive type on rhs
 * @tparam     T     underlying Xrray type
 */
template <typename Elhs, typename Erhs, typename T>
class XrrayMul : public ErrExpr<XrrayMul<Elhs, Erhs, T>, T> {
    ErrExpr<Elhs, T> const &_u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _u(i, j, k) * _v(i, j, k);
    }

    XrrayMul(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v)
        : _u{u}, _v{v} {
        ASSERT(u.shape() == v.shape(), "Shape mismatch in XrrayMul");
    }

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

template <typename T, typename Erhs>
class XrrayScalMul : public ErrExpr<XrrayScalMul<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _u * _v(i, j, k);
    }

    XrrayScalMul(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
XrrayMul<Elhs, Erhs, T> operator*(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v) {
    return XrrayMul<Elhs, Erhs, T>{u, v};
}

template <typename T, typename Erhs>
XrrayScalMul<T, Erhs> operator*(T const u, ErrExpr<Erhs, T> const &v) {
    return XrrayScalMul<T, Erhs>{u, v};
}

template <typename T, typename Erhs>
XrrayScalMul<T, Erhs> operator*(ErrExpr<Erhs, T> const &v, T const u) {
    return XrrayScalMul<T, Erhs>{u, v};
}

// ****************************************************************************
// *                              Exponentiation                              *
// ****************************************************************************

/**
 * @brief      Classes for performing element wise exponentiation, specialised
 *             for expr ^ expr, scalar ^ expr and expr ^ scalar
 *
 * @tparam     Elhs  curiously  recursive type on lhs
 * @tparam     Erhs  curiously  recursive type on rhs
 * @tparam     T     underlying Xrray type
 */
template <typename Elhs, typename Erhs, typename T>
class XrrayPow : public ErrExpr<XrrayPow<Elhs, Erhs, T>, T> {
    ErrExpr<Elhs, T> const &_u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return std::pow(_u(i, j, k), _v(i, j, k));
    }

    XrrayPow(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v)
        : _u{u}, _v{v} {
        ASSERT(u.shape() == v.shape(), "Shape mismatch in XrrayPow");
    }

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

template <typename T, typename Erhs>
class XrrayScalPow : public ErrExpr<XrrayScalPow<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return std::pow(_u, _v(i, j, k));
    }

    XrrayScalPow(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

template <typename T, typename Erhs>
class XrrayPowScal : public ErrExpr<XrrayPowScal<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return std::pow(_v(i, j, k), _u);
    }

    XrrayPowScal(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
XrrayPow<Elhs, Erhs, T> pow(ErrExpr<Elhs, T> const &u,
                            ErrExpr<Erhs, T> const &v) {
    return XrrayPow<Elhs, Erhs, T>{u, v};
}

template <typename T, typename Erhs>
XrrayScalPow<T, Erhs> pow(T const u, ErrExpr<Erhs, T> const &v) {
    return XrrayScalPow<T, Erhs>{u, v};
}

template <typename T, typename Erhs>
XrrayPowScal<T, Erhs> pow(ErrExpr<Erhs, T> const &v, T const u) {
    return XrrayPowScal<T, Erhs>{u, v};
}

// ****************************************************************************
// *                                  Slicing                                 *
// ****************************************************************************

/**
 * @brief      Class for returning a view into an xrray expression of given
 *             shape
 *
 * @tparam     T     underlying Xrray type
 * @tparam     E     curiously  recursive type
 */
template <typename T, typename E>
class Slice : public ErrExpr<Slice<T, E>, T> {
    ErrExpr<E, T> const &_v;
    Triple const _shape{0, 0, 0};
    Triple const _offset{0, 0, 0};
    index_t const _size;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _v(i + _offset.i, j + _offset.j, k + _offset.k);
    }

    Slice(ErrExpr<E, T> const &v, Triple const &o, Triple const &s)
        : _v{v}, _shape{s}, _offset{o}, _size{_shape.i * _shape.j * _shape.k} {}

    Triple shape(void) const { return _shape; }
    index_t size(void) const { return _size; }
};

// ****************************************************************************
// *                             Auto Element-Wise                            *
// ****************************************************************************

/**
 * @brief      Class for converting functions to element wise functions that act
 *             on xrray expressions, requires the function to be wrapped in a
 *             wrapper class. There is a macro defined in base that can be used
 *             in ErrExpr to easily convert any single input function to an
 *             xrray element-wise expression that integrates into expression
 *             templates using this class.
 *
 * @tparam     E      curiously  recursive type
 * @tparam     T      underlying Xrray type
 * @tparam     Funct  class wrapping function.
 */
template <typename E, typename T, typename Funct>
class XrrayElemWise : public ErrExpr<XrrayElemWise<E, T, Funct>, T> {
    ErrExpr<E, T> const &_v;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return Funct::operate(_v(i, j, k));
    }

    XrrayElemWise(ErrExpr<E, T> const &v) : _v{v} {}

    Triple shape(void) const { return _v.shape(); }
    index_t size(void) const { return _v.size(); }
};

// ****************************************************************************
// *                                 Transpose                                *
// ****************************************************************************

/**
 * @brief      Transpose an Xrray switching i and j
 *
 * @tparam     T     underlying Xrray type
 * @tparam     E     curiously  recursive type
 */
template <typename T, typename E>
class Transpose12 : public ErrExpr<Transpose12<T, E>, T> {
    ErrExpr<E, T> const &_v;
    Triple const _shape{0, 0, 0};

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _v(j, i, k);
    }

    Transpose12(ErrExpr<E, T> const &v)
        : _v{v}, _shape{v.shape().j, v.shape().i, v.shape().k} {}

    Triple shape(void) const { return _shape; }
    index_t size(void) const { return _v.size(); }
};

/**
 * @brief      Transpose an Xrray switching j and k
 *
 * @tparam     T     underlying Xrray type
 * @tparam     E     curiously  recursive type
 */
template <typename T, typename E>
class Transpose23 : public ErrExpr<Transpose23<T, E>, T> {
    ErrExpr<E, T> const &_v;
    Triple const _shape{0, 0, 0};

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _v(i, k, j);
    }

    Transpose23(ErrExpr<E, T> const &v)
        : _v{v}, _shape{v.shape().j, v.shape().i, v.shape().k} {}

    Triple shape(void) const { return _shape; }
    index_t size(void) const { return _v.size(); }
};

/**
 * @brief      Transpose an Xrray switching i and k
 *
 * @tparam     T     underlying Xrray type
 * @tparam     E     curiously  recursive type
 */
template <typename T, typename E>
class Transpose13 : public ErrExpr<Transpose13<T, E>, T> {
    ErrExpr<E, T> const &_v;
    Triple const _shape{0, 0, 0};

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return _v(k, j, i);
    }

    Transpose13(ErrExpr<E, T> const &v)
        : _v{v}, _shape{v.shape().j, v.shape().i, v.shape().k} {}

    Triple shape(void) const { return _shape; }
    index_t size(void) const { return _v.size(); }
};

// ****************************************************************************
// *                           Matrix Multiplication                          *
// ****************************************************************************

/**
 * @brief      Class for performing standard matrix multiplication on i by j by
 *             1 dimensional erray expressions
 *
 * @tparam     Elhs  curiously  recursive type on lhs
 * @tparam     Erhs  curiously  recursive type on rhs
 * @tparam     T     underlying Xrray type
 */
template <typename Elhs, typename Erhs, typename T>
class XrrayMM : public ErrExpr<XrrayMM<Elhs, Erhs, T>, T> {
    ErrExpr<Elhs, T> const &_u;
    ErrExpr<Erhs, T> const &_v;
    index_t const _sum_length;
    Triple const _shape{0, 0, 0};
    index_t const _size;

   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        T tmp = _u(i, 0) * _v(0, j);

        for (index_t index = 1; index < _sum_length; ++index) {
            tmp += _u(i, index) * _v(index, j);
        }
        return tmp;
    }

    XrrayMM(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v)
        : _u{u},
          _v{v},
          _sum_length{u.shape().j},
          _shape{u.shape().i, v.shape().j, 1},
          _size{_shape.i * _shape.j} {
        ASSERT(u.shape().j == v.shape().i, "Shapes wrong on XrrayMM");
        ASSERT(u.shape().k == 1 && v.shape().k == 1, "XrrayMM for for 2D only");
    }

    Triple shape(void) const { return _shape; }
    index_t size(void) const { return _size; }
};
/*----------------------------------------------------------------------------*/

template <typename Elhs, typename Erhs, typename T>
XrrayMM<Elhs, Erhs, T> mm(ErrExpr<Elhs, T> const &u,
                          ErrExpr<Erhs, T> const &v) {
    return XrrayMM<Elhs, Erhs, T>{u, v};
}

// ****************************************************************************
// *                            General Contraction                           *
// ****************************************************************************

}  // namespace xrray

#endif  // XRRAY_EXPR_HPP
