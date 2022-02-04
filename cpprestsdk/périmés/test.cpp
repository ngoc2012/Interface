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
#include "include/core.h"
// #include "include/stringF.h"
// #include "include/jsonF.h"
// #include "include/rido.h"

// #include "test.h"

// Not necessery includes
// #include <typeinfo>

using namespace std;

int main(int argc, char **argv)
{
    vector<int> a = {1,2,3,4};
    cout << core::isInVector(a, 1) <<endl;
    cout << core::isInVector(a, 5) <<endl;
    return 0;
}