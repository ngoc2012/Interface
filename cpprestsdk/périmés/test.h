#include <iostream>
// #include <fstream>  // ifstream
#include <vector>
// #include <string>
// #include <map>
// #include <regex>
// #include <iterator>
#include <algorithm>

// #include <math.h>       /* sqrt */

// #include "include/BAparoi.h"
// #include "include/core.h"
// #include "include/stringF.h"
// #include "include/jsonF.h"
// #include "include/rido.h"

// Not necessery includes
// #include <typeinfo>

using namespace std;

namespace test  // namespace declaration
{
    template<class T>
    extern bool isInVector(vector<T> vec, T item) {
        return (find(vec.begin(), vec.end(), item) != vec.end());
    }
    
}