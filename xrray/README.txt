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


Welcome to the xrray module! This module started as C. J. Williams' personal
project and is now released under the new BSD license.

The aim of this module is to introduce a light-weight, fast, extensible, easy to
use, elegant container the Xrray. Xrray is primarily a vector, matrix or tensor
class levering the power of 'expression templates' to perform efficient linear
algebra calculations. The syntax for the xrray expressions and functions is
deliberately similar to numpy and generic tensor algebra.

Xrray expressions build an "expression tree", that enable lazy evaluation of
expressions upon assignment to an Xrray eliminating temporaries and repeated
looping over Xrrays.

Xrray is a contraction of expression-array. An Xrray is considered an xrray
expression.

Xrray expressions support slicing through .slice() and assignment to slices

Xrrays can be printed through an overload of std::cout <<.

Xrray operations/assignments are - where appropriate - compatible with
"scalars" i.e entities of the xrray expression base type, T.

Xrrays expressions are optimised to work with trivial/base types however the
module handles arbitrary base types and can be used as a container for any type.

All shape checking and bounds checking can be disabled through the "#define
XRRAY_CHECK_ON"macro.

All debug printing can be enabled through the "#define XRRAY_DEBUG_ON" macro.

A selection of useful typedefs can be enabled with the "#define
XRRAY_TYPEDEF_ON"macro.

Any function can be easily converted to an element wise function and
automatically integrated into xrray expression using the ELEM_FUNCT_MACRO
(FUNCTION, METHODNAME) macro detailed in xrray/core/base.hpp

Supported operations: (), [], +, -, *, /, %, =, +=, -=, *=, /=, %=

Functions:  sum(), min(), max(), avg(), swap(windows), shuffle, pow()

Initialise: empty(), zeros() ones(), linspace(), enumerate(), random(),
            random_int(), eye()

Methods:    row(), col(), dep(), t12(), t23(), t13(), c12(), c23(), c13(),
            slice(), sum(), min(), max(), avg(), shape(), size(), pow(),  
            reshape()

Most functions in <cmath> and <complex> are available as element wise method.