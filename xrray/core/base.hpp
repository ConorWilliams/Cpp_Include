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
 * xrray/core/main.hpp
 *
 * Part of the xrray module implementing 1, 2 and 3 dimensional arrays using
 * expression templates
 *
 * Part of the core bundle, see errey_core.hpp for full details.
 *
 * This file implements the base class ErrExpr used for all expressions classes
 * to enable expression templates using the curiously recurring template
 * pattern. It also includes the main Xrray class and the similar Window
 * class for slicing lvalue Xrrays.
 */

#ifndef XRRAY_BASE_HPP
#define XRRAY_BASE_HPP

#include <xrray/core.hpp>

namespace xrray {

// ****************************************************************************
// *               Base class for Xrrays and Xrray expressions                *
// ****************************************************************************

/**
 * get class definitions from plugin.hpp
 */
#define XRRAY_EXTERNAL
#include <xrray/core/plugin.hpp>

/**
 * @brief      Base class for all Xrrays and Xrray expressions
 *
 * @tparam     E     Curiously recursive type deduction
 * @tparam     T     Underlying data type in Xrray
 */
template <typename E, typename T>
class ErrExpr {
   public:
    T operator()(const index_t i = 0, const index_t j = 0,
                 const index_t k = 0) const {
        return static_cast<E const &>(*this)(i, j, k);
    }

    Triple shape(void) const { return static_cast<E const &>(*this).shape(); }

    index_t size(void) const { return static_cast<E const &>(*this).size(); }

    inline auto slice(const index_t i0, const index_t i1) {
        ASSERT(i1 > i0, "Slice indices out of order");
        ASSERT(i1 <= shape().i, "Slice too big");

        return Slice<T, E>{static_cast<E const &>(*this), Triple{i0, 0, 0},
                           Triple{i1 - i0, shape().j, shape().k}};
    }

    inline auto slice(const index_t i0, const index_t i1, const index_t i2,
                      const index_t i3) {
        ASSERT(i1 > i0 && i3 > i2, "Slice indices out of order");
        ASSERT(i1 <= shape().i && i3 <= shape().j, "Slice too big");

        return Slice<T, E>{static_cast<E const &>(*this), Triple{i0, i2, 0},
                           Triple{i1 - i0, i3 - i2, shape().k}};
    }

    inline auto slice(const index_t i0, const index_t i1, const index_t i2,
                      const index_t i3, const index_t i4, const index_t i5) {
        ASSERT(i1 > i0 && i3 > i2 && i5 > i4, "Slice indices out of order");
        ASSERT(i1 <= shape().i && i3 <= shape().j && i5 <= shape().k,
               "Slice too big");

        return Slice<T, E>{static_cast<E const &>(*this), Triple{i0, i2, i4},
                           Triple{i1 - i0, i3 - i2, i5 - i4}};
    }

    inline auto row(index_t i) { return slice(i, i + 1); }
    inline auto col(index_t i) { return slice(0, shape().i, i, i + 1); }
    inline auto dep(index_t i) {
        return slice(0, shape().i, 0, shape().j, i, i + 1);
    }

    inline auto pow(T const scalar) const {
        return xrray::pow(static_cast<E const &>(*this), scalar);
    }

    template <typename K>
    inline auto pow(ErrExpr<K, T> const &err) const {
        return xrray::pow(static_cast<E const &>(*this), err);
    }

    inline auto t12(void) const {
        return xrray::Transpose12{static_cast<E const &>(*this)};
    }

    inline auto t23(void) const {
        return xrray::Transpose23{static_cast<E const &>(*this)};
    }

    inline auto t13(void) const {
        return xrray::Transpose13{static_cast<E const &>(*this)};
    }

    template <typename K>
    inline auto mm(ErrExpr<K, T> const &err) {
        return xrray::XrrayMM<E, K, T>{static_cast<E const &>(*this), err};
    }

/**
 * get method definitions from plugin.hpp
 */
#define XRRAY_INTERNAL
#include <xrray/core/plugin.hpp>

    inline T sum(void) const {
        return xrray::sum(static_cast<E const &>(*this));
    }

    inline T max(void) const {
        return xrray::max(static_cast<E const &>(*this));
    }

    inline T min(void) const {
        return xrray::max(static_cast<E const &>(*this));
    }
};

// ****************************************************************************
// *                                Xrray Class                               *
// ****************************************************************************

/**
 * @brief      Main Xrray class
 *
 * @tparam     T     Underlying data type in Xrray
 */
template <typename T = double>
class Xrray : public ErrExpr<Xrray<T>, T> {
   private:
    inline index_t to_flat(const index_t i = 0, const index_t j = 0,
                           const index_t k = 0) const {
        ASSERT(i < shape().i && j < shape().j && k < shape().k,
               "() indexing out of bounds");
        return i + m_shape.i * j + m_shape.i * m_shape.j * k;
    }

   public:
    T *m_elems = nullptr;
    Triple m_shape{0, 0, 0};

    inline T operator()(const index_t i = 0, const index_t j = 0,
                        const index_t k = 0) const {
        return m_elems[to_flat(i, j, k)];
    }

    inline T &operator()(const index_t i = 0, const index_t j = 0,
                         const index_t k = 0) {
        return m_elems[to_flat(i, j, k)];
    }

    inline T operator[](const index_t index) const {
        ASSERT(index < size(), "[] indexing out of bounds");
        return m_elems[index];
    }

    inline T &operator[](const index_t index) {
        ASSERT(index < size(), "[] indexing out of bounds");
        return m_elems[index];
    }

    inline Triple shape(void) const { return m_shape; }

    inline index_t size(void) const {
        return m_shape.i * m_shape.j * m_shape.k;
    }

    /**
     * @brief      Methods to reshape Xrray to given shape
     *
     * @param[in]  i     new shape in i dir
     * @param[in]  j     new shape in j dir
     * @param[in]  k     new shape in k dir
     */
    void reshape(const index_t i, const index_t j = 1, const index_t k = 1) {
        ASSERT(i * j * k == size(), "Reshape size must match");
        m_shape = Triple{i, j, k};
    }

    void reshape(void) { m_shape = Triple{size(), 1, 1}; }

    /**
     * @brief      Constructs the object.
     *
     * @param[in]  i     size in i dir
     * @param[in]  j     size in j dir
     * @param[in]  k     size in k dir
     */
    Xrray(const index_t i = 1, const index_t j = 1, const index_t k = 1)
        : m_shape(Triple{i, j, k}) {
        ASSERT(size() != 0, "Can't have any Xrray dimension equal to zero");
        dcout("Constructing Xrray");
        m_elems = new T[size()];
    }

    /**
     * @brief      Copy constructs the object from Xrray.
     *
     * @param      err   The Xrray to copy
     */
    Xrray(Xrray<T> const &err) : m_shape(err.shape()) {
        dcout("Copy constructing Xrray");

        m_elems = new T[size()];

        Triple tmp{shape()};

        for (index_t k = 0; k < tmp.k; ++k) {
            for (index_t j = 0; j < tmp.j; ++j) {
                for (index_t i = 0; i < tmp.i; ++i) {
                    m_elems[to_flat(i, j, k)] = err(i, j, k);
                }
            }
        }
    }

    /**
     * @brief      Move construct the Xrray
     *
     * @param      other  the other Xrray to move from
     */
    Xrray(Xrray<T> &&other) : m_shape(other.shape()) {
        dcout("Move construct Xrray");
        m_elems = std::exchange(other.m_elems, nullptr);
    }

    /**
     * @brief      Move assign Xrray to Xrray
     *
     * @param      other  The other Xrray to move from
     *
     * @return     an Xrray
     */
    Xrray<T> &operator=(Xrray<T> &&other) noexcept {
        dcout("Assign Xrray to move &&ref - move assign");

        if (this != &other) {
            ASSERT(shape() == other.shape(), "Shape check in move assign");
            delete[] m_elems;
            m_elems = std::exchange(other.m_elems, nullptr);
        }
        return *this;
    }

    /**
     * @brief      Assign Xrray to Xrray
     *
     * @param      other  The other Xrray
     *
     * @return     deep copy of the other Xrray
     */
    Xrray<T> &operator=(const Xrray<T> &other) {
        dcout("Assign Xrray to Xrray");

        if (this != &other) {
            ASSERT(shape() == other.shape(), "Shape check in assignment");

            Triple tmp{shape()};

            for (index_t k = 0; k < tmp.k; ++k) {
                for (index_t j = 0; j < tmp.j; ++j) {
                    for (index_t i = 0; i < tmp.i; ++i) {
                        m_elems[to_flat(i, j, k)] = other(i, j, k);
                    }
                }
            }
        }
        return *this;
    }

    /**
     * @brief      Assign every element in Xrray to be equal to a scalar
     *
     * @param[in]  scalar  The scalar to assign to every element
     *
     * @return     Xrray with all values assigned to scalar
     */
    Xrray<T> &operator=(const T scalar) {
        dcout("Assign Xrray to scalar");

        Triple tmp{shape()};

        for (index_t k = 0; k < tmp.k; ++k) {
            for (index_t j = 0; j < tmp.j; ++j) {
                for (index_t i = 0; i < tmp.i; ++i) {
                    m_elems[to_flat(i, j, k)] = scalar;
                }
            }
        }
        return *this;
    }

    /**
     * @brief      Construct an Xrray from an xrray expression, lazily evaluates
     *             expression during this phase
     *
     * @param      expr  The xrray expression
     *
     * @return     evaluated expression
     */
    template <typename E>
    Xrray(ErrExpr<E, T> const &expr) : m_shape(expr.shape()) {
        dcout("Construct Xrray from xrray expression");

        m_elems = new T[size()];

        Triple tmp{shape()};

        for (index_t k = 0; k < tmp.k; ++k) {
            for (index_t j = 0; j < tmp.j; ++j) {
                for (index_t i = 0; i < tmp.i; ++i) {
                    m_elems[to_flat(i, j, k)] = expr(i, j, k);
                }
            }
        }
    }

    /**
     * @brief      Assign the Xrray to xrray expression, lazily evaluates
     *             expression during this phase
     *
     * @param      expr  The xrray expression
     *
     * @return     evaluated expression
     */
    template <typename E>
    Xrray<T> &operator=(ErrExpr<E, T> const &expr) {
        dcout("Assign Xrray to xrray expression");
        ASSERT(shape() == expr.shape(), "Shape check in expression assign");

        Triple tmp{shape()};

        for (index_t k = 0; k < tmp.k; ++k) {
            for (index_t j = 0; j < tmp.j; ++j) {
                for (index_t i = 0; i < tmp.i; ++i) {
                    m_elems[to_flat(i, j, k)] = expr(i, j, k);
                }
            }
        }
        return *this;
    }

    ~Xrray() {
        dcout("Delete Xrray");
        delete[] m_elems;
        m_elems = nullptr;
    }

    /**
     * @brief      Set of methods to construct a window into the Xrray
     *
     * @param[in]  i0    first element along i axis
     * @param[in]  i1    last element along i axis + 1
     * @param[in]  i2    first element along j axis
     * @param[in]  i3    last element along j axis + 1
     * @param[in]  i4    first element along k axis
     * @param[in]  i5    last element along k axis + 1
     *
     * @return     Window to xrray
     */
    inline Window<T> slice(const index_t i0, const index_t i1) {
        ASSERT(i1 > i0, "Window bound 1");
        ASSERT(i1 <= shape().i, "Window shape 1");

        return Window<T>{std::move(*this), Triple{i0, 0, 0},
                         Triple{i1 - i0, shape().j, shape().k}};
    }

    inline Window<T> slice(const index_t i0, const index_t i1, const index_t i2,
                           const index_t i3) {
        ASSERT(i1 > i0 && i3 > i2, "Window bound 2");
        ASSERT(i1 <= shape().i && i3 <= shape().j, "Window shape 1");

        return Window<T>{std::move(*this), Triple{i0, i2, 0},
                         Triple{i1 - i0, i3 - i2, shape().k}};
    }

    inline Window<T> slice(const index_t i0, const index_t i1, const index_t i2,
                           const index_t i3, const index_t i4,
                           const index_t i5) {
        ASSERT(i1 > i0 && i3 > i2 && i5 > i4, "Window bound 3");
        ASSERT(i1 <= shape().i && i3 <= shape().j && i5 <= shape().k,
               "Window shape 1");

        return Window<T>{std::move(*this), Triple{i0, i2, i4},
                         Triple{i1 - i0, i3 - i2, i5 - i4}};
    }

    /**
     * @brief      Short-hands for getting slices of rows, cols and deps
     */
    inline Window<T> row(index_t i) { return slice(i, i + 1); }
    inline Window<T> col(index_t i) { return slice(0, shape().i, i, i + 1); }
    inline Window<T> dep(index_t i) {
        return slice(0, shape().i, 0, shape().j, i, i + 1);
    }
};

// ****************************************************************************
// *                               Window Class                               *
// ****************************************************************************

/**
 * @brief      Window class for slicing Xrrays as an rvalue that can be assigned
 *             to, make windows using the window() method.
 *
 * @tparam     T     Underlying data type in Xrray
 */
template <typename T>
class Window : public ErrExpr<Window<T>, T> {
   public:
    T *const m_elems = nullptr;
    Triple m_shape{0, 0, 0};
    const Triple m_offset{0, 0, 0};
    const Triple m_modded_shape{0, 0, 0};

    inline index_t to_flat(const index_t i = 0, const index_t j = 0,
                           const index_t k = 0) const {
        return i + m_offset.i + m_shape.i * (j + m_offset.j) +
               m_shape.i * m_shape.j * (k + m_offset.k);
    }

    inline T operator()(const index_t i = 0, const index_t j = 0,
                        const index_t k = 0) const {
        return m_elems[to_flat(i, j, k)];
    }

    inline T &operator()(const index_t i = 0, const index_t j = 0,
                         const index_t k = 0) {
        return m_elems[to_flat(i, j, k)];
    }

    inline Triple shape(void) const { return m_modded_shape; }

    inline index_t size(void) const {
        return m_modded_shape.i * m_modded_shape.j * m_modded_shape.k;
    }

    /**
     * Move construct the window class
     */
    Window(Window<T> &&other)
        : m_elems{other.m_elems},
          m_shape{other.m_shape},
          m_offset{other.m_offset},
          m_modded_shape{other.m_modded_shape} {
        dcout("Move copy construct Window from Window");
    }

    Window(Window<T> &other)
        : m_elems{other.m_elems},
          m_shape{other.m_shape},
          m_offset{other.m_offset},
          m_modded_shape{other.m_modded_shape} {
        dcout("Copy construct Window from Window");
    }

    Window(Xrray<T> &&other, Triple const &o, Triple const &w)
        : m_elems{other.m_elems},
          m_shape{other.shape()},
          m_offset{o},
          m_modded_shape{w} {
        dcout("Move construct Window from Xrray");
    }

    /**
     * Assignment to scalar
     */
    Window<T> &operator=(const T scalar) {
        dcout("Assign Window to scalar");

        Triple tmp{shape()};

        for (index_t k = 0; k < tmp.k; ++k) {
            for (index_t j = 0; j < tmp.j; ++j) {
                for (index_t i = 0; i < tmp.i; ++i) {
                    m_elems[to_flat(i, j, k)] = scalar;
                }
            }
        }
        return *this;
    }

    /**
     * Assignment operator
     */
    Window<T> &operator=(const Window<T> &other) {
        dcout("Assign Window to Window");

        if (this != &other) {
            ASSERT(shape() == other.shape(),
                   "Shape mismatch in Window assign to Window");

            Triple tmp{shape()};

            for (index_t k = 0; k < tmp.k; ++k) {
                for (index_t j = 0; j < tmp.j; ++j) {
                    for (index_t i = 0; i < tmp.i; ++i) {
                        m_elems[to_flat(i, j, k)] = other(i, j, k);
                    }
                }
            }
        }
        return *this;
    }

    /**
     * Evaluates an expression and sets the Xrray to be that evaluated
     * expression through use of the = operator
     */
    template <typename E>
    Window<T> &operator=(ErrExpr<E, T> const &expr) {
        dcout("Assign Window to xrray expression");
        ASSERT(shape() == expr.shape(),
               "Shape mismatch in Window assign to expression");

        Triple tmp{shape()};

        for (index_t k = 0; k < tmp.k; ++k) {
            for (index_t j = 0; j < tmp.j; ++j) {
                for (index_t i = 0; i < tmp.i; ++i) {
                    m_elems[to_flat(i, j, k)] = expr(i, j, k);
                }
            }
        }
        return *this;
    }
};

}  // namespace xrray

#endif  // XRRAY_BASE_HPP

/**
 * Depreciated window().window() code

      Window<T> slice(const index_t i0, const index_t i1) {
        assert(i1 > i0);
        assert(i1 <= shape().i);

        return Window<T>{std::move(*this), Triple{i0 + m_offset.i, 0, 0},
                         Triple{i1 - i0, shape().j, shape().k}};
    }

    Window<T> slice(const index_t i0, const index_t i1, const index_t i2,
                    const index_t i3) {
        assert(i1 > i0 && i3 > i2);
        assert(i1 <= shape().i && i3 <= shape().j);

        return Window<T>{std::move(*this),
                         Triple{i0 + m_offset.i, i2 + m_offset.j, 0},
                         Triple{i1 - i0, i3 - i2, shape().k}};
    }

    Window<T> slice(const index_t i0, const index_t i1, const index_t i2,
                    const index_t i3, const index_t i4, const index_t i5) {
        assert(i1 > i0 && i3 > i2 && i5 > i4);
        assert(i1 <= shape().i && i3 <= shape().j && i5 <= shape().k);

        return Window<T>{
            std::move(*this),
            Triple{i0 + m_offset.i, i2 + m_offset.j, i4 + m_offset.k},
            Triple{i1 - i0, i3 - i2, i5 - i4}};
    }
        Window(Window<T> &&other, Triple const &o, Triple const &w)
        : m_elems{other.m_elems},
          m_shape{other.m_shape},
          m_offset{o},
          m_modded_shape{w} {
        cout << "Construct Window from Window" << endl;
        dcout();
    }
 */
