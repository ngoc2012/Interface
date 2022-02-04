#ifndef CSV_H // include guard
#define CSV_H

#include <vector>
#include <string>
#include <regex>

#include "fileF.h"
#include "stringF.h"

#include "jsonF.h"
// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

using namespace std;

namespace csv  // namespace declaration
{
    bool isNotEmptyLine(string);

    extern vector<json> csv2JsonList(string, int, int);

    extern json csv2MapJsonList(const string, int, int);
    
    extern std::vector<std::vector<float>> csv2List(string);
}

#endif