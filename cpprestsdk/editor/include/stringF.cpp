#include "stringF.h"

// smatch matches;
// regex_search(line, matches, regex("\\s*(\\d+) \\.\\.\\. (.+)"));
// matches[i].str()

static const regex realNumberRegex("-?\\d+(\\.\\d*)?((e|E)(\\+|-)\\d+)?");

float stringF::evalString(string txt) {
    const char * c = txt.c_str();
    float f = te_interp(c, 0);
    return f;
}   

bool stringF::isRealNumber(string txt) {
    return regex_match(txt, realNumberRegex);
}

string stringF::replace(string str, const string from, const string to) {
    if(from.empty())
        return str;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return str;
}

void stringF::replaceInplace(string& str, const string from, const string to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

// Cout empty string
vector<string> stringF::splitStr(string str, string delimiter = ",") {
    vector<string> strArray;
    size_t last = 0;
    size_t next = 0;
    while ((next = str.find(delimiter, last)) != string::npos) {
        if (str.substr(last, next-last) != "") {
            strArray.push_back(str.substr(last, next-last));
        } else {
            strArray.push_back("");
        }
        last = next + 1;
    }
    strArray.push_back(str.substr(last));
    return strArray;
};

// Do not cout empty string
vector<string> stringF::splitStrSpace(string str, string delimiter = ",") {
    vector<string> strArray;
    size_t last = 0;
    size_t next = 0;
    while ((next = str.find(delimiter, last)) != string::npos) {
        if (str.substr(last, next-last) != "") {
            strArray.push_back(str.substr(last, next-last));
        }
        last = next + 1;
    }
    strArray.push_back(str.substr(last));
    return strArray;
};

string stringF::joinStr(vector<string> inputData, string delimiter) {
    string output = "";
    int i = 0;
    for (string v: inputData) {
        if (i < (inputData.size() - 1)) {
            output += v + delimiter;
        } else {
            output += v;
        }
        i += 1;
    }
    return output;
}

void stringF::printVectorStringInLine(vector<string> inputData) {
    cout << "[" << stringF::joinStr(inputData, ",") << "]" << endl;
}

void stringF::printVectorString(vector<string> inputData) {
    for (string v: inputData) {
        cout << v << endl;
    }
}

string stringF::substr(string line, int start, int length) {
    if (start > (int(line.size())-1)) {
        return "";
    } else {
        return line.substr(start, length);
    }
}

bool stringF::stringInStringVector(string str, vector<string> strVector) {
    for (string s: strVector)
        if (str == s) {return true;}
    return false;
}

float stringF::stof(string s) {
    if (s == "") {
        return 0.0;
    } else {
        try {
            return std::stof(s);
        } catch (...) {
            return 0.0;
        }
    }
}

static const regex positiveNumberRegex("\\d+");
static const regex negativeNumberRegex("-\\d+");
static const regex numbersGroupRegex("-?\\d+:-?\\d+");
static const regex positiveNumbersGroupRegex("\\d+-\\d+");

static int negative2PositivePosition(int listLen, int pos) {
    if (pos > -1) {return pos;}
    if ((-pos) > listLen) {
        return 0;
    } else {
        // 0 1 2 3 4 => (5-3=2, 5-1=4)
        return listLen+pos;
    }
}

vector<int> getNumbersGroupRegex(vector<int> list, string str) {
    vector<int> idList;
    if (regex_match(str, numbersGroupRegex)) {
        vector<string> txt = stringF::splitStr(str, ":");
        int start = stoi(txt[0]);
        int end = stoi(txt[1]);
        int listLen = list.size();

        for (int i=negative2PositivePosition(listLen, start); i <= negative2PositivePosition(listLen, end); i++) {
            idList.push_back(list[i]);
        }
    } else {
        cout << "String does not match getNumbersGroupRegex: -5:-2 : " << str << endl;
    }
    return idList;
}

vector<int> stringF::listStr2IdList(vector<int> list, string str) {
    vector<int> idList;
    // std::cout << str << std::endl;
    for (string s: splitStrSpace(str, ";"))
        for (int i: getNumbersGroupRegex(list, s))
            idList.push_back(i);

    return idList;
}

vector<int> stringF::str2IdList(string str, string delimiter = ",") {
    vector<int> idList;
    vector<string> strList = stringF::splitStr(str, delimiter);
	
    // printVectorStringInLine(strList);

    for (string s: strList) {
        if (regex_match(s, positiveNumberRegex)) {
            int newId = stoi(s);
            // cout << newId << endl;
            if (!vectorF::isInVector(idList, newId)) {
                idList.push_back(newId);
            }
        }
        if (regex_match(s, positiveNumbersGroupRegex)) {
            vector<string> txt = stringF::splitStr(s, "-");
            
            int start = stoi(txt[0]);
            int end = stoi(txt[1]);
            
            if (start > end) {
                for (int newId=start; newId >= end; newId--) {
                    if (!vectorF::isInVector(idList, newId)) {
                        idList.push_back(newId);
                    }
                }
            } else {
                for (int newId=start; newId <= end; newId++) {
                    if (!vectorF::isInVector(idList, newId)) {
                        idList.push_back(newId);
                    }
                }
            }
            

        }
    }
	// for txt0 in txtList:
	// 	if re.fullmatch(r"\d+", txt0):
	// 		# print(r"\d+")
	// 		if int(txt0) not in idList:
	// 			idList.append(int(txt0))
	// 	elif re.fullmatch(r"-\d+", txt0):
	// 		# print(r"-\d+")
	// 		idList.append(array[int(txt0)])

	// 	elif re.fullmatch(r"-?\d+:-?\d+", txt0):
	// 		# print(r"-?\d+:-?\d+")
	// 		txt1 = txt0.split(':')
	// 		# print(int(txt1[0]), (int(txt1[1])+1))
	// 		# print(array[int(txt1[0]):][:(int(txt1[1])+1)])
	// 		if int(txt1[1]) == -1:
	// 			idList += array[int(txt1[0]):]
	// 		else:
	// 			idList += array[int(txt1[0]):][:(int(txt1[1])+1)]
	// 	elif re.fullmatch(r"\d+-\d+", txt0):
	// 		# print(r"\d+-\d+")
	// 		txt1 = txt0.split('-')
			
	// 		for id0 in range(int(txt1[0]), int(txt1[1])+1):
	// 			if id0 not in idList:
	// 				idList.append(id0)
		
	// return idList
    // printVectorInLine(idList);
    return idList;
}

string stringF::replaceTermes(string input, map<string, string> termes){
    string input0 = input;
    for (auto& t: termes) {
        replaceInplace(input0, t.first, t.second);
    }
    return input0;
};

string stringF::ftos(float number, int precision) {
    if (precision == 0) {
        return to_string((int) round(number));
    }
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << number;
    return out.str();
}