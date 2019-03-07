/**
 * erray_funct.hpp
 *
 * The following code (part of the erray module) is Copyright © 2019 C. J.
 * Williams, all rights reserved.
 *
 * Part of the erray module implementing 1, 2 and 3 dimensional arrays using
 * expression templates.
 *
 * Part of the core bundle, see errey_core.hpp for full details.
 *
 * This file contains functions that take erray expressions as their input and
 * return non erray expressions or functions that take non erray expressions as
 * there input and return erray expressions.
 *
 */

#ifndef ERRAY_FUNCT_HPP
#define ERRAY_FUNCT_HPP

// ****************************************************************************
// *                            erray expression in                           *
// ****************************************************************************

#include <erray/core.hpp>

/**
 * @brief      Sums an Erray Expression
 *
 * @param      expr  input expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Erray type
 *
 * @return     sum of all elements
 */
template <typename E, typename T>
T sum(ErrExpr<E, T> const &expr) {
    T sum = expr(0, 0, 0);

    for (ull i = 0; i < expr.shape().i; ++i) {
        for (ull j = 0; j < expr.shape().j; ++j) {
            for (ull k = 0; k < expr.shape().k; ++k) {
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
 * @tparam     T     underlying Erray type
 *
 * @return     maximum element in expression
 */
template <typename E, typename T>
T max(ErrExpr<E, T> const &expr) {
    T max = expr(0, 0, 0);

    for (ull i = 0; i < expr.shape().i; ++i) {
        for (ull j = 0; j < expr.shape().j; ++j) {
            for (ull k = 0; k < expr.shape().k; ++k) {
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
 * @tparam     T     underlying Erray type
 *
 * @return     { description_of_the_return_value }
 */
template <typename E, typename T>
T min(ErrExpr<E, T> const &expr) {
    T min = expr(0, 0, 0);

    for (ull i = 0; i < expr.shape().i; ++i) {
        for (ull j = 0; j < expr.shape().j; ++j) {
            for (ull k = 0; k < expr.shape().k; ++k) {
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
 * @tparam     T      underlying Erray type
 */
template <typename T>
void swap(Window<T> &&left, Window<T> &&right) {
    cj::erray::Erray<T> tmp{left};
    left = right;
    right = tmp;
}

/**
 * @brief      swaps the elements in two window views
 *
 * @param      left   The left window
 * @param      right  The right window
 *
 * @tparam     T      underlying Erray type
 */
template <typename T>
void swap(Window<T> &left, Window<T> &right) {
    cj::erray::Erray<T> tmp{left};
    left = right;
    right = tmp;
}

/**
 * @brief      print any Erray expression
 *
 * @param[in]  err   input expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Erray type
 */
template <typename T, typename E>
void print(const ErrExpr<E, T> &err) {
    cout << "[";
    for (ull k = 0; k < err.shape().k; ++k) {
        if (k == 0) {
            cout << "(";
        } else {
            cout << " (";
        }
        for (ull i = 0; i < err.shape().i; ++i) {
            if (i == 0) {
                cout << "{ ";
            } else {
                cout << "  { ";
            }
            for (ull j = 0; j < err.shape().j; ++j) {
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
 * @brief      Overloading std::cout to print Erray expressions
 *
 * @param      stream  the stream
 * @param[in]  err     the input expression
 *
 * @tparam     E     curiously  recursive type
 * @tparam     T     underlying Erray type
 *
 * @return     the stream
 */
template <typename E, typename T>
std::ostream &operator<<(std::ostream &stream,
                         const cj::erray::ErrExpr<E, T> &err) {
    cj::erray::print(err);
    return stream;
}

// ****************************************************************************
// *                           erray expression out                           *
// ****************************************************************************

/**
 * @brief      returns an empty Erray
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 * @param[in]  k     dimension in k direction
 *
 * @tparam     T     underlying Erray type
 *
 * @return     an empty Erray
 */
template <typename T>
Erray<T> empty(const ull i, const ull j, const ull k) {
    return Erray<T>{i, j, k};
}

/**
 * @brief      returns an Erray of ones
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 * @param[in]  k     dimension in k direction
 *
 * @tparam     T     underlying Erray type
 *
 * @return     an Erray full of ones
 */
template <typename T>
Erray<T> ones(const ull i, const ull j, const ull k) {
    Erray<T> out{i, j, k};

    for (ull _k = 0; _k < k; ++_k) {
        for (ull _j = 0; _j < j; ++_j) {
            for (ull _i = 0; _i < i; ++_i) {
                out(_i, _j, _k) = 1;
            }
        }
    }

    return out;
}

/**
 * @brief      returns an Erray of zeros
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 * @param[in]  k     dimension in k direction
 *
 * @tparam     T     underlying Erray type
 *
 * @return     an Erray full of zeros
 */
template <typename T>
Erray<T> zeros(const ull i, const ull j, const ull k) {
    Erray<T> out{i, j, k};

    for (ull _k = 0; _k < k; ++_k) {
        for (ull _j = 0; _j < j; ++_j) {
            for (ull _i = 0; _i < i; ++_i) {
                out(_i, _j, _k) = 0;
            }
        }
    }

    return out;
}

/**
 * @brief      returns an 2D Erray with e(i, j) i == j ? 1 : 0
 *
 * @param[in]  i     dimension in i direction
 * @param[in]  j     dimension in j direction
 *
 * @tparam     T     underlying Erray type
 *
 * @return     2D identity Erray
 */
template <typename T>
Erray<T> identity(const ull i, ull j) {
    if (j == 0) {
        j = i;
    }
    auto out = zeros<T>(i, j, 1);

    ull range = i < j ? i : j;

    for (ull index = 0; index < range; ++index) {
        out(index, index, 0) = 1;
    }

    return out;
}

/**
 * @brief      returns an Erray with N elements linearly spaced between first
 *             and last
 *
 * @param[in]  first  the first element
 * @param[in]  last   the last element
 * @param[in]  N      number of elements
 *
 * @tparam     T      underlying Erray type
 *
 * @return     linearly spaced Erray of values
 */
template <typename T>
Erray<T> linspace(const ull first, const ull last, const ull N) {
    Erray<T> out{N, 1, 1};

    T step = static_cast<T>(last - first) / static_cast<T>(N - 1);
    ull count = 0;

    for (ull k = 0; k < out.shape().k; ++k) {
        for (ull j = 0; j < out.shape().j; ++j) {
            for (ull i = 0; i < out.shape().i; ++i) {
                out(i, j, k) = first + step * count;
                ++count;
            }
        }
    }
    return out;
}

/**
 * @brief      returns an Erray with of given shape with each element @step
 *             apart starting at @start
 *
 * @param[in]  i      dimension in i direction
 * @param[in]  j      dimension in j direction
 * @param[in]  k      dimension in k direction
 *
 * @tparam     T      underlying Erray type
 * @tparam     step   first element, default = 0
 * @tparam     start  difference between elements, default = 1
 *
 * @return     { description_of_the_return_value }
 */
template <typename T, ull step, ull start>
Erray<T> enumerate(const ull i, const ull j, const ull k) {
    Erray<T> out{i, j, k};
    ull count = 0;

    for (ull k = 0; k < out.shape().k; ++k) {
        for (ull j = 0; j < out.shape().j; ++j) {
            for (ull i = 0; i < out.shape().i; ++i) {
                out(i, j, k) = start + step * count;
                ++count;
            }
        }
    }
    return out;
}

#endif  // ERRAY_FUNCT_HPP
