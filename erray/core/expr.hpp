/**
 * erray_expr.hpp
 *
 * The following code (part of the erray module) is © 2019 C. J. Williams.
 *
 * Part of the erray module implementing 1, 2 and 3 dimensional arrays using
 * expression templates.
 *
 * Part of the core bundle, see errey_core.hpp for full details.
 *
 * This file contains the implementation for operators and functions that act on
 * erray expressions and return an erray expression that can be further
 * compounded.
 *
 */

#ifndef ERRAY_EXPR_HPP
#define ERRAY_EXPR_HPP

// ****************************************************************************
// *   Classes, operators and functions for the Erray class returning         *
// *                           expression templates                           *
// ****************************************************************************

#include <erray/core.hpp>

/* Macro used in most of the derived classes that make up Erray expressions */

#define COMMON_MACRO                                 \
    Tripple shape(void) const { return _v.shape(); } \
                                                     \
    ull size(void) const { return _v.size(); }

// ****************************************************************************
// *                                 Addition                                 *
// ****************************************************************************

template <typename Elhs, typename Erhs, typename T>
class ErraySum : public ErrExpr<ErraySum<Elhs, Erhs, T>, T> {
    ErrExpr<Elhs, T> const &_u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return _u(i, j, k) + _v(i, j, k);
    }

    ErraySum(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v)
        : _u{u}, _v{v} {
        assert(u.shape() == v.shape());
    }

    COMMON_MACRO
};

template <typename T, typename Erhs>
class ErrayScalSum : public ErrExpr<ErrayScalSum<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return _u + _v(i, j, k);
    }

    ErrayScalSum(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    COMMON_MACRO
};

template <typename Elhs, typename Erhs, typename T>
ErraySum<Elhs, Erhs, T> operator+(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v) {
    return ErraySum<Elhs, Erhs, T>{u, v};
}

template <typename T, typename Erhs>
ErrayScalSum<T, Erhs> operator+(T const &u, ErrExpr<Erhs, T> const &v) {
    return ErrayScalSum<T, Erhs>{u, v};
}

template <typename T, typename Erhs>
ErrayScalSum<T, Erhs> operator+(ErrExpr<Erhs, T> const &v, T const &u) {
    return ErrayScalSum<T, Erhs>{u, v};
}

// ****************************************************************************
// *                               Multiplication                             *
// ****************************************************************************

template <typename Elhs, typename Erhs, typename T>
class ErrayMul : public ErrExpr<ErrayMul<Elhs, Erhs, T>, T> {
    ErrExpr<Elhs, T> const &_u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return _u(i, j, k) * _v(i, j, k);
    }

    ErrayMul(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v)
        : _u{u}, _v{v} {
        assert(u.shape() == v.shape());
    }

    COMMON_MACRO
};

template <typename T, typename Erhs>
class ErrayScalMul : public ErrExpr<ErrayScalMul<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return _u * _v(i, j, k);
    }

    ErrayScalMul(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    COMMON_MACRO
};

template <typename Elhs, typename Erhs, typename T>
ErrayMul<Elhs, Erhs, T> operator*(ErrExpr<Elhs, T> const &u,
                                  ErrExpr<Erhs, T> const &v) {
    return ErrayMul<Elhs, Erhs, T>{u, v};
}

template <typename T, typename Erhs>
ErrayScalMul<T, Erhs> operator*(T const &u, ErrExpr<Erhs, T> const &v) {
    return ErrayScalMul<T, Erhs>{u, v};
}

template <typename T, typename Erhs>
ErrayScalMul<T, Erhs> operator*(ErrExpr<Erhs, T> const &v, T const &u) {
    return ErrayScalMul<T, Erhs>{u, v};
}

// ****************************************************************************
// *                              Exponentiation                              *
// ****************************************************************************

template <typename Elhs, typename Erhs, typename T>
class ErrayPow : public ErrExpr<ErrayPow<Elhs, Erhs, T>, T> {
    ErrExpr<Elhs, T> const &_u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return std::pow(_u(i, j, k), _v(i, j, k));
    }

    ErrayPow(ErrExpr<Elhs, T> const &u, ErrExpr<Erhs, T> const &v)
        : _u{u}, _v{v} {
        assert(u.shape() == v.shape());
    }

    COMMON_MACRO
};

template <typename T, typename Erhs>
class ErrayScalPow : public ErrExpr<ErrayScalPow<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return std::pow(_u, _v(i, j, k));
    }

    ErrayScalPow(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    COMMON_MACRO
};

template <typename T, typename Erhs>
class ErrayPowScal : public ErrExpr<ErrayPowScal<T, Erhs>, T> {
    T const _u;
    ErrExpr<Erhs, T> const &_v;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return std::pow(_v(i, j, k), _u);
    }

    ErrayPowScal(T const u, ErrExpr<Erhs, T> const &v) : _u{u}, _v{v} {}

    COMMON_MACRO
};

template <typename Elhs, typename Erhs, typename T>
ErrayPow<Elhs, Erhs, T> pow(ErrExpr<Elhs, T> const &u,
                            ErrExpr<Erhs, T> const &v) {
    return ErrayPow<Elhs, Erhs, T>{u, v};
}

template <typename T, typename Erhs>
ErrayScalPow<T, Erhs> pow(T const u, ErrExpr<Erhs, T> const &v) {
    return ErrayScalPow<T, Erhs>{u, v};
}

template <typename T, typename Erhs>
ErrayPowScal<T, Erhs> pow(ErrExpr<Erhs, T> const &v, T const u) {
    return ErrayPowScal<T, Erhs>{u, v};
}

// ****************************************************************************
// *                                  Slicing                                 *
// ****************************************************************************

template <typename T, typename E>
class Slice : public ErrExpr<Slice<T, E>, T> {
    ErrExpr<E, T> const &_v;
    Tripple const _shape;
    Tripple const _offset;
    ull const _size;

   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return _v(i + _offset.i, j + _offset.j, k + _offset.k);
    }

    Slice(ErrExpr<E, T> const &v, Tripple const &o, Tripple const &s)
        : _v{v}, _shape{s}, _offset{o}, _size{_shape.i * _shape.j * _shape.k} {}

    Tripple shape(void) const { return _shape; }

    ull size(void) const { return _size; }

    T sum(void) const { return cj::erray::sum(*this); }
};

template <typename T, typename E>
Slice<T, E> slice(ErrExpr<E, T> const &expr, const ull i0, const ull i1) {
    return Slice<T, E>{expr, Tripple{i0, 0, 0},
                       Tripple{i1 - i0, expr.shape().j, expr.shape().k}};
}

template <typename T, typename E>
Slice<T, E> slice(ErrExpr<E, T> const &expr, const ull i0, const ull i1,
                  const ull i2, const ull i3) {
    return Slice<T, E>{expr, Tripple{i0, i2, 0},
                       Tripple{i1 - i0, i3 - i2, expr.shape().k}};
}

template <typename T, typename E>
Slice<T, E> slice(ErrExpr<E, T> const &expr, const ull i0, const ull i1,
                  const ull i2, const ull i3, const ull i4, const ull i5) {
    assert(i1 > i0 && i3 > i2 && i5 > i4);
    assert(i1 <= expr.shape().i && i3 <= expr.shape().j &&
           i5 <= expr.shape().k);

    return Slice<T, E>{expr, Tripple{i0, i2, i4},
                       Tripple{i1 - i0, i3 - i2, i5 - i4}};
}

#endif  // ERRAY_EXPR_HPP
