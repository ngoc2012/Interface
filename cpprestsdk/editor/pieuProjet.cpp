#include <iostream>
#include <fstream>  // ifstream
#include <vector>
#include <string>
#include <map>
#include <regex>
// #include <iterator>
// #include <algorithm>

#include <math.h>       /* sqrt */

#include "include/setF.h"
#include "include/const.h"
#include "include/fileF.h"
#include "include/stringF.h"
#include "include/jsonF.h"
#include "include/csv.h"
#include "include/mathF.h"
#include "include/dxf.h"
#include "include/beton.h"
#include "include/arma.h"
#include "include/pieu.h"
#include "include/BAsection.h"

// Not necessery includes
// #include <typeinfo>

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

bool checkPieuTypeParam(json pieu1, json pieu2, vector<string> param) {
    for (string p: param) {
        if (pieu1[p].type() == json::value_t::number_float) {
            if (pieu1[p].get<float>() != pieu2[p].get<float>()) {
                // std::cout << "Param " << p << " float" << std::endl;
                return false;
                }
        } else {
            if (pieu1[p].get<string>() != pieu2[p].get<string>()) {
                // std::cout << "Param " << p << " string " << pieu1[p].get<string>() << " " << pieu2[p].get<string>() << std::endl;
                return false;}
        }
    }
    return true;
}

bool checkGroupeEffet(json pieu1, json pieu2) {
    return (pieu::groupeEffet(pieu1["d"].get<float>(), pieu1["diametre"].get<float>(), pieu1["m"].get<float>(), pieu1["n"].get<float>())  == pieu::groupeEffet(pieu2["d"].get<float>(), pieu2["diametre"].get<float>(), pieu2["m"].get<float>(), pieu2["n"].get<float>()));
}


bool checkPieuTypeExist(json pieu0, json pieuList) {
    vector<string> param = {"Type", "sondage", "plateforme", "sup", "supEff"};
    for (auto pieu: pieuList) {
        if (pieu["Appui"].get<string>() != pieu0["Appui"].get<string>()) {
            if (checkPieuTypeParam(pieu0, pieu, param)) {
                // std::cout << "Param not match" << std::endl;
                // if (checkGroupeEffet(pieu, pieu0)) {return false;}
                if (pieu["Type"].get<string>() == "barrette") {
                    if ((pieu["epaisseur"].get<float>() == pieu0["epaisseur"].get<float>()) & (pieu["largeur"].get<float>() != pieu0["largeur"].get<float>())) {return true;}
                } else {
                    if (pieu["diametre"].get<float>() == pieu0["diametre"].get<float>()) {return true;}
                }
            }
            
        }
    }
    return false;
}

// bool checkFondationTypeExist(json pieu0, json pieuList) {
//     vector<string> param = {"Type"};
//     for (auto pieu: pieuList) {
//         if (pieu["Appui"].get<string>() != pieu0["Appui"].get<string>()) {
//             if (checkPieuTypeParam(pieu0, pieu, param)) {
//                 return true;
//             }
//         }
//     }
//     return false;
// }


int main(int argc, char **argv)
{
    // .\editor.exe off armaPieu ngoc2012 "TramwayT3\PieuxInd0\param"
    std::string userName(argv[1]);
    std::string fileName(argv[2]);
    // std::cout << usersFolder + userName + "\\" + fileName + ".json" << std::endl;
    json param = jsonF::importJson(usersFolder + userName + "\\" + fileName + ".json");
    // beton::beton betonMateriaux;
    // arma::arma armaMateriaux;

    // Pieu list
    // std::cout << param["folder"].get<string>() + param["dispo"].get<string>() << std::endl;
    json pieuList = csv::csv2JsonList(param["folder"].get<string>() + "\\" + param["dispo"].get<string>(), 1, 1);

    json solliPieux = csv::csv2JsonList(param["folder"].get<string>() + "\\" + param["solliPieux"].get<string>(), 1, 2);
    
    vector<pieu::pieuType> pieuTypeList;

    geotech::typeFondation currentTypeFondation;
    std::set<geotech::typeFondation> typeFondationList;

    // All sondages
    std::map<std::string, geotech::sondage> sondagesProjet;
    for (std::string sondageName: jsonF::getKeys(param["coucheList"])) {
        geotech::sondage SP1;
        SP1.getCouchesList(param["coucheList"][sondageName]);
        SP1.nom = sondageName;
        sondagesProjet.insert(std::pair<std::string, geotech::sondage>(sondageName, SP1));
    }

    for (auto pieu: pieuList) {
        std::cout << pieu["Appui"].get<string>() << std::endl;
        
        // Nom des sondages
        vector<string> sondagesText = stringF::splitStr(pieu["sondage"].get<string>(), ",");

        currentTypeFondation = geotech::str2typeFondation(pieu["Type"].get<string>());
        std::set<geotech::typeFondation>::iterator it = typeFondationList.find(currentTypeFondation);
        // Check if type fondation does not exist
        if (it == typeFondationList.end()) {
            for (auto sondageNom: sondagesText) {
                for (auto couche: sondagesProjet[sondageNom].couchesList) {
                    couche.getKpMax(currentTypeFondation);
                    couche.getQsMax(currentTypeFondation);
                }
            }
            
            typeFondationList.insert(currentTypeFondation);
        }

        // if (!checkFondationTypeExist(pieu, pieuList)) {
        //     // ============ Rapport ============
        //     // std::vector<string> couchesName = jsonF::getKeys(param["coucheList"]);
        //     // cout << couchesName[0] << endl;
        //     // cout << param["coucheList"][couchesName[0]] << endl;
            
        // }

        // Get DDC
        vector<charge::charge> chargeList;
        for (auto solli: solliPieux)
            if (solli["Appui"].get<string>() == pieu["Appui"].get<string>()) {
                charge::charge newCharge;
                newCharge.etat = charge::getEtatName(solli["Etat"].get<string>());
                newCharge.Fx = solli["Fx"].get<float>();
                newCharge.Fy = solli["Fy"].get<float>();
                newCharge.Mx = solli["Mx"].get<float>();
                newCharge.My = solli["My"].get<float>();
                newCharge.N = solli["N"].get<float>();
                chargeList.push_back(newCharge);
            }

        if (!checkPieuTypeExist(pieu, pieuList)) {
            pieu::pieuCirc newPieuCirc;
            pieu::pieuType newPieuVari;
            pieu::pieuType *newPieuType = &newPieuCirc;
            if (pieu["Type"].get<string>() == "barrette") {
                pieu::pieuType *newPieuType = &newPieuVari;
            }

            newPieuType->sondagesProjet = sondagesProjet;
            newPieuType->sondagesNom = sondagesText;
            newPieuType->mailleNiveau = pieu["mailleNiveau"].get<float>();
            newPieuType->getParams(pieu);

            newPieuType->Ce = pieu::groupeEffet(pieu["d"].get<float>(), newPieuType->B, pieu["m"].get<float>(), pieu["n"].get<float>());
            
            newPieuType->getPortance(chargeList);
            
            pieuTypeList.push_back(*newPieuType);
        }
    }

    std::cout << "Nombre de pieu: " << pieuList.size() << std::endl;

    std::cout << "Nombre de pieu type: " << pieuTypeList.size() << std::endl;
    
    // // SP1.showCouchesList();
    // // ============ Pieu Type =============
    // std::map<std::string, pieu::pieuType> pieuTypeList;
    // std::vector<std::string> keys = jsonF::getKeys(param["pieuType"]);
    // for (std::string pieuName: keys) {
    //     std::cout << pieuName << endl;
    //     pieu::pieuType pieu0;
    //     pieu0.label = pieuName;
    //     pieu0.getSondages(sondagesProjet, param["pieuType"][pieuName]["sondages"]);
    //     // std::cout << param["pieuType"][pieuName]["nivAncrageMini"] << std::endl;
    //     pieu0.getPortance(param["pieuType"][pieuName]["nivAncrageMini"].get<float>());

    //     pieuTypeList.insert(std::pair<std::string, pieu::pieuType>(pieuName, pieu0));
    // }

    // ============ Pieu =============
    // pieu::pieu P1;
    // vector<json> pieuList = csv::csv2JsonList(param["folder"].get<string>() + "/" + param["dispo"].get<string>(), 1, 2);
    // P1.getJsonPieu(pieuList[0]);

    
    // // P1.updateDiametre(0.5);
    // // cout << P1.diametre << endl;
    // // cout << P1.EI[charge::etatName::CARA] << endl;

    // pieu::KfReduit reduction;
    // reduction.startNiveau = SP1.couchesList[0].sup;
    // reduction.endNiveau = SP1.couchesList[0].sup - 4.0*P1.diametre;
    // reduction.startCoef = 0.5;
    // reduction.endCoef = 1.0;
    // reduction.reduit = true;
    
    // P1.newModel(SP1, 0.1, reduction);
    
    // // ============ Section BA =============
    // BAsection::output out;
    
    // out.beton = betonMateriaux;
    // out.arma = armaMateriaux;
    // out.sectionBeton = BAsection::maillerSectionBetonCercle(param["section"]["diametre"].get<float>()/2.0, 0.0, 1000);
    // out.dispoArma = BAsection::dispoArmaCercle(param["section"]["n"].get<int>(), param["section"]["HA"].get<float>()/1000.0, 0.9/2.0 - 0.1);

    // out.charge.Mx = param["section"]["Mx"].get<float>();
    // out.charge.My = param["section"]["My"].get<float>();
    // out.charge.N = param["section"]["N"].get<float>();
    // out.charge.etat = charge::etatName::FOND;

    // out.verif();
    // out.getFbMoy();
    // out.checkCharges();
    // out.checkMateriaux();

    // out.showOutput();

    // Check inverse matrix
    // mathF::matrix J = {{7.0,2.0,6.0}, {6.0,1.0,5.0}, {4.0,5.0,1.0}};
    // mathF::showMatrix(J);
    // mathF::matrix zeros = {{0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}};
    // cout << mathF::getMatrixDeterminant(J) << endl;
    // mathF::showMatrix(mathF::getInverseMatrix(J, mathF::getMatrixDeterminant(J)));
    // mathF::matrix inverseJ = mathF::getInverseMatrix(J, mathF::getMatrixDeterminant(J));
    // mathF::sum2matrix(J, inverseJ, zeros);
    // mathF::showMatrix(zeros);
    // mathF::showMatrix(mathF::multi2matrix(J, inverseJ));

    std::cout << "END" << endl;
    return 0;
}