#include "rido.h"

using namespace std;

vector<charge::etatName> rido::ELS = {charge::etatName::QP, charge::etatName::CARA, charge::etatName::FREQ, charge::etatName::EB, charge::etatName::EC, charge::etatName::EH, charge::etatName::EE};
vector<charge::etatName> rido::ELU = {charge::etatName::FOND, charge::etatName::ACC};
vector<charge::etatName> rido::EAU = {charge::etatName::EB, charge::etatName::EC, charge::etatName::EH, charge::etatName::EE};

string rido::getAppuiTypeString(appuiType a) {
    if (a == appuiType::buton) {/*cout << "QP" << endl;*/return "buton";}
    if (a == appuiType::bracon) {/*cout << "QP" << endl;*/return "bracon";}
    if (a == appuiType::tirant) {/*cout << "FREQ" << endl;*/return "tirant";}
    if (a == appuiType::dalle) {/*cout << "CARA" << endl;*/return "dalle";}
    cout << "Etat : " << a << " does not exist" << endl;
    return "";
}

rido::rido::rido(string paroiName, string coupeName0, json param, json coupesCsv) {

    coupeName = coupeName0;
    getRioData(param["panneautage"][paroiName]["rido"].get<string>() + "/" + coupeName + ".LST");
    epaisseur = params["Eppm"];
    enrobage = param["materiaux"]["beton"]["enrobage"].get<float>()/100.0f;
    
    // Vérification tirant

    // Get ancrage list
    int i = 0;
    // std::cout << param["materiaux"]["ancrage"]["phi"].dump(4) << std::endl;
    for (auto dia: param["materiaux"]["ancrage"]["phi"]) {
        // std::cout << dia.get<float>() << " " << param["materiaux"]["ancrage"]["n"][i].get<float>() << std::endl;
        ancrageList.insert(std::pair<float, float>(dia.get<float>(), param["materiaux"]["ancrage"]["n"][i].get<float>()));
        i++;
    }

    // for (auto a: ancrageList) {
    //     std::cout << a.first << " " << a.second << std::endl;
    // }

    json paroiParam = param["panneautage"][paroiName];

    // std::cout << coupesCsv.dump(4) << std::endl;
    ratio = coupesCsv["ratio"].get<float>();
    folder = paroiParam["rido"].get<string>();   
    
    // Imperméabilité
    if (coupesCsv.contains("impermeable")) {
        if (coupesCsv["impermeable"].get<int>() == 1) {
            impermeable = true;
        }
    }

    // Armatures
    if (paroiParam.contains("armaDispose")) {
        armaDispo.getArmaDispo(paroiParam["armaDispose"].get<std::string>(), coupeName, params);
        getRatio();
    }
    

    if (paroiParam.contains("armaDispose") & (coupesCsv["arma"].get<int>() == 1)) {
        #ifdef CHECK
        std::cout << coupesCsv.dump(4) << std::endl;
        #endif 
        
        std::cout << "getMateriaux" << std::endl;
        // Get contraintes béton
        getMateriaux(param);
        
        std::cout << "getMoment" << std::endl;
        getMoment(coupesCsv);

        if (coupesCsv["type"].get<string>() == "PMP") {
            std::cout << "getArmatures" << std::endl;
            getArmatures(coupesCsv, paroiParam["armaDispose"].get<std::string>());

            std::cout << "getAdispo" << std::endl;
            getAdispo(charge::etatName::FOND);

            // verifArmatures(charge::etatName::FOND);

            std::cout << "verifArmatures" << std::endl;

            for (auto etat: etats)
                verifArmatures(coupesCsv, etat.first);

        } else if (coupesCsv["type"].get<string>() == "PMT") {
            std::cout << "PMT" << std::endl;
            std::cout << paroiParam["coupes"][coupeName]["elements"].get<std::string>() << std::endl;

            // Pour reprendre les données de la section plate
            std::cout << "getArmatures" << std::endl;
            getArmatures(coupesCsv, paroiParam["armaDispose"].get<std::string>());

            // std::cout << "getAdispoT" << std::endl;
            // getAdispoT(charge::etatName::FOND, paroiParam["coupes"][coupeName]["coorBars"].get<std::string>(), paroiParam["coupes"][coupeName]["dispoBars"].get<std::string>());

            // verifArmatures(charge::etatName::FOND);

            std::cout << "verifArmaturesT" << std::endl;

            for (auto etat: etats)
                verifArmaturesT(coupesCsv, etat.first, paroiParam["coupes"][coupeName]["points"].get<std::string>(), paroiParam["coupes"][coupeName]["elements"].get<std::string>(), paroiParam["coupes"][coupeName]["coorBars"].get<std::string>(), paroiParam["coupes"][coupeName]["dispoBars"].get<std::string>());


                if (paroiParam["coupes"][coupeName].contains("dxfSection")) {
                    dxfSection(paroiParam["coupes"][coupeName]["dxfSection"].get<std::string>(), paroiParam["coupes"][coupeName]["coorBars"].get<std::string>(), paroiParam["coupes"][coupeName]["dispoBars"].get<std::string>());
                }
        }

        getJsonOuput(paroiParam["rido"].get<std::string>() + "/" + coupeName + ".json");
    }   
    
    paramsPanneautage = paroiParam["coupes"][coupeName];
    
    dxfCoupeType();
    // model.showAppuis();
    // model.checkCoucheList();

    // for (auto phase: model.phases) {
    //     std::cout << "Id: " << phase.id << " Mmax: " << phase.Mmax  << " Mmin: " <<phase.Mmin << std::endl;
    // }
};

static void getParams(map<string, float>& params, string line) {
    // cout << line.substr(1) << endl;
    int pos = line.find("#");
    vector<string> expr = stringF::splitStr(line.substr(pos+1), "=");
    // cout << expr[0] << ", " << expr[1] << endl;
    if (stringF::isRealNumber(expr[1])) {
        params.emplace(expr[0], stof(expr[1]));
        // cout << "rido.cpp 74: " << expr[0] << ", " << stof(expr[1]) << endl;
    } else {
        // replace params term
        // cout << "1" << expr[0] << ", " << expr[1] << endl;
        for(auto& p : params) {
            stringF::replaceInplace(expr[1], p.first, to_string(p.second));
        }
        // cout << "2" << expr[0] << ", " << expr[1] << endl;
        const char * c = expr[1].c_str();
        float f = te_interp(c, 0);
        if (! (f != f)) {
            params.emplace(expr[0], f);
        }
    }
};

static void printRidoCodeSection(vector<string> section) {
    for (string line: section) {
        cout << line << endl;
    }
};

void rido::rido::getAppui(vector<string> command, vector<string> commandParams) {

    // stringF::printVectorInLine(commandParams);
    // cout << "commandParams[0] = " << commandParams[0] << endl;
    int codeCommand = stoi(stringF::substr(command[1], 0, 1));
    // cout << "codeCommand = " << codeCommand << endl;

    if (codeCommand == 0) {
        int appuiLit = stoi(stringF::substr(command[1], 2, 1));
        if (appuiList[appuiLit-1].type == appuiType::dalle) {
            appuiList[appuiLit-1].type = appuiType::buton;
        }
    } else {
        appui newAppui;
        newAppui.id = appuiList.size() + 1;
        newAppui.lit = appuiList.size() + 1;
        newAppui.niveau = stof(commandParams[1]);
        newAppui.espacement = stof(commandParams[2]);
        newAppui.angle = stof(commandParams[3]);
        newAppui.precontrainte = stof(commandParams[4]);
        newAppui.K = stof(commandParams[5]);
        newAppui.nToronts = params["NtorS" + to_string(newAppui.lit)];

        if (command[0] == "but") {
            newAppui.type = appuiType::dalle;
        } else if (command[0] == "tir") {
            newAppui.type = appuiType::tirant;
            if (commandParams.size() == 8) {
                newAppui.Lu = stof(commandParams[6]);
                newAppui.Ls = stof(commandParams[7]);
                newAppui.Ll = newAppui.Lu - newAppui.Ls*0.5;
            }
            
        }
        appuiList.push_back(newAppui);
    }
}

void rido::rido::getExcavation(vector<string> command, vector<string> commandParams) {
    int codeCommand = stoi(stringF::substr(command[1], 0, 1));
    excavation newExcavation;

    newExcavation.niveau = stof(commandParams[1]);
    newExcavation.cote = coteParoi::fouille;
    if (codeCommand == 1) {
        newExcavation.cote = coteParoi::terre;
    }
    excavationList.push_back(newExcavation);
}

rido::couche rido::rido::getCoucheData(vector<string> commandParams) {
    couche newCouche;
    newCouche.sup = supCouche;
    newCouche.inf = stof(commandParams[0]);
    newCouche.PVh = stof(commandParams[1]);
    newCouche.PVd = stof(commandParams[2]);
    newCouche.Ka = stof(commandParams[3]);
    newCouche.K0 = stof(commandParams[4]);
    newCouche.Kp = stof(commandParams[5]);
    newCouche.C = stof(commandParams[6]);
    newCouche.phi = stof(commandParams[7]);
    newCouche.Da = stof(commandParams[8]);
    newCouche.Dp = stof(commandParams[9]);
    newCouche.Dp = stof(commandParams[10]);

    string pleNom = "";
    string EmNom = "";

    newCouche.nom = comment;
    pleNom = "ple" + to_string(coucheList.size()+1);
    if (mapF::checkKeyInMap(pleNom, params)) {
        newCouche.ple = params[pleNom];
    }
    EmNom = "EmS" + to_string(coucheList.size()+1);
    if (mapF::checkKeyInMap(EmNom, params)) {
        newCouche.Em = params[EmNom];
    }
    EmNom = "EmSol" + to_string(coucheList.size()+1);
    if (mapF::checkKeyInMap(EmNom, params)) {
        newCouche.Em = params[EmNom];
    }

    supCouche = newCouche.inf;

    return newCouche;
}

void rido::rido::getRioData(string fileName) {
    
    cout << fileName << endl;
    vector<string> lines = fileF::textFile2LinesList(fileName);
    
    // map<string, float> params;
    
    // Command lines
    string ridoCodeSection = "start";
    string commentLine = "";
    vector<string> paroiInertieSection;
    vector<string> couchesSolsSection;
    vector<vector<string>> phaseTables;

    // Phases tables
    int phaseNumber = 0;
    vector<string> numberString0123 {"0", "1", "2", "3"};
    int currentAppuiNappe = 0;
    float currentNappeK = 0.0;

    for (string line: lines) {
        // cout << line << endl;
        // Params lines
        if (ridoCodeSection != "tables") {  // Check for optimisation (avoid regex)

            // ============= Comments ===============
            if (stringF::substr(line, 9, 1) == "*") {
                comment = line.substr(11);
            // ============= Paramètres ===============
            } else if (stringF::substr(line, 9, 1) == "#") {
                getParams(params, line);
            // ============= Commands lines ===============
            } else if (stringF::substr(line, 5, 3) == "...") {
                int pos = line.find("...");
                int lineNumber = stoi(line.substr(0, pos-1));
                vector<string> commandParams = stringF::splitStrSpace(line.substr(pos+4), " ");
                // stringF::printVectorStringInLine(commandParams);
                if (ridoCodeSection == "start") {
                    if ((lineNumber > 1) & (commandParams.size() == 1)) {
                        ridoCodeSection = "couches";
                        couchesSolsSection.push_back(line);
                        supCouche = stof(commandParams[0]);
                    } else {
                        paroiInertieSection.push_back(line);
                    }
                } else if (ridoCodeSection == "couches") {
                    if (commandParams.size() < 10) {
                        ridoCodeSection = "EC";
                        // stringF::printVectorStringInLine(commandParams);
                        params.insert( pair<string, float>("ECrido", stof(commandParams[0])) );
                        params.insert( pair<string, float>("pasDeCalcul", stof(commandParams[1])) );
                    } else {
                        // cout << "2 " << line << endl;
                        couchesSolsSection.push_back(line);
                        coucheList.push_back(getCoucheData(commandParams));
                    }
                } else if (ridoCodeSection == "EC") {
                    ridoCodeSection = "phases";
                } else if (ridoCodeSection == "phases") {
                    vector<string> command = stringF::splitStr(commandParams[0], "(");
                    if ((command[0] == "but") | (command[0] == "tir")) {
                        getAppui(command, commandParams);
                    } else if ((command[0] == "exc") | (command[0] == "ber")) {
                        getExcavation(command, commandParams);
                    }

                    // if (stringF::substr(line, 5, 8) == "... stop") {
                    if (stringF::substr(line, 5, 7) == "... fin") {
                        ridoCodeSection = "tables";
                    }
                } 
            }
        }
        
        if (ridoCodeSection == "tables") {
            if (line.substr(0,21) == "                  * \"") {
                // cout << "rido: " << line << endl;
                phaseNumber += 1;
                ridoPhase newPhase;
                newPhase.id = phaseNumber;
                newPhase.description = line.substr(21, int(line.size()) - 1);
                newPhase.niveauExcavationFouille = params["plateforme"];
                newPhase.niveauExcavationTerre = params["TN"];
                // cout << newPhase.description << endl;
                phases.push_back(newPhase);
            }
//          111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
// 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
// * POUR LA NAPPE DE BUTONS   N°  7     NOUVELLE RIGIDITE  =   32000.000 T/m 
// * POSE NAPPE DE BUTONS   N°  2                 NIVEAU                   =      26.250 m       
//                                                ESPACEMENT               =       1.000 m       
//                                                INCLINAISON              =       0.000 DEGRES
//                                                PRECHARGE                =       0.000 T       
//                                                RIGIDITE                 =   72000.000 T/m     
//                                                LONGUEUR UTILE           =       0.000 m    
// * POSE NAPPE DE TIRANTS  N°  1                 NIVEAU                   =      34.000 m       
//                                                ESPACEMENT               =       2.500 m       
//                                                INCLINAISON              =      35.000 DEGRES
//                                                PRECHARGE                =     -90.000 T       
//                                                RIGIDITE                 =    1096.875 T/m     
//                                                LONGUEUR UTILE           =      16.000 m       
//                                                LONGUEUR DE SCELLEMENT   =       2.000 m       
//                                                LIAISON UNILATERALE : ECRAN LIBRE VERS SOL 1
//                                                COMPOSANTE VERTICALE ORIENTEE VERS LE BAS  


            if (line.substr(0,12) == "* POSE NAPPE") {
                // std::cout << line.substr(28,5) << std::endl;
                currentAppuiNappe = stringF::stof(line.substr(28,5));
                
            }
            if (line.size() > 88) {
                if (line.substr(47,8) == "RIGIDITE") {
                    // std::cout << line.substr(73,13) << std::endl;
                    appuiList[currentAppuiNappe - 1].K = stringF::stof(line.substr(73,13));
                }
            }
            if (line.substr(0,15) == "* POUR LA NAPPE") {
                currentAppuiNappe = stringF::stof(line.substr(31,5));
                // std::cout << line.substr(31,5) << std::endl;
                // std::cout << line.substr(38,17) << std::endl;
                // std::cout << line.substr(58,13) << std::endl;
                if (line.substr(38,17) == "NOUVELLE RIGIDITE") {
                    appuiList[currentAppuiNappe - 1].K = stringF::stof(line.substr(58,13));
                }
            }

            if (stringF::substr(line, 0, 1) == "|") {
                if (stringF::stringInStringVector(line.substr(63,1), numberString0123)) {
                    // cout << line.substr(63,3) << endl;
                    // cout << line << endl;
                    
                    ridoPhase& currentPhase = phases[int(phases.size()) - 1];
                    ridoTableLine newLine;
                    
                    currentPhase.textTables.push_back(line);

                    // start = [2, 12, 20, 27, 35, 43, 51, 62, 66, 72, 81, 91,  94, 103, 110, 119, 123]
                    // end =   [9, 19, 27, 35, 43, 51, 59, 66, 72, 81, 88, 94, 101, 109, 117, 122, 130]
                    if (line.substr(2,7) == "       ") {
                        newLine.niveau = currentPhase.table[int(currentPhase.table.size()) - 1].niveau;
                    } else {
                        newLine.niveau = stringF::stof(line.substr(2,  7));
                    }
                    
                    // try{
                        newLine.deplacement     = stringF::stof(line.substr(12, 7));
                        newLine.rotation        = stringF::stof(line.substr(20, 7));
                        newLine.solli.M         = stringF::stof(line.substr(27, 7));
                        newLine.solli.V         = stringF::stof(line.substr(35, 7));
                        newLine.solli.N         = stringF::stof(line.substr(43, 7));
                        newLine.chargeRepartie  = stringF::stof(line.substr(51, 7));
                        newLine.sol1.etat       = stringF::stof(line.substr(62, 4));
                        newLine.sol1.pression   = stringF::stof(line.substr(66, 6));
                        newLine.sol1.surcharge  = stringF::stof(line.substr(72, 8));
                        newLine.sol1.kh         = stringF::stof(line.substr(81, 7));
                        newLine.sol2.etat       = stringF::stof(line.substr(91, 3));
                        newLine.sol2.pression   = stringF::stof(line.substr(94, 7));
                        newLine.sol2.surcharge  = stringF::stof(line.substr(103,7));
                        newLine.sol2.kh         = stringF::stof(line.substr(110,7));
                    
                    currentPhase.Mmax = max(currentPhase.Mmax, newLine.solli.M);
                    currentPhase.Mmin = min(currentPhase.Mmin, newLine.solli.M);

                    if (line.substr(27, 7) != "       ") {
                        currentPhase.dMax = max(currentPhase.dMax, newLine.deplacement);
                        if (newLine.niveau == params["TN"]) {
                            currentPhase.dTeteMax = max(currentPhase.dTeteMax, newLine.deplacement);
                        }
                    }
                    
                    if (newLine.sol2.etat == 0) {
                        currentPhase.niveauExcavationFouille = min(currentPhase.niveauExcavationFouille, newLine.niveau);
                    }
                    if (newLine.sol1.etat == 0) {
                        currentPhase.niveauExcavationTerre = min(currentPhase.niveauExcavationTerre, newLine.niveau);
                    } 
                    // }
                    // catch (std::exception const & e)
                    // {
                    //     std::cout << e.what() << std::endl;
                    // }

                    // Efforts appuis
                    if (line.substr(119, 3) != "   ") {
                        // cout << line << endl;
                        int appuiId = stoi(line.substr(119, 3));
                        float appuiEffort = stringF::stof(line.substr(123,7));
                        currentPhase.effortsAppuis.insert(make_pair(appuiId, make_pair(newLine.niveau, appuiEffort)));

                        appuiList[appuiId - 1].F = min(appuiList[appuiId - 1].F, appuiEffort);
                        appuiList[appuiId - 1].Flist.insert(pair<int, float>(currentPhase.id, appuiEffort));
                        currentPhase.appuiList.push_back(appuiList[appuiId - 1]);
                    }

                    currentPhase.table.push_back(newLine);
                }
            }

        }
        // regex word_regex("(\\s*,\\s*)+");
        // return !regex_match(line, word_regex);
    }

    // Check output
    // cout << "paroiInertieSection" << endl;
    // stringF::printVectorString(paroiInertieSection);
    // cout << "couchesSolsSection" << endl;
    // stringF::printVectorString(couchesSolsSection);
    // cout << "ECrido = " << ECrido << ", pasDeCalcul = " << pasDeCalcul << endl;

    // model.phases[int(model.phases.size()) - 1].showEffortsAppuis();
    // stringF::printVectorString(model.phases[int(model.phases.size()) - 1].textTables);
    // model.phases[int(model.phases.size()) - 1].showTable();
    // mapF::printMap(params);
    // printRidoCodeSection(paroiInertieSection)
    // if (mapF::checkKeyInMap("Eppm", params)) {
    //     epaisseur = params["Eppm"];
    // }
}

static bool checkNiveauInRidoTable(float niv, vector<rido::ridoTableLine> table) {
    for (rido::ridoTableLine line: table)
        if (line.niveau == niv) {
            return true;
        }
    return false;
}

rido::etatNiveau getNiveauHaut(float niv, map<float, rido::etatNiveau> lines) {
    rido::etatNiveau niveauHaut;
    float deltaNiveauMax = 1000000.0;
    // int i = 0;
    // cout << "START: getNiveauHaut : " << endl;
    for (auto line: lines) {
        // if (i == 0) {niveauHaut = line.second; i = 1;}
        if ((line.first >= niv) & ((line.first - niv) < deltaNiveauMax)) {
            deltaNiveauMax = line.first - niv;
            niveauHaut = line.second;
            // cout << "niv = " << niv << ", line.first: " << line.first << ", niveauHaut.niveau: " << niveauHaut.niveau << ", deltaNiveauMax = " << deltaNiveauMax << endl;

        }
    }
    // cout << "END: getNiveauHaut : " << deltaNiveauMax << ", ";
    // cout << "END: getNiveauHaut : niv = " << niv  << ", niveauHaut.niveau: " << niveauHaut.niveau << ", deltaNiveauMax = " << deltaNiveauMax << endl;
    return niveauHaut;
}

rido::etatNiveau getNiveauBas(float niv, map<float, rido::etatNiveau> lines) {
    rido::etatNiveau niveauBas;
    float deltaNiveauMax = 1000000.0;
    // int i = 0;
    for (auto line: lines) {
        // if (i == 0) {niveauBas = line.second; i = 1;}
        if ((line.first <= niv) & ((niv - line.first) < deltaNiveauMax)) {
            deltaNiveauMax = niv - line.first;
            niveauBas = line.second;
        }
    }
    // cout << "getNiveauBas : " << deltaNiveauMax << ", ";
    return niveauBas;
}

std::pair<float, rido::etatNiveau> interpolerNewEtatNiveau(float niv, map<float, rido::etatNiveau> lines1, map<float, rido::etatNiveau> lines2) {

    rido::etatNiveau niveauHaut1 = getNiveauHaut(niv, lines1);
    rido::etatNiveau niveauBas1 = getNiveauBas(niv, lines1);
    rido::etatNiveau niveauHaut2 = getNiveauHaut(niv, lines2);
    rido::etatNiveau niveauBas2 = getNiveauBas(niv, lines2);

    // cout << endl;
    rido::etatNiveau newNiveau;

    // if (niveauHaut1.niveau != niveauBas1.niveau) {
        
        newNiveau.niveau = niv;
        newNiveau.Mmax = max(   mathF::interpoler(niveauHaut1.niveau, niveauHaut1.Mmax, niveauBas1.niveau, niveauBas1.Mmax, niv), 
                                mathF::interpoler(niveauHaut2.niveau, niveauHaut2.Mmax, niveauBas2.niveau, niveauBas2.Mmax, niv));
        newNiveau.Mmin = min(   mathF::interpoler(niveauHaut1.niveau, niveauHaut1.Mmin, niveauBas1.niveau, niveauBas1.Mmin, niv),
                                mathF::interpoler(niveauHaut2.niveau, niveauHaut2.Mmin, niveauBas2.niveau, niveauBas2.Mmin, niv));
        newNiveau.Vmax = max(   mathF::interpoler(niveauHaut1.niveau, niveauHaut1.Vmax, niveauBas1.niveau, niveauBas1.Vmax, niv), 
                                mathF::interpoler(niveauHaut2.niveau, niveauHaut2.Vmax, niveauBas2.niveau, niveauBas2.Vmax, niv));
        newNiveau.Vmin = min(   mathF::interpoler(niveauHaut1.niveau, niveauHaut1.Vmin, niveauBas1.niveau, niveauBas1.Vmin, niv),
                                mathF::interpoler(niveauHaut2.niveau, niveauHaut2.Vmin, niveauBas2.niveau, niveauBas2.Vmin, niv));
        
    // }

    // cout << "niveau = " << niv << ", niveauHaut1.niveau = " << niveauHaut1.niveau << ", niveauBas1.niveau = " << niveauBas1.niveau <<", niveauHaut2.niveau = " << niveauHaut2.niveau << ", niveauBas2.niveau = " << niveauBas2.niveau << endl;
    // cout << "Mmax = " << newNiveau.Mmax << ", niveauHaut1.Mmax = " << niveauHaut1.Mmax << ", niveauBas1.Mmax = " << niveauBas1.Mmax <<", niveauHaut2.Mmax = " << niveauHaut2.Mmax << ", niveauBas2.Mmax = " << niveauBas2.Mmax << endl;


    return std::pair<float, rido::etatNiveau>(niv, newNiveau);
}

void rido::rido::getNewEtat(vector<int> phasesList, float ponderation, charge::etatName nom) {
    ridoEtat newEtat;
    // newEtat.nom = charge::getEtatNameString(nom);
    newEtat.nom = nom;
    newEtat.phasesList = phasesList;
    newEtat.ponderation = ponderation;
    map<float, etatNiveau> niveaux;
    // set<float> niveauxSet;

    // // Contruct a new set of niveaux of all tables
    // for (int phaseNumber: phasesList) {
    //     ridoPhase& currentPhases = phases[phaseNumber-1];
    //     for (ridoTableLine line: currentPhases.table)
    //         niveauxSet.insert(line.niveau);
    // }

    // 
    /*
    // Insert new line if niveau not in table
    for (int phaseNumber: phasesList) {
        ridoPhase& currentPhases = phases[phaseNumber];
        for (float niv: niveauxSet) {
            if (!checkNiveauInRidoTable(niv, currentPhases.table)) {
                interpolerNewRidoTableLine(niv, currentPhases.table);
            }
        }
    }
    */

    #ifdef CHECKEtat
    std::cout << charge::getEtatNameString(nom) << std::endl;
    stringF::printVectorInLine(phasesList);
    #endif

    // Enveloppe des phases
    for (int phaseNumber: phasesList) {
        // std::cout << "Phase " << phaseNumber << std::endl;
        ridoPhase& currentPhases = phases[phaseNumber-1];

        // for (ridoTableLine line: currentPhases.table) {
        for (ridoTableLine line: phases[phaseNumber-1].table) {
            if (!mapF::checkKeyInMap(line.niveau, newEtat.table)) {
                etatNiveau newLine;
                newLine.niveau = line.niveau;
                newLine.Mmax = max(line.solli.M*ponderation, 0.0f);
                newLine.Mmin = min(line.solli.M*ponderation, 0.0f);
                newLine.Vmax = line.solli.V*ponderation;
                newLine.Vmin = line.solli.V*ponderation;
                // newLine.teff = params["Eppm"];

                
                
                // if ( params.find("rayonMoy") == params.end() ) {
                //     // not found
                //     newLine.contrainteH = 0.0; // mm*MPa
                // } else {
                //     // found
                //     // Eppaisseur effective
                //     // 
                //     // newLine.teff = max(params["Eppm"] - max((params["plateforme"] - newLine.niveau), (float) 0.0)*params["inclinaison"]/((float) 100.0), (float) 0.0);
                //     newLine.contrainteH = line.deplacement*betonMateriaux.Eyoung[nom]/params["rayonMoy"]/1000.0; // mm*MPa
                // }
                
                

                // if (nom == "FOND") {
                //     std::cout << phaseNumber << "rido.cpp 460: " << "line.solli.M=" << line.solli.M << ", niveau=" << line.niveau << ", Mmax=" << newLine.Mmax << std::endl;
                // }
                
                newEtat.table.insert(pair<float, etatNiveau>(line.niveau, newLine));

            } else {
                etatNiveau& currentNiveau = newEtat.table[line.niveau];

                // if (nom == "FOND") {
                //     std::cout << phaseNumber << "rido.cpp 466: " << "line.solli.M=" << line.solli.M << ", niveau=" << line.niveau << ", Mmax=" << currentNiveau.Mmax << std::endl;
                // }

                if ((line.solli.M*ponderation) > currentNiveau.Mmax) {
                    currentNiveau.Mmax = line.solli.M*ponderation;
                    // std::cout << phaseNumber << "rido.cpp 476: " << "line.solli.M=" << line.solli.M << ", niveau=" << line.niveau << ", Mmax=" << currentNiveau.Mmax << std::endl;
                }
                if ((line.solli.M*ponderation) < currentNiveau.Mmin) {
                    currentNiveau.Mmin = line.solli.M*ponderation;
                }
                if ((line.solli.V*ponderation) > currentNiveau.Vmax) {
                    currentNiveau.Vmax = line.solli.V*ponderation;
                }
                if ((line.solli.V*ponderation) < currentNiveau.Vmin) {
                    currentNiveau.Vmin = line.solli.V*ponderation;
                }
            }

            currentPhases.Mmax = max(currentPhases.Mmax, line.solli.M*ponderation);
            currentPhases.Mmin = min(currentPhases.Mmax, line.solli.M*ponderation);

        }
            
    }

    // for (auto line: newEtat.table) {
    //     cout << "rido.cpp 377: " << line.second.niveau << ", M=" << line.second.Mmax << endl;
    // }
    etats.insert( pair<charge::etatName, ridoEtat>(nom, newEtat));
    // Decaler des moments
    
    // setF::printSetInLine(niveauxSet);
}

void rido::rido::decalerMomentEtat(float distance, charge::etatName nom) {
    if (!mapF::checkKeyInMap(nom, etats)) {
        cout << "L'etat " + charge::getEtatNameString(nom) + " n'existe pas" << endl;
        // return false;
    } else {
        map<float, etatNiveau> etatDecaleVersLeHaut;
        map<float, etatNiveau> etatDecaleVersLeBas;
        set<float> nouveauxNiveaux;

        float niveauMax = -1000000.0;
        float niveauMin = 1000000.0;
        for (auto line: etats[nom].table) {
            niveauMax = max(niveauMax, line.first);
            niveauMin = min(niveauMin, line.first);
        }

        // cout << "niveauMax = " << niveauMax << ", niveauMin = " << niveauMin << endl;

        for (auto line: etats[nom].table) {
            nouveauxNiveaux.insert(line.first);
            nouveauxNiveaux.insert(min(line.first + distance, niveauMax));
            nouveauxNiveaux.insert(max(line.first - distance, niveauMin));
            etatNiveau newNiveauHaut;
            newNiveauHaut = line.second;
            newNiveauHaut.niveau = line.first + distance;
            etatDecaleVersLeHaut.insert( pair<float, etatNiveau>(line.first + distance, newNiveauHaut));
            etatNiveau newNiveauBas;
            newNiveauBas = line.second;
            newNiveauBas.niveau = line.first - distance;
            etatDecaleVersLeBas.insert( pair<float, etatNiveau>(line.first - distance, newNiveauBas));
        }
        for (float niv: nouveauxNiveaux) {
            etats[nom].tableDecalee.insert( interpolerNewEtatNiveau(niv, etatDecaleVersLeHaut, etatDecaleVersLeBas) );
        }
    }

}

rido::coteParoi rido::armaDispo::getCote(string cote) {
    coteParoi cote1 = fouille;
    if (cote == "F") {
        cote1 = fouille;
    } else if (cote == "T") {
        cote1 = terre;
    } else {
        cout << "Côté \"" << cote << "\" n'existe pas" << endl;
    }
    return cote1;
};

void rido::rido::getMoment(json coupeParam) {
    // Enveloppes des moments
    string none = "x";
    if (coupeParam.contains("CARA")) {
        if (coupeParam["CARA"].get<std::string>() != none) {
            #ifdef CHECKEtat
            cout << "CARA: ";
            stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["CARA"]));
            #endif
            getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["CARA"]), 1.0, charge::etatName::CARA);
            decalerMomentEtat(epaisseur*0.8, charge::etatName::CARA);
        }
    }
    
    if (coupeParam.contains("FOND")) {
        if (coupeParam["FOND"].get<std::string>() != none) {
            #ifdef CHECKEtat
            cout << "FOND: ";
            std::cout << getPhasesNumber() << std::endl;
            stringF::printVectorInLine(mathF::rangeI(1,getPhasesNumber()+1,1));
            stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["FOND"]));
            #endif
            getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["FOND"]), 1.35, charge::etatName::FOND);
            decalerMomentEtat(epaisseur*0.8, charge::etatName::FOND);
        }
    }
    
    if (coupeParam.contains("ACC")) {
        if (coupeParam["ACC"].get<std::string>() != none) {
            #ifdef CHECKEtat
            cout << "ACC: ";
            stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["ACC"]));
            #endif
            getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["ACC"]), 1.0, charge::etatName::ACC);
            decalerMomentEtat(epaisseur*0.8, charge::etatName::ACC);
        }
    }

    // cout << "coupeParam.contains(\"EB\") " << coupeParam.contains("EB") << endl;
    if (coupeParam.contains("EB")) {
        if (coupeParam["EB"].get<std::string>() != none) {
            #ifdef CHECKEtat
            cout << "EB: ";
            stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["EB"]));
            #endif
            getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["EB"]), 1.0, charge::etatName::EB);
            decalerMomentEtat(epaisseur*0.8, charge::etatName::EB);
        }
    }
    if (coupeParam.contains("EH")) {
        if (coupeParam["EH"].get<std::string>() != none) {
            #ifdef CHECKEtat
            cout << "EH: ";
            stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["EH"]));
            #endif
            getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["EH"]), 1.0, charge::etatName::EH);
            decalerMomentEtat(epaisseur*0.8, charge::etatName::EH);
        }
    }
    if (coupeParam.contains("EE")) {
        if (coupeParam["EE"].get<std::string>() != none) {
            #ifdef CHECKEtat
            cout << "EE: ";
            stringF::printVectorInLine(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["EE"]));
            #endif
            getNewEtat(stringF::listStr2IdList(mathF::rangeI(1,getPhasesNumber()+1,1), coupeParam["EE"]), 1.0, charge::etatName::EE);
            decalerMomentEtat(epaisseur*0.8, charge::etatName::EE);
        }
    }
};
