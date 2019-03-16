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
 * xrray/core/funct.hpp
 *
 * Part of the xrray module implementing 1, 2 and 3 dimensional arrays using
 * expression templates.
 *
 * Part of the core bundle, see errey_core.hpp for full details.
 *
 * This file contains functions that: take xrray expressions as their input and
 * return non xrray expressions or functions that take non xrray expressions as
 * there input and return xrray expressions.
 */

#ifndef XRRAY_FUNCT_HPP
#define XRRAY_FUNCT_HPP

#include <xrray/core.hpp>

// ****************************************************************************
// *                            xrray expression in                           *
// ****************************************************************************

namespace xrray {

/**
 * @brief      Sums an Xrray Expression
 *
 * @param      expr  input expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Xrray type
 *
 * @return     sum of all elements
 */
template <typename E, typename T>
T sum(ErrExpr<E, T> const &expr) {
    T sum = expr(0, 0, 0);

    for (index_t i = 0; i < expr.shape().i; ++i) {
        for (index_t j = 0; j < expr.shape().j; ++j) {
            for (index_t k = 0; k < expr.shape().k; ++k) {
                sum += expr(i, j, k);
            }
        }
    }

    sum -= expr(0, 0, 0);

    return sum;
}

/**
 * @brief      returns the maximum element in an expression
 *
 * @param      expr  The expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Xrray type
 *
 * @return     maximum element in expression
 */
template <typename E, typename T>
T max(ErrExpr<E, T> const &expr) {
    T max = expr(0, 0, 0);

    for (index_t i = 0; i < expr.shape().i; ++i) {
        for (index_t j = 0; j < expr.shape().j; ++j) {
            for (index_t k = 0; k < expr.shape().k; ++k) {
                if (expr(i, j, k) > max) {
                    max = expr(i, j, k);
                }
            }
        }
    }

    return max;
}

/**
 * @brief      Return the minimum element in an expression
 *
 * @param      expr  The expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Xrray type
 *
 * @return     { description_of_the_return_value }
 */
template <typename E, typename T>
T min(ErrExpr<E, T> const &expr) {
    T min = expr(0, 0, 0);

    for (index_t i = 0; i < expr.shape().i; ++i) {
        for (index_t j = 0; j < expr.shape().j; ++j) {
            for (index_t k = 0; k < expr.shape().k; ++k) {
                if (expr(i, j, k) < min) {
                    min = expr(i, j, k);
                }
            }
        }
    }

    return min;
}

/*----------------------------------------------------------------------------*/

/**
 * @brief      swaps the elements in two window views accepts rvalue
 *
 * @param      left   The left window
 * @param      right  The right window
 *
 * @tparam     T      underlying Xrray type
 */
template <typename T>
void swap(Window<T> &&left, Window<T> &&right) {
    xrray::Xrray<T> tmp{std::move(left)};
    left = right;
    right = tmp;
}

/**
 * @brief      swaps the elements in two window views
 *
 * @param      left   The left window
 * @param      right  The right window
 *
 * @tparam     T      underlying Xrray type
 */
template <typename T>
void swap(Window<T> &left, Window<T> &right) {
    xrray::Xrray<T> tmp{std::move(left)};
    left = right;
    right = tmp;
}

/**
 * @brief      print any Xrray expression
 *
 * @param[in]  err   input expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Xrray type
 */
template <typename T, typename E>
void print(const ErrExpr<E, T> &err) {
    cout << "[";
    for (index_t k = 0; k < err.shape().k; ++k) {
        if (k == 0) {
            cout << "(";
        } else {
            cout << " (";
        }
        for (index_t i = 0; i < err.shape().i; ++i) {
            if (i == 0) {
                cout << "{ ";
            } else {
                cout << "  { ";
            }
            for (index_t j = 0; j < err.shape().j; ++j) {
                cout << err(i, j, k) << " ";
            }
            if (i == err.shape().i - 1) {
                cout << "}";
            } else {
                cout << "}" << endl;
            }
        }
        if (k == err.shape().k - 1) {
            cout << ")";

        } else {
            cout << ")" << endl;
            cout << endl;
        }
    }
    cout << "]";
}

/**
 * @brief      Overloading std::cout to print Xrray expressions
 *
 * @param      stream  the stream
 * @param[in]  err     the input expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Xrray type
 *
 * @return     the stream
 */
template <typename E, typename T>
std::ostream &operator<<(std::ostream &stream,
                         const xrray::ErrExpr<E, T> &err) {
    xrray::print(err);
    return stream;
}

// ****************************************************************************
// *                        xrray expression in and out                       *
// ****************************************************************************

/**
 * @brief      returns an empty Xrray
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 * @param[in]  k     dimension in k direction
 *
 * @tparam     T     underlying Xrray type
 *
 * @return     an empty Xrray
 */
template <typename T>
Xrray<T> empty(const index_t i, const index_t j, const index_t k) {
    return Xrray<T>{i, j, k};
}

/**
 * @brief      returns an Xrray of ones
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 * @param[in]  k     dimension in k direction
 *
 * @tparam     T     underlying Xrray type
 *
 * @return     an Xrray findex_t of ones
 */
template <typename T>
Xrray<T> ones(const index_t i, const index_t j, const index_t k) {
    Xrray<T> out{i, j, k};

    for (index_t _k = 0; _k < k; ++_k) {
        for (index_t _j = 0; _j < j; ++_j) {
            for (index_t _i = 0; _i < i; ++_i) {
                out(_i, _j, _k) = 1;
            }
        }
    }

    return out;
}

/**
 * @brief      returns an Xrray of zeros
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 * @param[in]  k     dimension in k direction
 *
 * @tparam     T     underlying Xrray type
 *
 * @return     an Xrray findex_t of zeros
 */
template <typename T>
Xrray<T> zeros(const index_t i, const index_t j, const index_t k) {
    Xrray<T> out{i, j, k};

    for (index_t _k = 0; _k < k; ++_k) {
        for (index_t _j = 0; _j < j; ++_j) {
            for (index_t _i = 0; _i < i; ++_i) {
                out(_i, _j, _k) = 0;
            }
        }
    }

    return out;
}

/**
 * @brief      returns an 2D Xrray with e(i, j) i == j ? 1 : 0
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 *
 * @tparam     T     underlying Xrray type
 *
 * @return     2D identity Xrray
 */
template <typename T>
Xrray<T> eye(const index_t i, index_t j) {
    if (j == 0) {
        j = i;
    }
    auto out = zeros<T>(i, j, 1);

    index_t range = i < j ? i : j;

    for (index_t index = 0; index < range; ++index) {
        out(index, index, 0) = 1;
    }

    return out;
}

/**
 * @brief      returns an Xrray with N elements linearly spaced between first
 *             and last
 *
 * @param[in]  first  the first element
 * @param[in]  last   the last element
 * @param[in]  N      number of elements
 *
 * @tparam     T      underlying Xrray type
 *
 * @return     linearly spaced Xrray of values
 */
template <typename T>
Xrray<T> linspace(const index_t first, const index_t last, const index_t N) {
    Xrray<T> out{N, 1, 1};

    T step = static_cast<T>(last - first) / static_cast<T>(N - 1);
    index_t count = 0;

    Triple tmp{out.shape()};

    for (index_t k = 0; k < tmp.k; ++k) {
        for (index_t j = 0; j < tmp.j; ++j) {
            for (index_t i = 0; i < tmp.i; ++i) {
                out(i, j, k) = first + step * count;
                ++count;
            }
        }
    }
    return out;
}

/**
 * @brief      returns an Xrray with of given shape with each element @step
 *             apart starting at @start
 *
 * @param[in]  i      dimension in i direction
 * @param[in]  j      dimension in j direction
 * @param[in]  k      dimension in k direction
 *
 * @tparam     T      underlying Xrray type
 * @tparam     step   first element, default = 0
 * @tparam     start  difference between elements, default = 1
 *
 * @return     Xrray with increasing elements
 */
template <typename T, index_t step, index_t start>
Xrray<T> enumerate(const index_t i, const index_t j, const index_t k) {
    Xrray<T> out{i, j, k};
    index_t count = 0;

    Triple tmp{out.shape()};

    for (index_t k = 0; k < tmp.k; ++k) {
        for (index_t j = 0; j < tmp.j; ++j) {
            for (index_t i = 0; i < tmp.i; ++i) {
                out(i, j, k) = start + step * count;
                ++count;
            }
        }
    }
    return out;
}

}  // namespace xrray

#endif  // XRRAY_FUNCT_HPP
