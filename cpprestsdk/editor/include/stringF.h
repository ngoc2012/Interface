#ifndef STRINGF_H // include guard
#define STRINGF_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <math.h>

#include "tinyexpr.h"
#include "vectorF.h"

// using namespace std;

namespace stringF  // namespace declaration
{
    extern vector<std::string> splitStr(std::string, std::string);

    extern vector<std::string> splitStrSpace(std::string, std::string);

    template <class T>
    extern void printVectorInLine(vector<T> vec) {
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
    }

    extern void printVectorStringInLine(vector<std::string>);

    extern void printVectorString(vector<std::string>);

    extern std::string joinStr(vector<std::string>, std::string);

    extern std::string substr(std::string, int, int);

    extern bool stringInStringVector(std::string, vector<std::string>);

    extern float stof(std::string);

    extern bool isRealNumber(std::string txt);

    extern std::string replace(std::string str, const std::string from, const std::string to);

    extern void replaceInplace(std::string& str, const std::string from, const std::string to);

    vector<int> str2IdList(std::string, std::string);

    vector<int> listStr2IdList(vector<int>, std::string);

    float evalString(std::string);

    std::string replaceTermes(std::string input, map<std::string, std::string> termes);

    std::string ftos(float, int);
}

#endif