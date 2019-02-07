// header file for hmap class

#ifndef ZMAP_HPP
#define ZMAP_HPP

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "ByteOfBits.hpp"
#include "MurmurHash3.hpp"

using std::copy;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::move;
using std::swap;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

namespace cj {

static const int OVERSIZE = 1;  // how much bigger table is
static const uint32_t INITIAL_TABLE_SIZE = 7;

template <class value_t = uint32_t>
class HashTable {
 private:
  struct item {
    cj::ByteOfBits flag;
    uint8_t dist;
    uint32_t key;
  };

  //===================variables==================//

  uint32_t size = INITIAL_TABLE_SIZE;
  uint32_t size_reserve = INITIAL_TABLE_SIZE;
  uint32_t members = 0;
  uint32_t tombstones = 0;

  uint64_t max_members = 0;
  uint64_t table_length = 0;
  uint64_t mesh = 0;

  item *table = nullptr;
  value_t *values = nullptr;

 public:
  value_t *not_in_table;  // dummy pointer to compare for find fail

  //===================Functions==================//
  inline uint32_t hash(const uint32_t key) {
    uint32_t hash = 0;
    MurmurHash3_x86_32(&key, 4, 0, &hash);  // hash the key
    return hash & mesh;
  }

  inline void update(void) {
    max_members = 1ULL << (size);
    table_length = 1ULL << (size + OVERSIZE);
    mesh = table_length - 1;
    return;
  }

  inline void alloc(void) {
    table = new item[table_length]{};
    values = new value_t[table_length]{};
    return;
  }

  inline void clean(void) {
    delete[] table;
    delete[] values;
    table = nullptr;
    values = nullptr;
    return;
  }

  // clears the table and resizes
  void clear(void) {
    clean();
    size = size_reserve;
    members = 0;
    tombstones = 0;
    update();
    alloc();

    return;
  }

  // reserve space in the table
  void reserve(uint32_t reserve) {
    if (reserve < INITIAL_TABLE_SIZE) {
      throw invalid_argument("Cannot reserve less than default");
    }
    size_reserve = reserve;
    if (reserve > size) {
      size = reserve;
      this->rebuild();
    }
    return;
  }

  // rebuilds the table to a new spec
  void rebuild(void) {
    if (size > 31) throw invalid_argument("Table overfilled");
    if (size < INITIAL_TABLE_SIZE) size = INITIAL_TABLE_SIZE;

    uint64_t table_length_old = table_length;
    uint64_t members_old = members;

    members = 0;
    tombstones = 0;

    item *table_old = table;
    value_t *values_old = values;

    update();
    alloc();

    cout << "Table size: " << size << endl;

    for (uint32_t index = 0; index < table_length_old; ++index) {  // insert
      if (table_old[index].flag.get(1) == 0 &&
          table_old[index].flag.get(0) == 1) {
        emplace(table_old[index].key, move(values_old[index]));
      }
    }

    members = members_old;

    delete[] table_old;
    delete[] values_old;
    return;
  }

  // add a copy to the map
  inline void insert(uint32_t key, value_t value) { emplace(key, move(value)); }

  // add key, value to table move() compatable
  void emplace(uint32_t key, value_t &&value) {
    if (members > max_members) {
      ++size;
      this->rebuild();
    }

    uint32_t index = this->hash(key);
    uint8_t dist = 0;

    while (true) {
      if (table[index].flag.get(0) == 0) {  // if unoccupied put here
        table[index].flag.set(0, 1);
        table[index].dist = (dist);
        table[index].key = (key);
        values[index] = move(value);
        ++members;
        return;
      } else if (table[index].key == key && table[index].flag.get(1) == 0) {
        values[index] = move(value);
        return;
      }

      // untested change
      if (dist >= table[index].dist && table[index].flag.get(1) == 1) {
        table[index].flag.set(1, 0);  // d flag is not deleted
        table[index].flag.set(0, 1);  // o flag is occupied
        table[index].dist = (dist);
        table[index].key = (key);
        values[index] = move(value);
        ++members;
        --tombstones;
        return;
      }

      if (dist > table[index].dist) {
        swap(dist, table[index].dist);
        swap(key, table[index].key);
        swap(value, values[index]);
      }

      index = (index + 1) & mesh;  // increase index
      ++dist;                      // increase distance
      if (dist >= __UINT8_MAX__) {
        this->rebuild();
        cout << "odd probe depth - rebuilding" << endl;
      }
    }
  }

  // delete key, returns 0 if not in table
  bool erase(const uint32_t key) {
    if (tombstones > (max_members >> 1)) this->rebuild();
    if (members < (max_members >> 2) && size > size_reserve) {
      --size;
      this->rebuild();
    }

    uint32_t index = this->hash(key);
    uint8_t dist = 0;

    while (true) {
      if (table[index].flag.get(0) == 0) return false;  // if unoccupied ntd
      if (dist > table[index].dist) return false;       // if too deep bale
      if (table[index].key == key && table[index].flag.get(1) == 0) {
        // if this spot matches key and is not deleted;
        table[index].flag.set(1, 1);  // flag deleted
        ++tombstones;
        --members;
        return true;
      }

      index = (index + 1) & mesh;  // increase index
      ++dist;                      // increase distance
    }
  }

  inline value_t &operator[](const uint32_t key) { return find(key); }

  // get value from key, returns a pointer to the value
  // returns ref to not_in_table pointer if not in table
  value_t &find(const uint32_t key) {
    uint32_t index = this->hash(key);
    uint8_t dist = 0;

    while (true) {
      if (table[index].flag.get(1) == 0 && table[index].flag.get(0) == 1 &&
          table[index].key == key) {
        // if this spot matches key and is not deleted;
        return values[index];
      }
      if (table[index].flag.get(0) == 0) return *not_in_table;
      if (dist > table[index].dist) return *not_in_table;

      index = (index + 1) & mesh;  // increase index
      ++dist;                      // increase distance
    }
  }

  // does it have it
  bool contains(const uint32_t key) {
    uint32_t index = this->hash(key);
    uint8_t dist = 0;

    while (true) {
      if (table[index].flag.get(1) == 0 && table[index].flag.get(0) == 1 &&
          table[index].key == key) {
        // if this spot matches key and is not deleted;
        return true;
      }
      if (table[index].flag.get(0) == 0) return false;  // if unoccupied ntd
      if (dist > table[index].dist) return false;       // if too deep bale

      index = (index + 1) & mesh;  // increase index
      ++dist;                      // increase distance
    }
  }

  // reports statistics of the hash table
  void report(void) {
    float load = static_cast<float>(members + tombstones) / table_length * 100;

    cout << "#=======Report, Start=======#" << endl;
    cout << "hmap is at " << load << "% load" << endl;
    cout << "hmap contains " << members << " elements" << endl;
    cout << "hmap size is " << size << " elements" << endl;
    cout << "hmap could fit " << max_members << " elements" << endl;
    cout << "hmap has " << tombstones << " tombstones" << endl;
    cout << "reserve is " << size_reserve << endl;
    cout << "#=======Report, End=======#" << endl;
  }

  ~HashTable() {  // destructor
    clean();
  }

  HashTable() {  // constructor
    update();
    alloc();
  }

  void _copy(HashTable &to, const HashTable &from) {
    if (&to != &from) {
      to.size = from.size;
      to.size_reserve = from.size_reserve;
      to.members = from.members;
      to.tombstones = from.tombstones;

      to.max_members = from.max_members;
      to.table_length = from.table_length;
      to.mesh = from.mesh;
    }
    return;
  }

  HashTable(HashTable const &other) {  // copy constructor for functions
    if (this != &other) {
      clean();
      _copy(*this, other);
      alloc();

      std::copy(other.table, other.table + other.table_length, table);
      std::copy(other.values, other.values + other.table_length, values);
    }
    //*this = other; //untested remove needs testing
  }

  HashTable &operator=(const HashTable &other) {  // assignment operator
    if (this != &other) {
      if (size != other.size) {
        clean();
        table_length = other.table_length;
        alloc();
      }
      _copy(*this, other);
      std::copy(other.table, other.table + other.table_length, table);
      std::copy(other.values, other.values + other.table_length, values);
    }
    return *this;
  }

  HashTable &operator=(HashTable &&other) noexcept {  // move operator
    if (this != &other) {
      clean();

      table = other.table;
      values = other.values;

      other.table = nullptr;
      other.values = nullptr;

      _copy(*this, other);
    }
    cout << "move operator" << endl;
    return *this;
  }
};

}  // namespace cj

#endif /*ZMAP_HPP */
