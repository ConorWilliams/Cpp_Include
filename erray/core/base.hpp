/**
 * erray_main.hpp
 *
 * The following code (part of the erray module) is Copyright © 2019 C. J.
 * Williams, all rights reserved.
 *
 * Part of the erray module implementing 1, 2 and 3 dimensional arrays using
 * expression templates
 *
 * Part of the core bundle, see errey_core.hpp for full details.
 *
 * This file implements the base class ErrExpr used for all expressions classes
 * to enable expression templates using the curiously recurring template
 * pattern. It also includes the main Erray class and the similar Window
 * class for slicing lvalue Errays.
 *
 */

#ifndef ERRAY_MAIN_HPP
#define ERRAY_MAIN_HPP

// ****************************************************************************
// *               Base class for Errays and Erray expressions                *
// ****************************************************************************

#include <erray/core.hpp>

/**
 * @brief      Macro for easily converting arbitrary functions that take a
 *             scalar as their input to a functions that act on erray
 *             expressions and are evaluated element-wise. To add your convert
 *             your own function just add it using this macro into the ErrExpr
 *             class. The macro automatically makes a wrapper class to wrap the
 *             input function into a functor object.
 *
 * @param      FUNCTION   The function to apply element-wise
 * @param      METHODNAME  The name of the method for accessing this
 *             "Function"Functor
 *
 * @return     { description_of_the_return_value }
 */
#define ELEM_FUNCT_MACRO(FUNCTION, METHODNAME)                         \
                                                                       \
    struct METHODNAME##_FunctorWrapper {                               \
        inline T operator()(T input) const { return FUNCTION(input); } \
        METHODNAME##_FunctorWrapper() = default;                       \
    };                                                                 \
    inline auto METHODNAME(void) const {                               \
        return cj::erray::elem_wise<METHODNAME##_FunctorWrapper>(      \
            static_cast<E const &>(*this));                            \
    }

/**
 * @brief      Base class for all Errays and Erray expressions
 *
 * @tparam     E     Curiously recursive type deduction
 * @tparam     T     Underlying data type in Erray
 */
template <typename E, typename T>
class ErrExpr {
   public:
    T operator()(const ull i = 0, const ull j = 0, const ull k = 0) const {
        return static_cast<E const &>(*this)(i, j, k);
    }

    Tripple shape(void) const { return static_cast<E const &>(*this).shape(); }

    ull size(void) const { return static_cast<E const &>(*this).size(); }

    inline auto slice(const ull i0, const ull i1) {
        return cj::erray::slice(static_cast<E const &>(*this), i0, i1);
    }

    inline auto slice(const ull i0, const ull i1, const ull i2, const ull i3) {
        return cj::erray::slice(static_cast<E const &>(*this), i0, i1, i2, i3);
    }

    inline auto slice(const ull i0, const ull i1, const ull i2, const ull i3,
                      const ull i4, const ull i5) {
        return cj::erray::slice(static_cast<E const &>(*this), i0, i1, i2, i3,
                                i4, i5);
    }

    inline T sum(void) const {
        return cj::erray::sum(static_cast<E const &>(*this));
    }

    inline T max(void) const {
        return cj::erray::max(static_cast<E const &>(*this));
    }

    inline T min(void) const {
        return cj::erray::max(static_cast<E const &>(*this));
    }

    inline auto pow(T const scalar) const {
        return cj::erray::pow(static_cast<E const &>(*this), scalar);
    }

    template <typename K>
    inline auto pow(ErrExpr<K, T> const &err) const {
        return cj::erray::pow(static_cast<E const &>(*this), err);
    }

    ELEM_FUNCT_MACRO(std::cos, cos)
    ELEM_FUNCT_MACRO(std::sin, sin)
    ELEM_FUNCT_MACRO(std::tan, tan)
    ELEM_FUNCT_MACRO(std::abs, abs)
};

// ****************************************************************************
// *                                Erray Class                               *
// ****************************************************************************

/**
 * @brief      Main Erray class
 *
 * @tparam     T     Underlying data type in Erray
 */
template <typename T = double>
class Erray : public ErrExpr<Erray<T>, T> {
   public:
    T *m_elems = nullptr;
    Tripple m_shape{0, 0, 0};

    inline ull to_flat(const ull i = 0, const ull j = 0,
                       const ull k = 0) const {
        ASSERT(i < shape().i && j < shape().j && k < shape().k,
               "() indexing out of bounds");
        return i + m_shape.i * j + m_shape.i * m_shape.j * k;
    }

    inline T operator()(const ull i = 0, const ull j = 0,
                        const ull k = 0) const {
        return m_elems[to_flat(i, j, k)];
    }

    inline T &operator()(const ull i = 0, const ull j = 0, const ull k = 0) {
        return m_elems[to_flat(i, j, k)];
    }

    inline T operator[](const ull index) const {
        ASSERT(index < size(), "[] indexing out of bounds");
        return m_elems[index];
    }

    inline T &operator[](const ull index) {
        ASSERT(index < size(), "[] indexing out of bounds");
        return m_elems[index];
    }

    inline Tripple shape(void) const { return m_shape; }

    inline ull size(void) const { return m_shape.i * m_shape.j * m_shape.k; }

    /**
     * main constructor
     */
    Erray(const ull i = 1, const ull j = 1, const ull k = 1)
        : m_shape(Tripple{i, j, k}) {
        ASSERT(size() != 0, "Can't have any Erray dimension equal to zero");
        dcout("Constructing Erray");
        m_elems = new T[size()];
    }

    /**
     * Copy construct
     */
    Erray(Erray<T> const &err) : m_shape(err.shape()) {
        dcout("Copy constructing Erray");

        m_elems = new T[size()];

        for (ull i = 0; i < shape().i; ++i) {
            for (ull j = 0; j < shape().j; ++j) {
                for (ull k = 0; k < shape().k; ++k) {
                    m_elems[to_flat(i, j, k)] = err(i, j, k);
                }
            }
        }
    }

    /**
     * Move construct
     */
    Erray(Erray<T> &&other) : m_shape(other.shape()) {
        dcout("Move construct Erray");
        m_elems = std::exchange(other.m_elems, nullptr);
    }

    /**
     * Move operator
     */
    Erray<T> &operator=(Erray<T> &&other) noexcept {
        dcout("Assign Erray to move &&ref - move assign");

        if (this != &other) {
            ASSERT(shape() == other.shape(), "Shape check in move assign");
            delete[] m_elems;
            m_elems = std::exchange(other.m_elems, nullptr);
        }
        return *this;
    }

    /**
     * Assignment operator
     */
    Erray<T> &operator=(const Erray<T> &other) {
        dcout("Assign Erray to Erray");

        if (this != &other) {
            ASSERT(shape() == other.shape(), "Shape check in assignment");

            for (ull i = 0; i < shape().i; ++i) {
                for (ull j = 0; j < shape().j; ++j) {
                    for (ull k = 0; k < shape().k; ++k) {
                        m_elems[to_flat(i, j, k)] = other(i, j, k);
                    }
                }
            }
        }
        return *this;
    }

    /**
     * Assignment to scalar
     */
    Erray<T> &operator=(const T scalar) {
        dcout("Assign Erray to scalar");

        for (ull i = 0; i < shape().i; ++i) {
            for (ull j = 0; j < shape().j; ++j) {
                for (ull k = 0; k < shape().k; ++k) {
                    m_elems[to_flat(i, j, k)] = scalar;
                }
            }
        }
        return *this;
    }

    /**
     * Evaluates an expression and sets a new Erray to be equal to the evaluated
     * expression
     */
    template <typename E>
    Erray(ErrExpr<E, T> const &expr) : m_shape(expr.shape()) {
        dcout("Construct Erray from erray expression");

        m_elems = new T[size()];

        for (ull i = 0; i < shape().i; ++i) {
            for (ull j = 0; j < shape().j; ++j) {
                for (ull k = 0; k < shape().k; ++k) {
                    m_elems[to_flat(i, j, k)] = expr(i, j, k);
                }
            }
        }
    }

    /**
     * Evaluates an expression and sets the Erray to be that evaluated
     * expression through use of the = operator
     */
    template <typename E>
    Erray<T> &operator=(ErrExpr<E, T> const &expr) {
        dcout("Assign Erray to erray expression");
        ASSERT(shape() == expr.shape(), "Shape check in expression assign");

        for (ull i = 0; i < shape().i; ++i) {
            for (ull j = 0; j < shape().j; ++j) {
                for (ull k = 0; k < shape().k; ++k) {
                    m_elems[to_flat(i, j, k)] = expr(i, j, k);
                }
            }
        }

        return *this;
    }

    ~Erray() {
        dcout("Delete Erray");
        delete[] m_elems;
        m_elems = nullptr;
    }

    Window<T> window(const ull i0, const ull i1) {
        ASSERT(i1 > i0, "Window bound 1");
        ASSERT(i1 <= shape().i, "Window shape 1");

        return Window<T>{std::move(*this), Tripple{i0, 0, 0},
                         Tripple{i1 - i0, shape().j, shape().k}};
    }

    Window<T> window(const ull i0, const ull i1, const ull i2, const ull i3) {
        ASSERT(i1 > i0 && i3 > i2, "Window bound 2");
        ASSERT(i1 <= shape().i && i3 <= shape().j, "Window shape 1");

        return Window<T>{std::move(*this), Tripple{i0, i2, 0},
                         Tripple{i1 - i0, i3 - i2, shape().k}};
    }

    Window<T> window(const ull i0, const ull i1, const ull i2, const ull i3,
                     const ull i4, const ull i5) {
        ASSERT(i1 > i0 && i3 > i2 && i5 > i4, "Window bound 3");
        ASSERT(i1 <= shape().i && i3 <= shape().j && i5 <= shape().k,
               "Window shape 1");

        return Window<T>{std::move(*this), Tripple{i0, i2, i4},
                         Tripple{i1 - i0, i3 - i2, i5 - i4}};
    }
};

// ****************************************************************************
// *                               Window Class                               *
// ****************************************************************************

/**
 * @brief      Window class for slicing Errays as an rvalue that can be assigned
 *             to, make windows using the window() method.
 *
 * @tparam     T     Underlying data type in Erray
 */
template <typename T>
class Window : public ErrExpr<Window<T>, T> {
   public:
    T *m_elems = nullptr;
    Tripple m_shape{0, 0, 0};
    Tripple m_offset{0, 0, 0};
    Tripple m_modded_shape{0, 0, 0};

    inline ull to_flat(const ull i = 0, const ull j = 0,
                       const ull k = 0) const {
        return i + m_offset.i + m_shape.i * (j + m_offset.j) +
               m_shape.i * m_shape.j * (k + m_offset.k);
    }

    inline T operator()(const ull i = 0, const ull j = 0,
                        const ull k = 0) const {
        return m_elems[to_flat(i, j, k)];
    }

    inline T &operator()(const ull i = 0, const ull j = 0, const ull k = 0) {
        return m_elems[to_flat(i, j, k)];
    }

    inline Tripple shape(void) const { return m_modded_shape; }

    inline ull size(void) const {
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
        cout << "move construct 2" << endl;
        // m_elems = other.m_elems;
    }

    Window(Erray<T> &&other, Tripple const &o, Tripple const &w)
        : m_shape(other.shape()), m_offset{o}, m_modded_shape{w} {
        cout << "move construct 3" << endl;
        m_elems = other.m_elems;
    }

    Window(Window<T> &&other, Tripple const &o, Tripple const &w)
        : m_shape{other.m_shape}, m_offset{o}, m_modded_shape{w} {
        cout << "move construct 4" << endl;
        m_elems = other.m_elems;
    }

    /**
     * Assignment to scalar
     */
    Window<T> &operator=(const T scalar) {
        cout << "win scalar assign" << endl;
        for (ull i = 0; i < shape().i; ++i) {
            for (ull j = 0; j < shape().j; ++j) {
                for (ull k = 0; k < shape().k; ++k) {
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
        dcout("win copy assign");
        if (this != &other) {
            assert(shape() == other.shape());

            for (ull i = 0; i < shape().i; ++i) {
                for (ull j = 0; j < shape().j; ++j) {
                    for (ull k = 0; k < shape().k; ++k) {
                        m_elems[to_flat(i, j, k)] = other(i, j, k);
                    }
                }
            }
        }
        return *this;
    }

    /**
     * Evaluates an expression and sets the Erray to be that evaluated
     * expression through use of the = operator
     */
    template <typename E>
    Window<T> &operator=(ErrExpr<E, T> const &expr) {
        dcout("win expr assign");
        assert(shape() == expr.shape());

        for (ull i = 0; i < shape().i; ++i) {
            for (ull j = 0; j < shape().j; ++j) {
                for (ull k = 0; k < shape().k; ++k) {
                    m_elems[to_flat(i, j, k)] = expr(i, j, k);
                }
            }
        }

        return *this;
    }

    ~Window() { cout << "win del" << endl; }

    Window<T> window(const ull i0, const ull i1) {
        assert(i1 > i0);
        assert(i1 <= shape().i);

        return Window<T>{std::move(*this), Tripple{i0 + m_offset.i, 0, 0},
                         Tripple{i1 - i0, shape().j, shape().k}};
    }

    Window<T> window(const ull i0, const ull i1, const ull i2, const ull i3) {
        assert(i1 > i0 && i3 > i2);
        assert(i1 <= shape().i && i3 <= shape().j);

        return Window<T>{std::move(*this),
                         Tripple{i0 + m_offset.i, i2 + m_offset.j, 0},
                         Tripple{i1 - i0, i3 - i2, shape().k}};
    }

    Window<T> window(const ull i0, const ull i1, const ull i2, const ull i3,
                     const ull i4, const ull i5) {
        assert(i1 > i0 && i3 > i2 && i5 > i4);
        assert(i1 <= shape().i && i3 <= shape().j && i5 <= shape().k);

        return Window<T>{
            std::move(*this),
            Tripple{i0 + m_offset.i, i2 + m_offset.j, i4 + m_offset.k},
            Tripple{i1 - i0, i3 - i2, i5 - i4}};
    }
};

#endif  // ERRAY_MAIN_HPP
