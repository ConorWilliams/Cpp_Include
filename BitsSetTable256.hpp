/**
 * BitsSetsTable256.hpp
 *
 * https://graphics.stanford.edu/~seander/bithacks.html
 *
 * Bit Twiddling Hacks
 * By Sean Eron Anderson
 * seander@cs.stanford.edu
 *
 * Individually, the code snippets here are in the public domain (unless
 * otherwise noted) — feel free to use them however you please. The aggregate
 * collection and descriptions are © 1997-2005 Sean Eron Anderson. The code and
 * descriptions are distributed in the hope that they will be useful, but
 * WITHOUT ANY WARRANTY and without even the implied warranty of merchantability
 * or fitness for a particular purpose.
 *
 * Generates a lookup table for number of high bits in a byte
 */

#ifndef BITSSETSTABLE256_HPP
#define BITSSETSTABLE256_HPP

namespace cj {

#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)

static const unsigned char BitsSetTable256[256] = {B6(0), B6(1), B6(1), B6(2)};

}  // namespace cj

#endif