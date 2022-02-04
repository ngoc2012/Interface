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

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

// const string usersFolder = "C:\\Users\\mnnguyen\\Documents\\Gcpm\\users\\";

map<string, float> getInputData(char **argv, int startId) {
    
    string arg2(argv[startId]);
    vector<string> strArray = stringF::splitStr(arg2, ",");

    map<string, float> inputData;
    int i = startId + 1;
    for(string param : strArray) {
        inputData.emplace(param, atof(argv[i]));
        i++;
    }
    return inputData;
}

void online(char **argv) {
    string arg2(argv[2]);
    if (arg2 == "BASecRecDimELU") {
        map<string, float> inputData = getInputData(argv, 3);
        // printMap(inputData);
        // BAparoi::calculArmaELU(inputData);
    }
}

void offline(char **argv) {
    string arg2(argv[2]);
    if (arg2 == "BASecRecDimELU") {
        string folderName(argv[3]);
        string fileName(argv[4]);
        json inputData = jsonF::importJson(folderName + fileName);
    } 
    else if (arg2 == "armaRIDO") {
        // .\editor.exe off armaRIDO ngoc2012 "Puteaux\Ind5\param" paroi1 0
        string userName(argv[3]);
        string fileName(argv[4]);
        json param = jsonF::importJson(usersFolder + userName + "\\" + fileName + ".json");
        string paroiName(argv[5]);
        int index = atoi(argv[6]);
        vector<string> keys = jsonF::getKeys(param["panneautage"][paroiName]["coupes"]);
        
        json coupesCsv = csv::csv2MapJsonList(param["panneautage"][paroiName]["coupesCsv"], 1, 1);
        
        string extension = fileF::getFileExtension(param["panneautage"][paroiName]["coffrage"]);

        rido::paroi newParoi;

        int i;
        if (extension == "dxf") {
            dxf::readDXF readDXF;
            readDXF.readDXFfile(param["panneautage"][paroiName]["folder"].get<string>() + "/" + param["panneautage"][paroiName]["coffrage"].get<string>());

            // readDXF.showDXFentities();

            // vector<dxf::entity> entities = dxf::readDXF();
            // dxf::showDXFentities(entities);
            vector<string> layers = stringF::splitStrSpace(param["panneautage"][paroiName]["dxfLayers"].get<string>(), ".");
            vector<string> entitiesTypes = stringF::splitStrSpace(param["panneautage"][paroiName]["dxfEntities"].get<string>(), ".");
            
            newParoi.getEdgeList(readDXF.entityList, layers, entitiesTypes);

            //dxf::test(param["panneautage"][paroiName]["folder"].get<string>());
            
        } else if (extension == "csv") {
            // newParoi.edgesList = csv::csv2JsonList(param["panneautage"][paroiName]["folder"].get<string>() + "/" + param["panneautage"][paroiName]["coffrage"].get<string>(), 1, 2);
        }
        
        int id = 0;
        // vector<string> keysForCheck = {"CARA", "FOND", "ACC"}
        for (string coupeName: keys) {
            if (coupesCsv[coupeName]["calcul"] == 1) {
                // RIDO: RIO LISTING
                cout << "Coupe: " << coupeName << endl;
                // cout << "Folder: " << param["panneautage"][paroiName]["coupes"][keys[index]]["folder"] << endl;
                // cout << "File rido: " << param["panneautage"][paroiName]["coupes"][keys[index]]["folder"].get<string>() + "/" + keys[index] + ".RIO" << endl;
                // cout << "File listing: " << param["panneautage"][paroiName]["coupes"][keys[index]]["folder"].get<string>() + "/" + keys[index] + ".LST" << endl;
                
                rido::rido model(coupeName, param["panneautage"][paroiName]["coupes"][coupeName]["folder"].get<string>() + "/" + coupeName + ".LST");                
                
                model.folder = param["panneautage"][paroiName]["coupes"][coupeName]["folder"].get<string>();
                model.ratio = coupesCsv[coupeName]["ratio"].get<float>();

                // cout << "editor.cpp 109 - Epaissseur: " << model.params["Eppm"] << endl;
                float epaisseur = model.params["Eppm"];
                model.getMateriaux(param);

                // Enveloppes des moments
                if (coupesCsv[coupeName]["CARA"].get<string>() != "x") {
                    // cout << "CARA: ";
                    // stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["CARA"]));
                    model.getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["CARA"]), 1.0, "CARA");
                    model.decalerMomentEtat(epaisseur*0.8, "CARA");
                }
                
                
                if (coupesCsv[coupeName]["FOND"].get<string>() != "x") {
                    // cout << "FOND: ";
                    // stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["FOND"]));
                    model.getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["FOND"]), 1.35, "FOND");
                    model.decalerMomentEtat(epaisseur*0.8, "FOND");
                }
                
                
                if (coupesCsv[coupeName]["ACC"].get<string>() != "x") {
                    // cout << "ACC: ";
                    // stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["ACC"]));
                    model.getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["ACC"]), 1.0, "ACC");
                    model.decalerMomentEtat(epaisseur*0.8, "ACC");
                }

                // cout << "coupesCsv[coupeName].contains(\"EB\") " << coupesCsv[coupeName].contains("EB") << endl;
                if (coupesCsv[coupeName].contains("EB")) {
                    if (coupesCsv[coupeName]["EB"].get<string>() != "x") {
                        // cout << "EB: ";
                        // stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["EB"]));
                        model.getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["EB"]), 1.0, "EB");
                        model.decalerMomentEtat(epaisseur*0.8, "EB");
                    }
                }
                if (coupesCsv[coupeName].contains("EH")) {
                    if (coupesCsv[coupeName]["EH"].get<string>() != "x") {
                        // cout << "EH: ";
                        // stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["EH"]));
                        model.getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["EH"]), 1.0, "EH");
                        model.decalerMomentEtat(epaisseur*0.8, "EH");
                    }
                }
                if (coupesCsv[coupeName].contains("EE")) {
                    if (coupesCsv[coupeName]["EE"].get<string>() != "x") {
                        // cout << "EE: ";
                        // stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["EE"]));
                        model.getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,model.getPhasesNumber()+1,1), coupesCsv[coupeName]["EE"]), 1.0, "EE");
                        model.decalerMomentEtat(epaisseur*0.8, "EE");
                    }
                }

                // Armatures
                if (param["panneautage"][paroiName].contains("armaDispose")) {
                    
                    // Armatures disposées
                    // rido::armaDispo armaDispo;
                    // cout << param["panneautage"][paroiName]["armaDispose"] << endl;
                    model.armaDispo.getArmaDispo(param["panneautage"][paroiName]["armaDispose"], model.coupeName);
                    // model.armaDispo.showArma(model.armaDispo.getArmaCoupe("COUPE1_2butons_Note", coteParoi::fouille));
                    // model.armaDispo.showArma();
                    
                    model.getContrainteDTU(model.contrainteDTU.contraintes, "CARA");
                    // model.getContrainteDTU2020(contrainteDTU.contraintes, "EB");
                    // model.getContrainteDTU2020(contrainteDTU.contraintes, "EC");
                    // model.checkContrainteArmaturesELS(charge::etatName::CARA, coteParoi::fouille);

                    // contrainteDTU.checkContrainte();

                    model.getArmatures("CARA");
                    model.getArmatures("FOND");
                    model.getArmatures("ACC");
                    
                    if (coupesCsv[coupeName].contains("EB")) {
                        model.getArmatures("EB");
                    }

                    if (coupesCsv[coupeName].contains("EH")) {
                        model.getArmatures("EH");
                    }

                    if (coupesCsv[coupeName].contains("EE")) {
                        model.getArmatures("EE");
                    }

                    model.getJsonOuput(param["panneautage"][paroiName]["coupes"][coupeName]["folder"].get<string>() + "/" + coupeName + ".json");
                }   
                
                
                //    coupeList.insert(pair<string, rido::rido>(coupeName, model));
                model.paramsPanneautage = param["panneautage"][paroiName]["coupes"][coupeName];
                
                model.dxfCoupeType();
                // model.showAppuis();
                // model.checkCoucheList();

                model.id = id;
                newParoi.coupeList.push_back(model);
                id++;
            }
        }
        
        // std::cout << "smt" << endl;
        if (param["panneautage"][paroiName]["dxfPanneautage"].get<bool>()) {
            // newParoi.pelle = param["benneList"][0];
            newParoi.params = param["panneautage"][paroiName];
            if (CHECK) {std::cout << "Panneautage" << endl;}
            newParoi.panneautage();
            // newParoi.checkPanneauList();
            if (CHECK) {std::cout << "panneautageMetre" << endl;}
            newParoi.panneautageMetre(param["panneautage"][paroiName]["folder"].get<string>() + "/" + paroiName + "_metre.csv");

            std::cout << "getLiernes" << endl;
            newParoi.getLiernes(param["panneautage"][paroiName]["liernes"].get<string>());

            if (param["panneautage"][paroiName]["buton"].get<bool>() & param["panneautage"][paroiName].contains("butonDispo")) {
                // std::cout << "getLiernes" << endl;
                newParoi.materiaux = param["materiaux"];
                json Cprofiles = csv::csv2JsonList("C:\\Users\\mnnguyen\\Documents\\References\\profiles_C.csv", 1, 2);
                newParoi.butonnage(param["panneautage"][paroiName]["butonDispo"].get<string>(), Cprofiles);
                // newParoi.checkButonList();
            }
            
            std::cout << "dxfPanneautage" << endl;
            newParoi.dxfPanneautage(param["panneautage"][paroiName]["folder"].get<string>() + "/" + param["panneautage"][paroiName]["dxfFilename"].get<string>());
            
            newParoi.getJson(param["panneautage"][paroiName]["folder"].get<string>() + "/" + paroiName + ".json");
        }
        
    }
    
    
    
}


int main(int argc, char **argv)
{
    // for(int i = 0; i < argc; ++i) cout << argv[i] << '\n';
    string arg1(argv[1]);
    
    // Web version
    if (arg1 == "on") {
        online(argv);
    } else if (arg1 == "off") {
        offline(argv);
    }

    return 0;
}