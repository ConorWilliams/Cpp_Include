/**
 * IsingArray.hpp
 *
 * Copyright (c) 2019, C. J. Williams
 * All rights reserved.
 *
 * The main data structure containing a dense representation of N*N spins for
 * the Ising model simulation.
 */

#ifndef ISINGARRAY_HPP
#define ISINGARRAY_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "DenseBits.hpp"
#include "comforts.hpp"

namespace cj {

using std::copy;
using std::cout;
using std::endl;
using std::invalid_argument;

template <uint32_t N>
class IsingArray {
   public:
    cj::DenseBitsH<N> *array = nullptr;

    // functions to get val in i,j position
    inline bool test(const uint32_t i, const uint32_t j) const {
        return array[i % N].test(j);
    }

    // functions to set val in i,j position to 1
    inline void high(const uint32_t i, const uint32_t j) {
        array[i % N].high(j);
    }

    // functions to set val in i,j position to 0
    inline void low(const uint32_t i, const uint32_t j) { array[i % N].low(j); }

    // functions to swap val in i,j position
    inline void flip(const uint32_t i, const uint32_t j) {
        array[i % N].flip(j);
    }

    // function to see how many bits are set to 1 in next to i,j
    inline int adjacent(const uint32_t i, const uint32_t j) const {
        int count = test(i + 1, j);
        count += test(i - 1 + N, j);  //+N to stop % neg in test
        count += test(i, j + 1);
        count += test(i, j - 1 + N);
        return count;
    }

    /*-------------------------------Overloads--------------------------------*/

    template <typename T>
    inline int test(const cj::Pair<T> &pair) const {
        return array[pair.i % N].test(pair.j);
    }

    template <typename T>
    inline void high(const cj::Pair<T> &pair) {
        array[pair.i % N].high(pair.j);
    }

    template <typename T>
    inline void low(const cj::Pair<T> &pair) {
        array[pair.i % N].low(pair.j);
    }

    template <typename T>
    inline void flip(const cj::Pair<T> &pair) {
        array[pair.i % N].flip(pair.j);
    }

    template <typename T>
    inline int adjacent(const cj::Pair<T> &pair) const {
        int count = test(pair.i + 1, pair.j);
        count += test(pair.i - 1 + N, pair.j);
        count += test(pair.i, pair.j + 1);
        count += test(pair.i, pair.j - 1 + N);
        return count;
    }

    /*------------------------------------------------------------------------*/

    // function to see how many bits are set to 1 in array
    inline uint64_t count(void) const {
        uint64_t count = 0;
        for (uint32_t i = 0; i < N; ++i) {
            count += array[i].count();
        }
        return count;
    }

    // function to find total intrinsic energy
    inline double intrinisic(const double (&lookup)[5][2]) const {
        double tmp = 0;
        for (unsigned i = 0; i < N; ++i) {
            for (unsigned j = 0; j < N; ++j) {
                tmp += lookup[adjacent(i, j)][test(i, j)];
            }
        }
        return tmp;
    }

    // functions to count the domain sizes, returns a vector of domain sizes
    std::vector<uint32_t> domain(
        const cj::Pair<uint32_t> (&order)[N * N]) const {
        std::vector<uint32_t> domains;
        IsingArray<N> used;

        uint32_t count = 0;
        uint32_t position = 0;
        uint32_t i;
        uint32_t j;
        bool type;

        while (count < N * N) {
            if (used.test(order[position]) == 0) {
                uint32_t size = 1;
                i = order[position].i;
                j = order[position].j;

                type = test(i, j);
                used.high(i, j);

                size = _left(i, j - 1 + N, size, type, used);
                size = _up(i + 1, j, size, type, used);
                size = _right(i, j + 1, size, type, used);
                size = _down(i - 1 + N, j, size, type, used);

                domains.push_back(size);
                count += size;
            }
            ++position;
        }

        return domains;
    }

    // domain helper function
    uint32_t _left(const uint32_t i, const uint32_t j, uint32_t size,
                   const bool type, IsingArray<N> &ising) {
        // dcout("left at " << i << " " << j);
        if (test(i, j) == type && ising.test(i, j) == 0) {
            ising.high(i, j);
            size += 1;

            size = _left(i, j - 1 + N, size, type, ising);
            size = _up(i + 1, j, size, type, ising);
            size = _down(i - 1 + N, j, size, type, ising);
        }

        return size;
    }

    // domain helper function
    uint32_t _up(const uint32_t i, const uint32_t j, uint32_t size,
                 const bool type, IsingArray<N> &ising) {
        // dcout("up at " << i << " " << j);
        if (test(i, j) == type && ising.test(i, j) == 0) {
            ising.high(i, j);
            size += 1;

            size = _left(i, j - 1 + N, size, type, ising);
            size = _up(i + 1, j, size, type, ising);
            size = _right(i, j + 1, size, type, ising);
        }

        return size;
    }

    // domain helper function
    uint32_t _right(const uint32_t i, const uint32_t j, uint32_t size,
                    const bool type, IsingArray<N> &ising) {
        // dcout("right at " << i << " " << j);
        if (test(i, j) == type && ising.test(i, j) == 0) {
            ising.high(i, j);
            size += 1;

            size = _up(i + 1, j, size, type, ising);
            size = _right(i, j + 1, size, type, ising);
            size = _down(i - 1 + N, j, size, type, ising);
        }

        return size;
    }

    // domain helper function
    uint32_t _down(const uint32_t i, const uint32_t j, uint32_t size,
                   const bool type, IsingArray<N> &ising) {
        // dcout("down at " << i << " " << j);
        if (test(i, j) == type && ising.test(i, j) == 0) {
            ising.high(i, j);
            size += 1;

            size = _left(i, j - 1 + N, size, type, ising);
            size = _right(i, j + 1, size, type, ising);
            size = _down(i - 1 + N, j, size, type, ising);
        }

        return size;
    }

    // function to print the ith DenseBitsH on one line
    void print(const uint32_t i) const { array[i % N].print(); }

    // function to print all the Ising array
    void print_all(void) const {
        for (unsigned i = 0; i < N; ++i) {
            print(i);
        }
        cout << endl;
    }

    // constructor
    IsingArray() { array = new cj::DenseBitsH<N>[N]; }

    // de-constructor
    ~IsingArray() {
        delete[] array;
        array = nullptr;
    }

    // copy constructor for functions
    IsingArray(IsingArray const &other) {
        if (this != &other) {
            delete[] array;
            array = nullptr;
            array = new DenseBitsH<N>[N];
            std::copy(other.array, other.array + N, array);
        }
        dcout("copy");
    }

    // assignment operator
    IsingArray &operator=(const IsingArray &other) {
        if (this != &other) {
            std::copy(other.array, other.array + N, array);
        }
        dcout("assignment");
        return *this;
    }

    // move operator
    IsingArray &operator=(IsingArray &&other) noexcept {
        if (this != &other) {
            delete[] array;
            array = nullptr;
            array = other.array;
            other.array = nullptr;
        }
        dcout("move operator");
        return *this;
    }
};  // namespace cj

}  // namespace cj

#endif