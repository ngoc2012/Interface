#include "charge.h"

using namespace std;

charge::etatName charge::getEtatName(string stringEtatName) {
    if (stringEtatName == "QP") {/*cout << "QP" << endl;*/return etatName::QP;}
    if (stringEtatName == "FREQ") {/*cout << "FREQ" << endl;*/return etatName::FREQ;}
    if (stringEtatName == "CARA") {/*cout << "CARA" << endl;*/return etatName::CARA;}
    if (stringEtatName == "FOND") {/*cout << "FOND" << endl;*/return etatName::FOND;}
    if (stringEtatName == "ACC") {/*cout << "ACC" << endl;*/return etatName::ACC;}
    if (stringEtatName == "EB") {/*cout << "ACC" << endl;*/return etatName::EB;}
    if (stringEtatName == "EC") {/*cout << "ACC" << endl;*/return etatName::EC;}
    if (stringEtatName == "EH") {/*cout << "ACC" << endl;*/return etatName::EH;}
    if (stringEtatName == "EE") {/*cout << "ACC" << endl;*/return etatName::EE;}
    cout << "Etat name : " << stringEtatName << " does not exist" << endl;
    return etatName::CARA;
}

string charge::getEtatNameString(etatName etat) {
    if (etat == etatName::QP) {/*cout << "QP" << endl;*/return "QP";}
    if (etat == etatName::FREQ) {/*cout << "FREQ" << endl;*/return "FREQ";}
    if (etat == etatName::CARA) {/*cout << "CARA" << endl;*/return "CARA";}
    if (etat == etatName::FOND) {/*cout << "FOND" << endl;*/return "FOND";}
    if (etat == etatName::ACC) {/*cout << "ACC" << endl;*/return "ACC";}
    if (etat == etatName::EB) {/*cout << "ACC" << endl;*/return "EB";}
    if (etat == etatName::EC) {/*cout << "ACC" << endl;*/return "EC";}
    if (etat == etatName::EH) {/*cout << "ACC" << endl;*/return "EH";}
    if (etat == etatName::EE) {/*cout << "ACC" << endl;*/return "EE";}
    cout << "Etat : " << etat << " does not exist" << endl;
    return "";
}