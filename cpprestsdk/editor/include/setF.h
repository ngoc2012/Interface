#ifndef SETF_H // include guard
#define SETF_H

#include <iostream>
#include <set>

using namespace std;

namespace setF  // namespace declaration
{
    template <class T>
    extern void printSetInLine(set<T> vec) {
        cout << "[";
        int i = 0;
        for (T v: vec) {
            if (i < (int(vec.size()) - 1)) {
                cout << v << ",";
            } else {
                cout << v;
            }
            i++;
        }
        cout << "]" << endl;
    };

    template <class T>
    T maxValue(set<T> vec) {
        T maxValue = vec[0];
        for (T v: vec) {
            maxValue = max(maxValue, v);
        }
        return maxValue;
    };
}

#endif