#include "fileF.h"

vector<string> fileF::textFile2LinesList(string fileName) {
    vector<string> listOut;
    string line;
    ifstream myfile (fileName);
    if (myfile.is_open()) {
            while (getline(myfile,line))
                listOut.push_back(line);
            myfile.close();
        } else {
            cout << "Unable to open file";
        }
    return listOut; 
};

string fileF::textFile2Text(string fileName) {
    string textOut = "";
    string line;
    ifstream myfile (fileName);
    if (myfile.is_open()) {
            while (getline(myfile,line))
                textOut += line + "\n";
            myfile.close();
        } else {
            cout << "Unable to open file";
            return "";
        }
    return textOut;
};

string fileF::getFileExtension(string fileName) {
    vector<string> text= stringF::splitStr(fileName, ".");
    if (text.size() > 1) {
        return text[text.size() - 1];
    } else {
        return "";
    }
};
    
