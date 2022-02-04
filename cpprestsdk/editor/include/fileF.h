#ifndef FILEF_H // include guard
#define FILEF_H

#include <iostream>
#include <string>
#include <vector>
// #include <map>
// #include <regex>
#include <fstream>  // ifstream
// #include <algorithm>

#include "stringF.h"

using namespace std;

namespace fileF  // namespace declaration
{
    extern vector<string> textFile2LinesList(string);

    extern string textFile2Text(string);
    
    string getFileExtension(string);
}

#endif