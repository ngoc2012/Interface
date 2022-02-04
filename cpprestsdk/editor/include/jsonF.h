#ifndef JSONF_H // include guard
#define JSONF_H

#include "fileF.h"
#include "stringF.h"

using namespace std;

// #include "../nlohmann/json.hpp"
#include <nlohmann/json.hpp>
// using json = nlohmann::json;
using json = nlohmann::ordered_json;

namespace jsonF  // namespace declaration
{

    vector<string> getKeys(json obj, bool show=false);

    bool checkKeyInJsonKeys(string key, json obj);

    // bool checkKeysInJsonKeys(vector<string> keys, json obj);

    json importJson(string fileName);

    float eval(json, map<string, string>);

}



// json contains
// if (response.contains("subject_id")
// {
//     cout << "it is found" << endl;

// }
// else
// {
//     cout << "not found " << endl;
// }
#endif