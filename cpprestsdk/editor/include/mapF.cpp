// #include "core.h"
#include "mapF.h"

/*
C++ functions
stof(): string to float,
*/

void mapF::printMap(map<string, float> inputData) {
    // map contain
    string output = "{";
    for (auto& x: inputData)
        // cout << "\"" << x.first << "\":" << x.second << ", ";
        output += "\"" + x.first + "\":" + to_string(x.second) + ", ";
    
    output = output.substr(0, output.size()-2);
    output += "}\n";
    cout << output;
}; 
