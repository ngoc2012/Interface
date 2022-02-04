#ifndef CHARGE_H // include guard
#define CHARGE_H

#include <iostream>
#include <string>

#include "jsonF.h"

using namespace std;

namespace charge  // namespace declaration
{
    enum etatName { QP, FREQ, CARA, FOND, ACC, EB, EC, EH, EE, last }; // fake last to iterate
    // extern vector<etatName> etatNameList = {etatName::QP, etatName::FREQ, etatName::CARA, etatName::FOND, etatName::ACC};

    struct charge {
        public:
            float Mx=0.0, My=0.0, N=0.0, Fx=0.0, Fy=0.0;
            etatName etat;
    };

    etatName getEtatName(string);
    string getEtatNameString(etatName);

    template<class T>
    json getJsonMapEtat(map<etatName, T> data){
        json o = json::object();
        for (auto e: data) {
            o[getEtatNameString(e.first)] = e.second;
        }
        
        return o;
    };
}

#endif