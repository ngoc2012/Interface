#include "core.h"

/*
C++ functions
stof(): string to float,
*/


vector<string> core::textFile2LinesList(string fileName) {
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
string core::textFile2Text(string fileName) {
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


    
