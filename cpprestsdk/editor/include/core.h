#ifndef CORE_H // include guard
#define CORE_H

#include <vector>

using namespace std;

namespace core  // namespace declaration
{
    template<class T>
    bool isIn(T e, vector<T> v) {
        for (auto& i: v)
            if (e == i) {return true;}
        return false;
    }
}

#endif