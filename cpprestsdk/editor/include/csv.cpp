#include "csv.h"

static const regex lineRegex("(\\s*,\\s*)+");

bool csv::isNotEmptyLine(string line) {
    if (line == "") return false;
    // regex word_regex("(\\s*,\\s*)+");
    return !regex_match(line, lineRegex);
    // cout << isNotEmptyLine("") << endl;
    // cout << isNotEmptyLine(", , ,") << endl;
    // cout << isNotEmptyLine(",, ") << endl;
    // cout << isNotEmptyLine(",smt, ") << endl;
};

vector<vector<float>> csv::csv2List(const string filename) {
    vector<vector<float>> output;
    vector<string> lines = fileF::textFile2LinesList(filename);

    for(string line : lines) {
        if (isNotEmptyLine(line)) {
            std::vector<float> L;
            vector<string> values = stringF::splitStr(line, ",");
            for(string v : values) {
                if (stringF::isRealNumber(v)) {
                    L.push_back(stof(v));
                }
            }
            output.push_back(L);
        }
    }
        
    return output;
};

vector<json> csv::csv2JsonList(const string filename, int keyLine = 1, int startLine = 2) {
    vector<json> output;
    vector<string> lines = fileF::textFile2LinesList(filename);
    vector<string> keys = stringF::splitStr(lines[keyLine-1], ",");
    // for(string k : keys)
    //     cout << k << ", ";
    // cout << endl;

    // cout << "startLine = " << startLine << endl;

    int i = 1;
    for(string line : lines) {
        if ((i != keyLine) & (i > (startLine-1)) & isNotEmptyLine(line)) {
            // cout << line << endl;
            vector<string> values = stringF::splitStr(line, ",");
            json jLine;
            int j = 0;
            for(string v : values) {
                if (stringF::isRealNumber(v)) {
                    // jLine[keys[j]] = core::roundNumber(stof(v), 3);
                    jLine[keys[j]] = stof(v);
                } else {
                    jLine[keys[j]] = v;
                }
                j++;
            }
            output.push_back(jLine);
        }
        i++;
    }
        
    return output;
};

json csv::csv2MapJsonList(string filename, int mapCol = 1, int keyLine = 1) {
    json output;
    vector<string> lines = fileF::textFile2LinesList(filename);
    vector<string> keys = stringF::splitStr(lines[keyLine-1], ",");

    int i = 1;
    for(string line : lines) {

        if ((i != keyLine) & isNotEmptyLine(line)) {
            // std::cout << line << std::endl;
            vector<string> values = stringF::splitStr(line, ",");
            json jLine;
            int j = 0;
            for(string v : values) {
                if (stringF::isRealNumber(v)) {
                    jLine[keys[j]] = stof(v);
                } else {
                    jLine[keys[j]] = v;
                }
                j++;
            }
            output[values[mapCol-1]] = jLine;
        }
        i++;
    }
        
    return output;
};
