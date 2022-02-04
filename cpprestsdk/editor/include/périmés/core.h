#ifndef CORE_H // include guard
#define CORE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <fstream>  // ifstream
#include <algorithm>

#include "stringF.h"

using namespace std;

namespace core  // namespace declaration
{
    // the compiler can't include complete versions of template functions => must be define in header file
    template<class T>
    bool isInVector(vector<T> vec, T item) {
        return (find(vec.begin(), vec.end(), item) != vec.end());
    }

    // template<class T>
    // bool isInVector(vector<T>, T)

    extern vector<string> textFile2LinesList(string);

    extern string textFile2Text(string);
    
}

#endif