#ifndef MAPF_H // include guard
#define MAPF_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>  // ifstream

using namespace std;

namespace mapF  // namespace declaration
{
    extern void printMap(map<string, float>);

    // extern bool checkKeyInMap(string, map<string, float>);
    template <class T, class U>
    extern bool checkKeyInMap(T key, map<T, U> inputData) {
        for (auto& x: inputData) {
            if (x.first == key) {
                return true;
            }
        }
        return false;
    };

    // Map with linear key (float) for fast interpolation
    // Columns name
    // Niveau   M       N
    // 1.0      2.3     7.5
    // 2.0      3.5     6.5
    // 5.0      6.2     2.3

    // [0.0] = {2.3, 7.5}
    // [1.0] = {2.3, 7.5}
    // [1.5] = {2.9, 7.0}
    // [6.0] = {6.2, 2.3}

//     template<
//     class Key,
//     class T,
//     class Compare = std::less<Key>,
//     class Allocator = std::allocator<std::pair<const Key, T> >
// > class map;

// namespace pmr {
//     template <class Key, class T, class Compare = std::less<Key>>
//     using map = std::map<Key, T, Compare,
//                          std::pmr::polymorphic_allocator<std::pair<const Key,T>>>
// }
}

#endif