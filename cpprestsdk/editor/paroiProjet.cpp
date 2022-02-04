#include <iostream>
#include <fstream>  // ifstream
#include <vector>
#include <string>
#include <map>
#include <regex>
// #include <iterator>
// #include <algorithm>

#include <math.h>       /* sqrt */

#include "include/const.h"
#include "include/fileF.h"
#include "include/stringF.h"
#include "include/jsonF.h"
#include "include/csv.h"
#include "include/mathF.h"
#include "include/dxf.h"
#include "include/rido.h"
#include "include/beton.h"
#include "include/arma.h"
#include "include/pieu.h"
#include "include/BAsection.h"

// Not necessery includes
// #include <typeinfo>

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;


int main(int argc, char **argv)
{
    // .\paroiProjet.exe ngoc2012 "Warde\PieuxInd0\param" paroi1
    std::string userName(argv[1]);
    std::string fileName(argv[2]);
    
    json param = jsonF::importJson(usersFolder + userName + "\\" + fileName + ".json");
    std::string paroiName(argv[3]);

    json coupesCsv = csv::csv2MapJsonList(param["panneautage"][paroiName]["coupesCsv"], 1, 1);
    
    rido::paroi newParoi;
    newParoi.getEdgesList(param["panneautage"][paroiName]);

    int id = 0;
    // for (string coupeName: jsonF::getKeys(param["panneautage"][paroiName]["coupes"])) {
    for (auto c: coupesCsv) {
        string coupeName = c["coupes"];
        if (coupesCsv[coupeName]["calcul"] == 1) {
            // RIDO: RIO LISTING
            cout << "Coupe: " << coupeName << endl;
            rido::rido model(paroiName, coupeName, param, coupesCsv[coupeName]);
            model.id = id;
            std::cout << "Epaisseur: " << model.epaisseur << std::endl;
            newParoi.coupeList.push_back(model);
            id++;
        }
    }
    
    if (param["panneautage"][paroiName]["dxfPanneautage"].get<bool>()) {
        newParoi.params = param["panneautage"][paroiName];

        // #ifdef CHECKParoiProjet
        std::cout << "Panneautage" << std::endl;
        // #endif

        newParoi.panneautage();
        // newParoi.checkPanneauList();

        std::cout << "getCages" << std::endl;
        newParoi.getCages(param["materiaux"]["beton"]["enrobage"].get<float>());

        if (param["panneautage"][paroiName].contains("liernes")) {
            std::cout << "getLiernes" << endl;
            newParoi.getLiernes(param["panneautage"][paroiName]["liernes"].get<std::string>());
        }

        if (param["panneautage"][paroiName]["metre"]) {
            
            // #ifdef CHECKParoiProjet
            std::cout << "panneautageMetre" << std::endl;
            // #endif

            newParoi.panneautageMetre(param["panneautage"][paroiName]["folder"].get<std::string>() + "/" + paroiName + "_metre.csv");

            newParoi.panneautageMetrePanneaux(param["panneautage"][paroiName]["folder"].get<std::string>() + "/" + paroiName + "_metre_pan.csv");

            newParoi.panneautagePrix(param["panneautage"][paroiName]["folder"].get<std::string>() + "/" + paroiName + "_prix.csv");

        }

        if (param["panneautage"][paroiName].contains("recapTirant")) {
            
            // #ifdef CHECKParoiProjet
            std::cout << "recapTirant" << std::endl;
            // #endif

            newParoi.recapTirant(param["panneautage"][paroiName]["recapTirant"].get<std::string>());

        }
        
        // if (param["panneautage"][paroiName]["buton"].get<bool>() & param["panneautage"][paroiName].contains("butonDispo")) {
        if (param["panneautage"][paroiName].contains("buton")) {
            newParoi.materiaux = param["materiaux"];
            json Cprofiles = csv::csv2JsonList("C:\\Users\\mnnguyen\\Documents\\References\\profiles_C.csv", 1, 2);

            newParoi.butonnage(param["panneautage"][paroiName]["buton"].get<std::string>(), Cprofiles);
            // newParoi.checkButonList();
        }

        if (param["panneautage"][paroiName].contains("bracon")) {
            newParoi.materiaux = param["materiaux"];
            json Cprofiles = csv::csv2JsonList("C:\\Users\\mnnguyen\\Documents\\References\\profiles_C.csv", 1, 2);

            newParoi.braconnage(param["panneautage"][paroiName]["bracon"].get<std::string>(), Cprofiles);
            // newParoi.checkButonList();
        }
        
        std::cout << "dxfPanneautage" << std::endl;

        newParoi.dxfPanneautage(param["panneautage"][paroiName]["folder"].get<std::string>() + "/" + param["panneautage"][paroiName]["dxfFilename"].get<string>());
        
        newParoi.getJson(param["panneautage"][paroiName]["folder"].get<std::string>() + "/" + paroiName + ".json");

        // newParoi.getCagesDxf(param["materiaux"]["beton"]["enrobage"].get<float>());
    }

    
    
    std::cout << "END" << std::endl;
    return 0;
}