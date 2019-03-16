/**
 * @brief      Macro for easily converting arbitrary functions that take a
 *             scalar as their input to a functions that act on xrray
 *             expressions and are evaluated element-wise. To add your convert
 *             your own function just add it using this macro into the ErrExpr
 *             class. The macro automatically makes a wrapper class to wrap the
 *             input function into a functor object.
 *
 * @param      FUNCTION   The function to apply element-wise
 * @param      METHODNAME  The name of the method for accessing this function
 *
 */
#ifdef XRRAY_INTERNAL
#define ELEM_FUNCT_MACRO(FUNCTION, METHODNAME)                      \
    inline auto METHODNAME(void) const {                            \
        return xrray::XrrayElemWise<E, T, METHODNAME##_Wrapper<T>>{ \
            static_cast<E const &>(*this)};                         \
    }
#endif  // XRRAY_INTERNAL

#ifdef XRRAY_EXTERNAL
#define ELEM_FUNCT_MACRO(FUNCTION, METHODNAME)                             \
    template <typename T>                                                  \
    struct METHODNAME##_Wrapper {                                          \
        static inline const T operate(T input) { return FUNCTION(input); } \
    };
#endif  // XRRAY_EXTERNAL

/**
 * Including most functions from <cmath>
 */
ELEM_FUNCT_MACRO(std::cos, cos)
ELEM_FUNCT_MACRO(std::sin, sin)
ELEM_FUNCT_MACRO(std::tan, tan)

ELEM_FUNCT_MACRO(std::acos, acos)
ELEM_FUNCT_MACRO(std::asin, asin)
ELEM_FUNCT_MACRO(std::atan, atan)

ELEM_FUNCT_MACRO(std::cosh, cosh)
ELEM_FUNCT_MACRO(std::sinh, sinh)
ELEM_FUNCT_MACRO(std::tanh, tanh)

ELEM_FUNCT_MACRO(std::acosh, acosh)
ELEM_FUNCT_MACRO(std::asinh, asinh)
ELEM_FUNCT_MACRO(std::atanh, atanh)

ELEM_FUNCT_MACRO(std::exp, exp)
ELEM_FUNCT_MACRO(std::exp2, exp2)
ELEM_FUNCT_MACRO(std::expm1, expm1)

ELEM_FUNCT_MACRO(std::log, log)
ELEM_FUNCT_MACRO(std::log2, log2)
ELEM_FUNCT_MACRO(std::log10, log10)
ELEM_FUNCT_MACRO(std::log1p, log1p)

ELEM_FUNCT_MACRO(std::sqrt, sqrt)
ELEM_FUNCT_MACRO(std::cbrt, cbrt)

ELEM_FUNCT_MACRO(std::erf, erf)
ELEM_FUNCT_MACRO(std::erfc, erfc)
ELEM_FUNCT_MACRO(std::tgamma, tgamma)
ELEM_FUNCT_MACRO(std::lgamma, lgamma)

ELEM_FUNCT_MACRO(std::ceil, ceil)
ELEM_FUNCT_MACRO(std::floor, floor)
ELEM_FUNCT_MACRO(std::trunc, trunc)

ELEM_FUNCT_MACRO(std::abs, abs)

/**
 * including functions from <complex>
 */
ELEM_FUNCT_MACRO(std::real, real)
ELEM_FUNCT_MACRO(std::imag, imag)
ELEM_FUNCT_MACRO(std::arg, arg)
ELEM_FUNCT_MACRO(std::conj, conj)

#undef ELEM_FUNCT_MACRO
#undef XRRAY_INTERNAL
#undef XRRAY_EXTERNAL
