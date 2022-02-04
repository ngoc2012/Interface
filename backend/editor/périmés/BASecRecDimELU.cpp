#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <any>
#include <math.h>       /* sqrt */

#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

void printMap(map<string, float> inputData) {
    // map contain
    string output = "{";
    for (auto& x: inputData)
        // cout << "\"" << x.first << "\":" << x.second << ", ";
        output += "\"" + x.first + "\":" + to_string(x.second) + ", ";
    
    output = output.substr(0, output.size()-2);
    output += "}\n";
    cout << output;
}

void calculArmaELU(map<string, float> inputData) {

    map<string, float> outputData;
    float d = inputData["h"] - inputData["enrobage"]/100.0;   // m
    float fcd = inputData["fck"]/1.5;       // MPa
    float fyd = inputData["fyk"]/1.15;      // MPa
    float muybu = inputData["MEd"]/inputData["b"]/d/d/fcd/1000.0;
    
    if (muybu > 0.25) {
        outputData["A"] = 10000.0;
    } else {
        float alpha = 1.25*(1.0 - sqrt(1.0 - 2.0*muybu));
        float z = d*(1.0 - 0.4*alpha);
        outputData["A"] = inputData["MEd"]/z/fyd*10.0;  // cm2
    };
    outputData["d"] = d;
    outputData["fcd"] = fcd;
    outputData["fyd"] = fyd;
    outputData["muybu"] = muybu;
    printMap(outputData);
}

vector<string> getStrArray(string str) {
    vector<string> strArray;

    string delimiter = ",";

    size_t last = 0;
    size_t next = 0;

    while ((next = str.find(delimiter, last)) != string::npos) {
        strArray.push_back(str.substr(last, next-last));
        last = next + 1;
    }
    strArray.push_back(str.substr(last));

    return strArray;
}

map<string, float> getInputData(char **argv, int startId) {
    
    string arg2(argv[startId]);
    vector<string> strArray = getStrArray(arg2);

    map<string, float> inputData;
    int i = startId + 1;
    for(string param : strArray) {
        inputData.emplace(param, atof(argv[i]));
        i++;
    }
    return inputData;
}

string readTextFile(string fileName) {
    string textOut = "";
    string line;
    ifstream myfile (fileName);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) ) {
            // cout << line << '\n';
            textOut += line + "\n";
        }
        myfile.close();
    }
    else cout << "Unable to open file"; 
    return textOut;
}

int main(int argc, char **argv)
{
    // for(int i = 0; i < argc; ++i) cout << argv[i] << '\n';

    string arg1(argv[1]);
    string arg2(argv[2]);

    map<string, float> inputData = getInputData(argv, 3);
    // printMap(inputData);

    // Web version
    if (arg1 == "on") {
        if (arg2 == "BASecRecDimELU") {
            calculArmaELU(inputData);
        }
    } else if (arg1 == "off") {
        if (arg2 == "BASecRecDimELU") {
            string folderName(argv[3]);
            string fileName(argv[4]);
            // cout << folderName << "\n";
            string text = readTextFile(folderName + fileName);
            // cout << text << "\n";
            // parse and serialize JSON
            json j_complete = json::parse(text);
            cout << j_complete << "\n\n";
        }
    }

    return 0;
}