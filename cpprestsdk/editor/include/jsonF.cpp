#include "jsonF.h"

using namespace std;

vector<string> jsonF::getKeys(json obj, bool show) {
    vector<string> keys;
    for (auto it: obj.items())
        keys.push_back(it.key());
    if (show) {
        string txt = "[";
        for(string k : keys) 
            txt += k + ", ";
        cout << txt.substr(0, txt.size()-2) + "]\n";
    }
    return keys;
};

bool jsonF::checkKeyInJsonKeys(string key, json obj) {
    // vector<string> keys = jsonF::getKeys(obj);
    // return stringF::stringInStringVector(key, keys);
    return obj.contains(key);
};

json jsonF::importJson(string fileName) {
    // cout << "Import json file: " + fileName << "\n";
    string text = fileF::textFile2Text(fileName);
    // cout << text << "\n";
    // parse and serialize JSON
    if (text == "") {return json::parse("{}");}
    json j_complete = json::parse(text);
    // cout << j_complete << "\n\n";
    return j_complete;
};

float jsonF::eval(json j, map<string, string> termes){
    if (j.is_string()) {
        return stringF::evalString(stringF::replaceTermes(j.get<string>(), termes));
        // cout << stringF::evalString(line["startChild"]["offsetValue"].get<string>()) << endl;
    } else {
        return j.get<float>();
    }
};
    