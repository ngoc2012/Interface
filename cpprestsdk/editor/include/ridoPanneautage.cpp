#include "const.h"
#include "rido.h"

json rido::cage::getJson() {
    json armaJson = json::array();
    for (auto a: arma)
        armaJson.push_back(a.getJson());

    return json {
        {"id", id},
        {"label", label},
        {"length", length},
        {"arma", armaJson}
    };
}

json rido::panneau::getJson(vector<cage>& cageList0) {
    // sdfqsdf
    json panneauJson = json {
        {"id", id},
        {"label", label},
        {"length", length},
        {"nCages", nCages},
    };

    panneauJson["cages"] = json::array();

    for (auto cageId: cageList) {
        // std::cout << cageId << " " << coupe.id << " " << pan.id << std::endl;
        panneauJson["cages"].push_back(cageList0[cageId - 1].getJson());
    }

    return panneauJson;
}

void rido::paroi::getJson(string fileName) {
    json paroiJson = json::object();
    paroiJson["butonList"] = json::array();
    paroiJson["recap"] = json::object();
    int nButons = 0;
    float poidsButons = 0.0;
    float poidsPlatines = 0.0;
    int nTiges = 0;
    float poidsBeches = 0.0;
    // paroiJson["recap"]["butons"][""] = 0;
    for (butonMetal but: butonList) {
        
		nButons++;
        // poidsButons += but.LA*but.poids;
        poidsButons += but.poids;
		// 'poidsButon': sum([but['LA']/1000.0*but['profile']['G (kg/m)'] for but in data['butons']]),
		// 'poidsPlatines': sum([but['platine1']['Hp0']*but['platine1']['Lp0']*but['platine1']['tp']/1000.0*7850 for but in data['butons']]) + sum([but['platine2']['Hp0']*but['platine2']['Lp0']*but['platine2']['tp']/1000.0*7850 for but in data['butons']]),
        poidsPlatines += but.startBout.platine.Hp*but.startBout.platine.Lp*but.startBout.platine.tp/1000.0*7.85;
        poidsPlatines += but.endBout.platine.Hp*but.endBout.platine.Lp*but.endBout.platine.tp/1000.0*7.85;
        nTiges += but.startBout.assemblage.tige.n;
        nTiges += but.endBout.assemblage.tige.n;
        poidsBeches += 7.4*but.startBout.assemblage.beche.n*but.startBout.assemblage.beche.La/1000.0;
        poidsBeches += 7.4*but.endBout.assemblage.beche.n*but.endBout.assemblage.beche.La/1000.0;
		// 'tiges': sum([but['assemblage1']['tige']['nt'] for but in data['butons']]) + sum([but['assemblage2']['tige']['nt'] for but in data['butons']]),
		// 'beches': sum([7.4*but['assemblage1']['beche']['La']*but['assemblage1']['beche']['nbeches'] for but in data['butons']]) + sum([7.4*but['assemblage2']['beche']['La']*but['assemblage2']['beche']['nbeches'] for but in data['butons']])
        paroiJson["butonList"].push_back(but.getJson());
    }

    paroiJson["recap"]["butons"] = json {
        {"nButons", nButons},
        {"poidsButons", ceil(poidsButons)},
        {"poidsPlatines", ceil(poidsPlatines*10.0)/10.0},
        {"nTiges", nTiges},
        {"poidsBeches", ceil(poidsBeches*10.0)/10.0}
    };

    // std::cout << paroiJson["recap"]["butons"].dump(4) << endl;
    std::cout << "ridoPanneautage/getJson" << std::endl;
    std::cout << "Nombre de butons: " << paroiJson["recap"]["butons"]["nButons"] << std::endl;
    std::cout << "Poids des butons: " << paroiJson["recap"]["butons"]["poidsButons"] << "t" << std::endl;
    std::cout << "Poids des platines: " << paroiJson["recap"]["butons"]["poidsPlatines"] << "t" << std::endl;
    std::cout << "Nombre des tiges: " << paroiJson["recap"]["butons"]["nTiges"] << std::endl;
    std::cout << "Poids des beches: " << paroiJson["recap"]["butons"]["poidsBeches"] << "t" << std::endl;

    paroiJson["lierneList"] = json::object();
    for (auto lie: lierneList) {
        if (!paroiJson["lierneList"].contains(lie.first)) {
            paroiJson["lierneList"][lie.first] = json::array();
        }
        for (auto l: lie.second) {
            paroiJson["lierneList"][lie.first].push_back(l.getJson());
        }
    }

    paroiJson["coupes"] = json::object();

    for (auto coupe: coupeList) {
        paroiJson["coupes"][coupe.coupeName] = json::object();
        paroiJson["coupes"][coupe.coupeName]["etats"] = json::object();
        for (auto etat: coupe.etats) {
            paroiJson["coupes"][coupe.coupeName]["etats"][charge::getEtatNameString(etat.first)] = etat.second.getJson();
        }

        paroiJson["coupes"][coupe.coupeName]["panneaux"] = json::array();
        for (auto pan: panneauList) {
            if (pan.coupeId == coupe.id) {
                // std::cout << "1. " << pan.id << " " << pan.coupeId << " ";
                // stringF::printVectorInLine(pan.cageList);
                paroiJson["coupes"][coupe.coupeName]["panneaux"].push_back(pan.getJson(cageList));
            }
        }

        
    }
    
    // paroiJson["materiaux"] = materiaux;
    ofstream o(fileName);
    o << paroiJson.dump(4) << endl;
    std::cout << fileName << " created" << endl;
}

void showEdgeList(vector<rido::edge> edgeList) {
    for (auto edge:edgeList) {
        std::cout << "id: " << edge.id << ", start.x: " << edge.start.x << ", start.y: " << edge.start.y << ", end.x: " << edge.end.x << ", end.y: " << edge.end.y << endl;
        // cout << "Edge: i = " << i << " start.x = " << edgeList[i-1].start.x << ", start.y = " << edgeList[i-1].start.y << ", end.x = " << edgeList[i-1].end.x << ", end.y = " << edgeList[i-1].end.y << endl;
        // cout << "length = " << edgeList[i-1].length << ", vectorX = " << edgeList[i-1].vectorUnite.x << ", vectorY = " << edgeList[i-1].vectorUnite.y << endl;
    }
}

void getPanneauRepartitionLpanFixe(vector<float>& lengthList, float L, float Lpan) {
    int n = floor(L/Lpan);
    for (int i=0; i<n; i++) {
        lengthList.push_back(Lpan);
    }
    
    float Lp = L - n*Lpan;
    if (Lp > 0.01) {
        lengthList.push_back(Lp);
    }
}

void getPanneauRepartition(vector<float>& lengthList, float L, float Lpan, float espacement) {
    int n = floor(L/Lpan);
    if (n > 0) {
        float Lchild = (L - (n-1)*espacement)/n;
        for (int i=0; i<n; i++) {
            lengthList.push_back(Lchild);
        }
    }
}

void rido::box::getPolyline() {
    coor coor1, coor2, coor3, coor4;
    coor1.x = start.x + perpUnite.x*epaisseur*0.5;
    coor1.y = start.y + perpUnite.y*epaisseur*0.5;
    coor2.x = start.x - perpUnite.x*epaisseur*0.5;
    coor2.y = start.y - perpUnite.y*epaisseur*0.5;
    coor3.x = end.x - perpUnite.x*epaisseur*0.5;
    coor3.y = end.y - perpUnite.y*epaisseur*0.5;
    coor4.x = end.x + perpUnite.x*epaisseur*0.5;
    coor4.y = end.y + perpUnite.y*epaisseur*0.5;
    polyline.push_back(coor1);
    polyline.push_back(coor2);
    polyline.push_back(coor3);
    polyline.push_back(coor4);
}

vector<float> getPanneauLengthListOnEdge(float length, json line, json pelle) {

    vector<float> lengthList;
    // cout << "currentEdge.getLength()" << currentEdge.getLength() << endl;
    map<string, string> termes = {{(string) "L", to_string(length)}};

    float startOffset = jsonF::eval(line["startChild"]["offsetValue"], termes);
    float startLength = jsonF::eval(line["startChild"]["length"], termes);
    float endOffset = jsonF::eval(line["endChild"]["offsetValue"], termes);
    float endLength = jsonF::eval(line["endChild"]["length"], termes);


    float L = length - startOffset - startLength + endOffset - endLength;
    float Lpanlim = 0.1;

    if (startLength > 0.0) {lengthList.push_back(startLength);}
    if (jsonF::checkKeyInJsonKeys("repartition", line)) {
        // L0 = ((box.startChild.x-box.endChild.x)**2+(box.startChild.y-box.endChild.y)**2)**(0.5) - box.startChild.length/2 - box.endChild.length/2
        
        getPanneauRepartitionLpanFixe(lengthList, L, line["repartition"].get<float>());

    } else {
        float LpanMax = 2*pelle["LP"].get<float>() + pelle["LMmax"].get<float>();
	    float LpanMin = 2*pelle["LP"].get<float>() + pelle["LMmin"].get<float>();

        // cout << "LpanMax = " << LpanMax << ", LpanMin = " << LpanMin << endl;
        if (L > LpanMax) {
            /*
            # Benne hydraulique :	LP = 2,5m
            # Benne cable : 		LP = 2,8m
            # Benne KS : 			LP = 3,15m
            # Merlon:				LM = 0,5m -> 2m
            # Longueur d'un panneau courant : 
            # LMmin->LMmax ou LP ou 2*LP + LMmin->2*LP + LMmax
            # Benne 2.5 : 0,5->2 ou 2,5 ou 5,5 -> 7
            # Benne 2.8 : 0,5->2 ou 2,8 ou 6,1 -> 7.6
            # Exemple: 
            # Problem 1:
            # 16.0 / 5.5 = 2.9 => 3 => L = 5.3 < LpanMin: trop court, 2 => L= 8.0 > LpanMax: trop longue
            # 16.0 / 7.0 = 2.28 => 2 => L= 8.0 > LpanMax: trop longue
            # floor(16.0 / 5.5) = 2.0 < ceil(16.0 / 7.0)
            # Solution: 
            # floor((16.0-2.5) / 5.5) = 2	(2.45)
            # ceil((16.0-2.5) / 7.0) = 2	(1.928)
            # Problem 2: 
            # floor((16.7-2.5) / 5.5) = 2	(2.58)
            # ceil((16.7-2.5) / 7.0) = 3	(2.03)
            # Solution:
            # floor((16.7-2.5*2) / 5.5) = 2	(2.13)
            # ceil((16.7-2.5*2) / 7.0) = 2	(1.67)
            # Problem 3: 
            # floor((7.58-2.5) / 5.5) = 0	(0...)
            # ceil((7.58-2.5) / 7.3) = 1	(0...)
            # Solution:
            */
            if (L < max(3*LpanMin, 2*LpanMax)) {
                int nLp = 0;
                while ((ceil((L - nLp*pelle["LP"].get<float>())/LpanMax) != floor((L - nLp*pelle["LP"].get<float>())/LpanMin)) & ((L - nLp*pelle["LP"].get<float>()) > 0)) {
                    nLp += 1;
                }   

                if (nLp == 0) {
                    getPanneauRepartition(lengthList, L, LpanMin, 0.0);
                } else if (nLp == 1) {
                    getPanneauRepartition(lengthList, L - pelle["LP"].get<float>(), LpanMin, 0.0);
                    lengthList.push_back(pelle["LP"].get<float>());
                } else if (nLp == 2) {
                    lengthList.push_back(pelle["LP"].get<float>());
                    getPanneauRepartition(lengthList, L - 2.0*pelle["LP"].get<float>(), LpanMin, 0.0);
                    lengthList.push_back(pelle["LP"].get<float>());
                } else {
                    lengthList.push_back(L);
                }
            } else {
                getPanneauRepartition(lengthList, L, LpanMin, 0.0);
            }
        } else if (L > 0.1) {
            lengthList.push_back(L);
        }
    }
    if (endLength > 0.0) {lengthList.push_back(endLength);}
    return lengthList;
}

void rido::paroi::getPanneauxInEdge(edge currentEdge, rido coupe, json line, vector<json>& panDep) {
    // rido coupe = coupeList[coupeId-1];
    // json Offset = coupe.paramsPanneautage["panDep"];
    // edge currentEdge = edgesList[edgeId-1];
    vector<float> lengthList = getPanneauLengthListOnEdge(currentEdge.length, line, params["pelleList"][line["pelle"].get<int>()-1]);
    
    // cout << "currentEdge->length " << currentEdge.length << endl;
    #ifdef CHECKPanneautage
    stringF::printVectorInLine(lengthList);
    #endif

    map<string, string> termes = {{(string) "L", to_string(currentEdge.length)}};
    float Lstart = jsonF::eval(line["startChild"]["offsetValue"], termes);
    
    // std::cout << "Lstart: " << Lstart << endl;

    for (float length: lengthList) {

        int id = panneauList.size() + 1;
        panneau newPanneau(
            id,
            currentEdge.start.x + Lstart*currentEdge.vectorUnite.x,
            currentEdge.start.y + Lstart*currentEdge.vectorUnite.y,
            currentEdge.start.x + (Lstart + length)*currentEdge.vectorUnite.x,
            currentEdge.start.y + (Lstart + length)*currentEdge.vectorUnite.y,
            coupe.id,
            coupe.epaisseur
        );
        
        // for (json offsetLine: Offset) {
        //     if (vectorF::isInVector(stringF::str2IdList(offsetLine["idList"], ","), id))
        //         newPanneau.offset(jsonF::eval(offsetLine["start"], termes), jsonF::eval(offsetLine["end"], termes));
        // }

        // std::cout << "getPanneauxInEdge " << id << std::endl;

        for (auto p: panDep) {
            // std::cout << "getPanneauxInEdge " << p["id"] << std::endl;
            // stringF::printVectorInLine(stringF::str2IdList(p["id"], ","));
            if (vectorF::isInVector(stringF::str2IdList(p["id"], ","), id)) {
                newPanneau.offset(jsonF::eval(p["start"], termes), jsonF::eval(p["end"], termes));
                // std::cout << "getPanneauxInEdge " << id << " " << jsonF::eval(p["start"], termes) << " " << jsonF::eval(p["end"], termes) << std::endl;
            }
        }

        
        // newPanneau.id0 = panneauList.size() + 1;
        newPanneau.getPolyline();
        
        
        panneauList.push_back(newPanneau);
        Lstart += length;
    }
}

std::string int2letter(int n) {
    // A => Z : 65 => 90
    // char W = static_cast<char>(a);
    // n = 1 => A
	if (n < 27) {
        // return chr(n+64);
        std::string s1{static_cast<char>(n+64)};
        return s1;
    }
	int m = round((n-1)/26);
	n -= m*26;
    std::string s1{static_cast<char>(m+64)};
    std::string s2{static_cast<char>(n+64)};
	// return (chr(m+64)+chr(n+64));
    return s1 + s2;
};

int checkCageExist(rido::cage c, vector<rido::cage> cageList0) {
    for (auto c0: cageList0)
        if ((c0.arma[0].n == c.arma[0].n) & (c0.coupeId == c.coupeId)) {
            return c0.id;
        }
    return -1;
}

struct tirantPosition {
    vector<int> pan, lit;
    int pos;
    float dis, angle;
};

void rido::paroi::getCages(float enrobage) {

    vector<int> tirantOff;
    if (params.contains("tirantOff")) {
        // std::cout << params["tirantOff"].dump(4) << std::endl;
        tirantOff = stringF::str2IdList(params["tirantOff"].get<string>(), ",");
    }

    vector<int> prim = stringF::str2IdList(params["prim"].get<string>(), ",");
    vector<int> ferm = stringF::str2IdList(params["ferm"].get<string>(), ",");
    vector<int> left = stringF::str2IdList(params["left"].get<string>(), ",");
    vector<int> rightEnrobage = stringF::str2IdList(params["rightEnrobage"].get<string>(), ",");
    vector<int> leftEnrobage = stringF::str2IdList(params["leftEnrobage"].get<string>(), ",");
    vector<int> rightEnrobage1 = stringF::str2IdList(params["rightEnrobage1"].get<string>(), ",");
    vector<int> leftEnrobage1 = stringF::str2IdList(params["leftEnrobage1"].get<string>(), ",");
    vector<int> labelCageOff = stringF::str2IdList(params["labelCageOff"].get<string>(), ",");
    // vector<int> lCageMax = stringF::str2IdList(params["labelCageOff"].get<string>(), ",");

    map<int, float> lCageMax;
    
    if (params.contains("lCageMax")) {
        // std::cout << "lCageMax" << std::endl;
        vector<json> lCageMaxJsonList = csv::csv2JsonList(params["lCageMax"].get<string>(), 1, 1);
        // std::cout << params["lCageMax"].get<string>() << std::endl;
        for (json lCageMaxJson: lCageMaxJsonList) {
            for (int i: stringF::str2IdList(lCageMaxJson["pan"].get<string>(), ",")) {
                lCageMax.insert(pair<int, float>(i, lCageMaxJson["LcageMax"].get<float>()));
            }
        }
    }

    
    float distanceBetweenCage = 0.2;
    float startGap, endGap;
    int nCages;
    float Lcage, distanceBetweenCageReal;
    float enrobage1 = enrobage;

    if (params.contains("ecartementCage")) {
        distanceBetweenCage = params["ecartementCage"].get<float>();
    }

    if (params.contains("enrobage1")) {
        enrobage1 = params["enrobage1"].get<float>();
    }

    // std::cout << "enrobage1 = " << enrobage1 << std::endl;

    int cageLabel = 0;
    vector<cage> cageList0;
    for (int i: stringF::str2IdList(params["idPanneaux"], ",")) {
        if (i > panneauList.size()) {
            std::cout << "Error: Panneau " << i << " n'existe pas" << std::endl;
            // exit(0);
        } else {
            // for (auto& pan: panneauList) {
            panneau& pan = panneauList[i-1];

            float cageLengthMax = 3.0;
            if (mapF::checkKeyInMap(pan.id, lCageMax)) {
                cageLengthMax = lCageMax[pan.id];
                // std::cout << pan.id << ", LcageMax = " << cageLengthMax << std::endl;
            }

            startGap = 0.15;
            endGap = 0.4;
            if (vectorF::isInVector(prim, pan.id)) {
                startGap = 0.4;
                endGap = 0.4;
            } else if (vectorF::isInVector(ferm, pan.id)) {
                endGap = 0.15;
            } else if (vectorF::isInVector(left, pan.id)) {
                startGap = 0.4;
                endGap = 0.15;
            } 
            if (vectorF::isInVector(rightEnrobage, pan.id)) {
                endGap = enrobage/100.0f;
            } 
            if (vectorF::isInVector(leftEnrobage, pan.id)) {
                startGap = enrobage/100.0f;
            }
            if (vectorF::isInVector(rightEnrobage1, pan.id)) {
                endGap = enrobage1/100.0f;

                // std::cout << "endGap = " << endGap << std::endl;
            } 
            if (vectorF::isInVector(leftEnrobage1, pan.id)) {
                startGap = enrobage1/100.0f;

                // std::cout << "startGap = " << startGap << std::endl;
            }
            
            nCages = ceil((pan.length - startGap - endGap)/cageLengthMax);
            nCages = ceil((pan.length - startGap - endGap - (nCages - 1)*distanceBetweenCage)/cageLengthMax);
            
            pan.nCages = nCages;
            Lcage = pan.length - startGap - endGap;
            distanceBetweenCageReal = 0.0;
            if (nCages > 1) {
                Lcage = round((pan.length - startGap - endGap - (nCages - 1)*distanceBetweenCage)/nCages * 100.0)/100.0;
                distanceBetweenCageReal = (pan.length - startGap - endGap - nCages*Lcage)/(nCages - 1);
            }

            if (Lcage > cageLengthMax) {
                nCages++;

                pan.nCages = nCages;
                Lcage = pan.length - startGap - endGap;
                distanceBetweenCageReal = 0.0;
                if (nCages > 1) {
                    Lcage = round((pan.length - startGap - endGap - (nCages - 1)*distanceBetweenCage)/nCages * 100.0)/100.0;
                    distanceBetweenCageReal = (pan.length - startGap - endGap - nCages*Lcage)/(nCages - 1);
                }
            }

            // std::cout << pan.id << ", pan.length = " << pan.length  << ", nCages = " << nCages << ", Lcage = " << Lcage << std::endl;
            rido& coupePan = coupeList[pan.coupeId];

            // vector<int> nTirants;
            bool tirantImpair = false;
            for (auto appui: coupeList[pan.coupeId].appuiList) {

                if ((appui.type == appuiType::tirant) & (!vectorF::isInVector(tirantOff, pan.id))) {
                // if (appui.type == appuiType::tirant) {
                    // nTirants.push_back(ceil((pan.length-0.01f)/appui.espacement));
                    if (((int) ceil((pan.length-0.01f)/appui.espacement/nCages) % 2) == 1) {
                        tirantImpair = true;
                    }
                }
            }

            for (int i=0; i < nCages; i++) {
                cage newCage(
                    cageList.size() + 1,
                    pan.start.x + (startGap + i*(Lcage + distanceBetweenCageReal))*pan.vectorUnite.x,
                    pan.start.y + (startGap + i*(Lcage + distanceBetweenCageReal))*pan.vectorUnite.y,
                    pan.start.x + (startGap + i*(Lcage + distanceBetweenCageReal) + Lcage)*pan.vectorUnite.x,
                    pan.start.y + (startGap + i*(Lcage + distanceBetweenCageReal) + Lcage)*pan.vectorUnite.y,
                    pan.coupeId,
                    pan.epaisseur - enrobage/100.0f * 2.0f
                );
                newCage.panId = pan.id;
                
                // std::cout << "newCage.start.x = " << newCage.start.x << ", newCage.start.y = " << newCage.start.y << "newCage.end.x = " << newCage.end.x << ", newCage.end.y = " << newCage.end.y << ", newCage.length = " << newCage.length  << ", newCage.epaisseur = " << newCage.epaisseur << std::endl;
                newCage.getPolyline();
                
                for (auto arma: coupePan.armaDispo.armaDispo) {
                    float nBars = ceil(arma.n*pan.length/nCages);
                    if ((((int) nBars % 2) == 1) & tirantImpair) {
                        nBars += 1;
                    }

                    armaParoi newArma = {
                        coupePan.coupeName,
                        arma.cote,
                        nBars,
                        arma.HA,
                        arma.sup,
                        arma.inf,
                        nBars*nCages/pan.length
                    };
                        
                    newCage.arma.push_back(newArma);
                }

                int cageId = checkCageExist(newCage, cageList0);
                if (cageId == -1) {
                    cageLabel++;
                    // newCage.label = to_string(cageLabel);
                    newCage.label0 = int2letter(cageLabel);
                    newCage.label = int2letter(cageLabel) + pan.label;
                    cageList0.push_back(newCage);
                } else {
                    newCage.label0 = cageList[cageId-1].label0;
                    newCage.label = cageList[cageId-1].label0 + pan.label;
                }

                if (vectorF::isInVector(labelCageOff, pan.id)) {
                    newCage.label = "";
                }
                
                cageList.push_back(newCage);
                pan.cageList.push_back(newCage.id);
            }
        }
    }

}

// vector<int> get

void rido::paroi::panneautage() {

    vector<int> edgesIdList, setEdgesIdList0;
    // edge* currentEdge;

    
    // for (auto& coupe: coupeList) {
        
    //     set<int> setEdgesIdList;
    //     // edgesIdList = stringF::str2IdList(coupe.paramsPanneautage["edges"], ",");

    //     for (auto line: coupe.paramsPanneautage["edgeList"]) {
    //         edgesIdList = stringF::str2IdList(line["idList"], ",");
    //         #ifdef CHECKPanneautage
    //         std::cout << coupe.coupeName << " " << line["idList"] << " " << line["startChild"]["length"] << " " << line["startChild"]["offsetValue"] << " " << line["endChild"]["length"] << " " << line["endChild"]["offsetValue"] << endl;
    //         #endif
    //         // stringF::printVectorInLine(edgesIdList);
    //         for (int i: edgesIdList) {
    //             // currentEdge = &edgeList[i-1];
    //             getPanneauxInEdge(edgesList[i-1], coupe, line);
    //         }
    //     }
    // }

    vector<json> panDep = csv::csv2JsonList(params["panDep"], 1, 1);
    json coupesCsv = csv::csv2MapJsonList(params["coupesCsv"], 1, 1);

    for (auto line: params["edgeList"]) {
        if (coupesCsv[line["coupe"].get<std::string>()]["calcul"] == 1) {
            edgesIdList = stringF::str2IdList(line["idList"], ",");
            
            // stringF::printVectorInLine(edgesIdList);

            rido& coupe = coupeList[getCoupeByName(line["coupe"])];

            #ifdef CHECKPanneautage
            std::cout << coupe.coupeName << " " << line["idList"] << " " << line["startChild"]["length"] << " " << line["startChild"]["offsetValue"] << " " << line["endChild"]["length"] << " " << line["endChild"]["offsetValue"] << endl;
            #endif

            for (int i: edgesIdList) {
                // currentEdge = &edgeList[i-1];
                getPanneauxInEdge(edgesList[i-1], coupe, line, panDep);
            }
        }
    }
    
    for (auto& pan: panneauList) {
        pan.label = to_string(pan.id);
    }
    
    int label = 1;
    if (params.contains("label")) {
        for (string group: stringF::splitStrSpace(params["label"], ";")) {
            
            if (group[0] == '(') {
                group = group.substr(1, (int(group.size())-2));

                // std::cout << group << std::endl;
                // stringF::printVectorInLine(stringF::str2IdList(group, ","));
                for (int i : stringF::str2IdList(group, ",")) {
                    if (i > panneauList.size()) {
                        std::cout << "Error: panneau " << i << " n'existe pas (label)" << std::endl;
                    } else {
                        panneauList[i-1].label = to_string(label);
                    }
                }
                label++;
            } else {
                for (int i : stringF::str2IdList(group, ",")) {
                    if (i > panneauList.size()) {
                        std::cout << "Error: panneau " << i << " n'existe pas (label)" << std::endl;
                    } else {
                        panneauList[i-1].label = to_string(label);
                    }
                    label++;
                }
            }
        }
    }
    
    vector<int> labelPanOff = stringF::str2IdList(params["labelPanOff"].get<string>(), ",");

    for (auto& pan: panneauList) {
        if (vectorF::isInVector(labelPanOff, pan.id)) {
            pan.label = "";
        }
    }
}

void rido::paroi::checkPanneauList() {
    // cout << "Panneau: id = " << id << ", coupe.coupeName" << newPanneau.coupe->coupeName << ", Lstart = " << Lstart << ", length = " << length << ", currentEdge->start.x = " << currentEdge->start.x << ",, currentEdge->start.y = " << currentEdge->start.y << ", currentEdge->vectorUnite.x = " << currentEdge->vectorUnite.x << ", currentEdge->vectorUnite.y = " << currentEdge->vectorUnite.y << endl;
    // cout << "Panneau: id = " << id << ", coupe->coupeName = " << coupe->coupeName << ", coupe->epaisseur = " << coupe->epaisseur << ", perpUnite.x = " << perpUnite.x << ", perpUnite.y = " << perpUnite.y << ", start.x = " << start.x << ", start.y = " << start.y << endl;

    cout << "Panneaux list: " << endl;
    for (auto pan: panneauList) {
        cout << "id = " << pan.id << ", coupeName: " << coupeList[pan.coupeId].coupeName << ", length = " << pan.length << endl;
    }
}

void rido::paroi::recapTirant(string fileName) {
    // std::cout << fileName << endl;

    vector<int> tirantOff;
    if (params.contains("tirantOff")) {
        // std::cout << params["tirantOff"].dump(4) << std::endl;
        tirantOff = stringF::str2IdList(params["tirantOff"].get<string>(), ",");
    }

    float LtirantTotal = 0.0;
    int nTirantTotal = 0;

    vector<tirantPosition> tirantList;
    if (params.contains("tirant")) {
        vector<json> tirantJsonList = csv::csv2JsonList(params["tirant"].get<string>(), 1, 1);
        for (json tirantJson: tirantJsonList) {
            tirantList.push_back(tirantPosition {
                stringF::str2IdList(tirantJson["pan"].get<string>(), ","),
                stringF::str2IdList(tirantJson["lit"].get<string>(), ","),
                tirantJson["pos"].get<int>(),
                tirantJson["dis"].get<float>(),
                tirantJson["angle"].get<float>()
            });

            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["pan"].get<string>(), ","));
            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["lit"].get<string>(), ","));
        }
    }

    json coupesCsv = csv::csv2MapJsonList(params["coupesCsv"], 1, 1);

    string txt = "N??,Tirant,Panneau,PanLen,Coupe,Type,Lit,Niveau,Espacement,Inclinaison,D??viation,Pr??contrainte,Tension de blocage,Tension de service,Tension d'??preuve,Longueur libre,Longueur scell??e,Longueur totale\n";
    int tirantId = 0;
    
    std::cout << "Ce tableau n'a pas pris en compte des tirants d??finitifs" << std::endl;
    for (auto pan: panneauList) {
        // if (coupesCsv[coupeList[pan.coupeId].coupeName]["metre"].get<int>() == 1) {
        float lPanTotal = pan.length;
        for (auto pan0: panneauList) {
            if ((pan0.label == pan.label) & (pan0.id != pan.id)) {
                lPanTotal += pan0.length;
            }
        }
        if (pan.length != lPanTotal) {
            std::cout << "ridoPanneautage >> recapTirant >> " << pan.label << " " << pan.length << " " << lPanTotal << std::endl;
        }
        for (auto appui: coupeList[pan.coupeId].appuiList) {
            if ((appui.type == appuiType::tirant) & (!vectorF::isInVector(tirantOff, pan.id))) {


                //int nTirants = ceil((pan.length-0.01f)/appui.espacement);
                int nTirantsPan = ceil((lPanTotal - 0.01f)/appui.espacement);
                int nTirants = ceil((lPanTotal - 0.01f)/appui.espacement);
                for (auto pan0: panneauList) {
                    if ((pan0.label == pan.label) & (pan0.id != pan.id)) {
                        nTirants -= ceil((pan0.length-0.01f)/appui.espacement);
                    }
                }
                nTirants = max(nTirants, 1);

                LtirantTotal += nTirants*(appui.Ll + appui.Ls);
                nTirantTotal += nTirants;

                // Effort maximal
                float p = 0.0;
                // for (auto etat: coupeList[pan.coupeId].etats) {
                // cout << "etat.first " << etat.first << endl;
                for (auto F: coupeList[pan.coupeId].appuiList[appui.lit-1].Flist) {
                    //     cout << F.first << endl;
                    //     stringF::printVectorInLine(etat.second.phasesList);
                    //     if (vectorF::isInVector(etat.second.phasesList, F.first)) {
                    // isInEtat = true;
                    p = min(p, F.second);
                    //     }
                    // }
            }

            for (int i=0; i<nTirants; i++) {
                tirantId++;
                float angle = 0.0;
                for (auto t: tirantList) {
                    if (vectorF::isInVector(t.pan, pan.id) & vectorF::isInVector(t.lit, appui.lit) & ((i+1) == t.pos)) {
                        angle = t.angle;
                    }
                }

                //float espacementReel = pan.length/nTirants;
                float espacementReel = lPanTotal/nTirantsPan;
                float coef = espacementReel/appui.espacement;
                // stringF::ftos(appui.niveau, 2)
                txt += to_string(tirantId) + "," + pan.label + "-" + to_string(appui.lit) + " " + to_string(i+1) + "," + pan.label + "," + stringF::ftos(pan.length,2) + "," + coupeList[pan.coupeId].coupeName + "," + to_string(appui.nToronts) + "T15," + to_string(appui.lit) + "," + to_string(appui.niveau) + "," + stringF::ftos(espacementReel,2) + "," + to_string(appui.angle) + "," + to_string(angle) + "," + stringF::ftos(-appui.precontrainte*coef,1) + "," + stringF::ftos(-appui.precontrainte*1.06*coef,1) + "," + stringF::ftos(-p*coef,1) + "," + stringF::ftos(-p*1.15*coef,1) + "," + to_string(appui.Ll) + "," + to_string(appui.Ls) + "," + to_string(appui.Ls + appui.Ll) + "\n";

                // std::cout << p << std::endl;
                // std::cout << to_string(tirantId) + " ," + stringF::ftos(p,2) + " ," + stringF::ftos(coef,2) + " # " + stringF::ftos(appui.espacement,2) + " # " + pan.label + "-" + to_string(appui.lit) + " " + to_string(i+1) + "," + pan.label + "," + stringF::ftos(pan.length,2) + "," + coupeList[pan.coupeId].coupeName + "," + to_string(appui.nToronts) + "T15," + to_string(appui.lit) + "," + to_string(appui.niveau) + "," + stringF::ftos(espacementReel,2) + "," + to_string(appui.angle) + "," + to_string(angle) + "," + stringF::ftos(-appui.precontrainte*coef,1) + "," + stringF::ftos(-appui.precontrainte*1.06*coef,1) + "," + stringF::ftos(-p*coef,1) + "," + stringF::ftos(-p*1.15*coef,1) + "," + to_string(appui.Ll) + "," + to_string(appui.Ls) + "," + to_string(appui.Ls + appui.Ll) + "\n" << std::endl;

            }

                }
            }
        // }
    }

    std::ofstream myfile;
    myfile.open(fileName);
    if (myfile.is_open()) {
        myfile << txt;
        myfile.close();
        std::cout << fileName << " created." << endl;
    } else {
        std::cout << fileName << " didn't write" << std::endl;
    }
}

void rido::paroi::panneautageMetre(string fileName) {
    // std::cout << fileName << endl;

    vector<int> tirantOff;
    if (params.contains("tirantOff")) {
        // std::cout << params["tirantOff"].dump(4) << std::endl;
        tirantOff = stringF::str2IdList(params["tirantOff"].get<string>(), ",");
    }

    float LtirantTotal = 0.0;
    int nTirantTotal = 0;
    map<int, float> tirantMetreL;
    map<int, int> tirantMetreN;
    for (int i=3;i<10;i++) {
        tirantMetreL.insert(pair<int, float>(i,0.0));
        tirantMetreN.insert(pair<int, float>(i,0));
    }

    vector<tirantPosition> tirantList;
    if (params.contains("tirant")) {
        vector<json> tirantJsonList = csv::csv2JsonList(params["tirant"].get<string>(), 1, 1);
        for (json tirantJson: tirantJsonList) {
            tirantList.push_back(tirantPosition {
                stringF::str2IdList(tirantJson["pan"].get<string>(), ","),
                stringF::str2IdList(tirantJson["lit"].get<string>(), ","),
                tirantJson["pos"].get<int>(),
                tirantJson["dis"].get<float>(),
                tirantJson["angle"].get<float>()
            });

            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["pan"].get<string>(), ","));
            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["lit"].get<string>(), ","));
        }
    }

    json coupesCsv = csv::csv2MapJsonList(params["coupesCsv"], 1, 1);


    for (auto pan: panneauList) {

        // std::cout << coupesCsv[coupeList[pan.coupeId].coupeName]["metre"] << std::endl;

        if (coupesCsv[coupeList[pan.coupeId].coupeName]["metre"].get<int>() == 1) {
            for (auto appui: coupeList[pan.coupeId].appuiList) {
                if ((appui.type == appuiType::tirant) & (!vectorF::isInVector(tirantOff, pan.id))) {
                // if (appui.type == appuiType::tirant) {
                    int nTirants = ceil((pan.length-0.01f)/appui.espacement);
                    tirantMetreL[appui.nToronts] += nTirants*(appui.Ll + appui.Ls);
                    tirantMetreN[appui.nToronts] += nTirants;
                    LtirantTotal += nTirants*(appui.Ll + appui.Ls);
                    nTirantTotal += nTirants;
                }
            }
        }
        
    }

    std::cout << "M??tr?? des tirants :" << std::endl;
    for (auto t: tirantMetreL) {
        if (t.second > 0.0) {
            std::cout << "Tirants " << t.first << " toronts: Ltotal = " << t.second << "m" << std::endl;
        }
    }
    for (auto t: tirantMetreN) {
        if (t.second > 0.0) {
            std::cout << "Tirants " << t.first << " toronts: ntotal = " << t.second << " tirants" << std::endl;
        }
    }

    std::cout << "L tirants total: " << LtirantTotal << "m" << std::endl;
    std::cout << "n tirants total: " << nTirantTotal << std::endl;

    vector<string> txtList = {
        ",", 
        "Epaisseur,",
        "D??placement maximal,",
        "D??placement en t??te,",
        "Lin??aire Murette Guide,",
        "Nombre de Panneaux,",
        "Arase Sup??rieure Plateforme de Travail,",
        "Arase Sup??rieure Paroi Moul??e,",
        "Arase inf??rieure Paroi Moul??e (m??canique),",
        "Arase inf??rieure Paroi Moul??e (hydraulique),",
        "Niveau Fond de Fouille,",
        "Surface de Forage,",
        "Volume de b??ton Th??orique,",
        "Volume de D??blais Th??orique,",
        "Ratio D'acier,",
        "Tonnage d'acier,"
    };

    for (int i=0; i<5; i++){
        txtList.push_back("Nbre de Torons Lit " + to_string(i+1) + ",");
        txtList.push_back("Nombre de Tirants Lit " + to_string(i+1) + ",");
        txtList.push_back("Longueur Libre Lit " + to_string(i+1) + ",");
        txtList.push_back("Longueur Scell??e Lit " + to_string(i+1) + ",");
    }
    
    float VbetonTotal = 0.0;
    float poidsAcierTotal = 0.0;
    float SforageTotal = 0.0;
    map<string, int> tubageTirant;

    for (auto coupe: coupeList) {
        if (coupesCsv[coupe.coupeName]["metre"].get<int>() == 1) {

            int nPanneaux = 0;
            float Lmurette = 0.0;
            float Sforage = 0.0;
            float Vbeton = 0.0;
            float Vdeblais = 0.0;
            for (auto pan: panneauList) {
                if (coupe.id == pan.coupeId) {
                    nPanneaux++;
                    Lmurette += pan.length;
                    Sforage += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"]);
                    Vbeton += pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;
                    Vdeblais += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"])*coupe.epaisseur;

                    SforageTotal += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"]);
                    VbetonTotal += pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;
                    poidsAcierTotal += ceil(coupe.ratio/5.0)*5.0*pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;

                }
                // cout << "id = " << pan.id << ", coupeName: " << coupeList[pan.coupeId].coupeName << ", length = " << pan.length << endl;
            }

            float dMax=0.0, dTeteMax=0.0;
            // string EE = "EE";
            for (auto phase: coupe.phases) {
                if (mapF::checkKeyInMap(charge::etatName::EE, coupe.etats)) {
                    if (!vectorF::isInVector(coupe.etats[charge::etatName::EE].phasesList, phase.id)) {
                        dMax = max(dMax, phase.dMax);
                        dTeteMax = max(dTeteMax, phase.dTeteMax);
                    }
                } else {
                    dMax = max(dMax, phase.dMax);
                    dTeteMax = max(dTeteMax, phase.dTeteMax);
                }
                
            }
            txtList[0] += coupe.coupeName + ",";
            txtList[1] += to_string(coupe.epaisseur) + ",";
            txtList[2] += to_string(round(dMax*100.0)/100.0) + ",";
            txtList[3] += to_string(round(dTeteMax*100.0)/100.0) + ",";
            txtList[4] += to_string(round(Lmurette*100.0)/100.0) + ",";
            txtList[5] += to_string(nPanneaux) + ",";
            txtList[6] += to_string(round(coupe.params["plateforme"]*100.0)/100.0) +  ",";
            txtList[7] += to_string(round(coupe.params["ASpm"]*100.0)/100.0) +  ",";
            txtList[8] += to_string(round(coupe.params["AIpm"]*100.0)/100.0) +  ",";
            txtList[9] += to_string(round(coupe.params["AIpmH"]*100.0)/100.0) +  ",";
            txtList[10] += to_string(round(coupe.params["FF"]*100.0)/100.0) +  ",";
            txtList[11] += to_string(round(Sforage*100.0)/100.0) + ",";
            txtList[12] += to_string(round(Vbeton*100.0)/100.0) + ",";
            txtList[13] += to_string(round(Vdeblais*100.0)/100.0) + ",";
            // txtList[14] += "0,";
            // txtList[15] += "0,";
            txtList[14] += to_string(ceil(coupe.ratio/5.0)*5.0) +  ",";
            txtList[15] += to_string(round(ceil(coupe.ratio/5.0)*5.0*Vbeton)/1000.0) + ",";
            int i = 0;
            for (auto appui: coupe.appuiList) {

                if (appui.type == appuiType::tirant) {
                    int nTirants = 0;
                    for (auto pan: panneauList) {
                        if ((coupe.id == pan.coupeId) & (!vectorF::isInVector(tirantOff, pan.id))) {
                            nTirants += ceil((pan.length-0.01)/appui.espacement);

                            for (int ii=0; ii<ceil((pan.length-0.01)/appui.espacement); ii++) {
                                float angle1 = 0.0;
                                for (auto t: tirantList) {
                                    if (vectorF::isInVector(t.pan, pan.id) & vectorF::isInVector(t.lit, appui.lit) & ((ii+1) == t.pos)) {
                                        angle1 = t.angle;
                                        // dis = t.dis;
                                    }
                                }
                                string tirantType = stringF::ftos(coupe.epaisseur, 2) + " | " + stringF::ftos(appui.angle,0) + " | " + stringF::ftos(angle1,1);
                                if (mapF::checkKeyInMap(tirantType, tubageTirant)) {
                                    tubageTirant[tirantType]++;
                                } else {
                                    tubageTirant[tirantType] = 1;
                                }
                            }
                            
                            // std::cout << pan.id << " " << ceil((pan.length-0.01)/appui.espacement) << std::endl;
                    }}
                    
                    txtList[16+i] += to_string(appui.nToronts) + ",";
                    txtList[17+i] += to_string(nTirants) + ",";
                    txtList[18+i] += to_string(appui.Ll) + ",";
                    txtList[19+i] += to_string(appui.Ls) + ",";
                    // std::cout << appui.Ll << " " << appui.Ls << endl;
                    // std::cout << coupe.coupeName << " " << 
                    i+=4;
                }
            }
            // std::cout << i << endl;
            if (i < 16) {
                for (; i<17; i+=4){
                    // std::cout << i << endl;
                    txtList[16+i] += "0,";
                    txtList[17+i] += "0,";
                    txtList[18+i] += "0,";
                    txtList[19+i] += "0,";
                }
            }
        }
    }

    std::cout << "Volumne beton total: " << VbetonTotal << "m3" << std::endl;
    std::cout << "Surface de forage totale: " << SforageTotal << "m2" << std::endl;
    std::cout << "Poids d'aciers total: " << ceil(poidsAcierTotal/100.0)/10.0 << "t" << std::endl;

    for (auto t: tubageTirant) {
        std::cout << t.second << " tubes, ep | phiV | phiH = " << t.first << std::endl;
    }

    std::ofstream myfile;
    myfile.open(fileName);
    if (myfile.is_open()) {
        for (string t: txtList) {
            myfile << t << "\n";
        }
        myfile.close();
        std::cout << fileName << " created." << endl;
    } else {
        std::cout << fileName << " didn't write" << std::endl;
    }
}

void rido::paroi::panneautageMetrePanneaux(string fileName) {
    // std::cout << fileName << endl;

    vector<int> tirantOff;
    if (params.contains("tirantOff")) {
        // std::cout << params["tirantOff"].dump(4) << std::endl;
        tirantOff = stringF::str2IdList(params["tirantOff"].get<string>(), ",");
    }

    vector<string> txtList = {
        ","
        "Coupe,"
        "Ep(m),"
        "Lpan(m),"// "Lin??aire panneaux Guide,"
        "Murette(m),"// "Lin??aire Murette Guide,"
        "nPan,"// "Nombre de Panneaux,"
        "Plateforme,"// "Arase Sup??rieure Plateforme de Travail,"
        "ASpm,"// "Arase Sup??rieure Paroi Moul??e,"
        "AIpm,"// "Arase inf??rieure Paroi Moul??e (m??canique),"
        "AIpmH,"// "Arase inf??rieure Paroi Moul??e (hydraulique),"
        "FF,"// "Niveau Fond de Fouille,"
        "Sforage(m??),"// "Surface de Forage,"
        "Vb??ton(m3),"// "Volume de b??ton Th??orique,"
        "Vd??blais(m3),"// "Volume de D??blais Th??orique,"
        "Ratio,"// "Ratio D'acier,"
        "Aciers(t),"// "Tonnage d'acier,"
        "Tirants,"
        "Cage"
    };

    vector<tirantPosition> tirantList;
    if (params.contains("tirant")) {
        vector<json> tirantJsonList = csv::csv2JsonList(params["tirant"].get<string>(), 1, 1);
        for (json tirantJson: tirantJsonList) {
            tirantList.push_back(tirantPosition {
                stringF::str2IdList(tirantJson["pan"].get<string>(), ","),
                stringF::str2IdList(tirantJson["lit"].get<string>(), ","),
                tirantJson["pos"].get<int>(),
                tirantJson["dis"].get<float>(),
                tirantJson["angle"].get<float>()
            });

            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["pan"].get<string>(), ","));
            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["lit"].get<string>(), ","));
        }
    }

    
    int nPanneaux = 0;
    float Lmurette = 0.0;
    float Sforage = 0.0;
    float Vbeton = 0.0;
    float Vdeblais = 0.0;
    float Lpan = 0.0;
    
    int i=0;
    for (auto pan: panneauList) {
        // if (i < (panneauList.size()-1)) {
        //     if (panneauList[i+1].label == pan.label) {
        //         Lmurette += pan.length;
        //         Lpan += pan.length;
        //     } else {
                string txt = "";
                rido& coupe = coupeList[pan.coupeId];
                Lmurette += pan.length;
                Lpan += pan.length;
                Sforage = pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"]);
                Vbeton = pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;
                Vdeblais += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"])*coupe.epaisseur;
                
                string cageName = "";
                for (auto c: cageList) {
                    if (c.panId == pan.id) {
                        std::cout << "pan.id" << pan.id << " " << c.label << std::endl;
                        cageName = c.label;
                    }
                }

                // txt +=  to_string(pan.id) + ",";
                txt +=  pan.label + ",";
                txt +=  coupe.coupeName + ",";
                txt +=  to_string(coupe.epaisseur) + ",";
                txt +=  to_string(round(Lpan*100.0)/100.0) + ",";
                txt +=  to_string(round(Lmurette*100.0)/100.0) + ",";
                txt +=  to_string(nPanneaux) + ",";
                txt +=  to_string(round(coupe.params["plateforme"]*100.0)/100.0) +  ",";
                txt +=  to_string(round(coupe.params["ASpm"]*100.0)/100.0) +  ",";
                txt +=  to_string(round(coupe.params["AIpm"]*100.0)/100.0) +  ",";
                txt +=  to_string(round(coupe.params["AIpmH"]*100.0)/100.0) +  ",";
                txt +=  to_string(round(coupe.params["FF"]*100.0)/100.0) +  ",";
                txt +=  to_string(round(Sforage*100.0)/100.0) + ",";
                txt += to_string(round(Vbeton*100.0)/100.0) + ",";
                txt += to_string(round(Vdeblais*100.0)/100.0) + ",";
                // txtList[14] += "0,";
                // txtList[15] += "0,";
                txt += to_string(ceil(coupe.ratio/5.0)*5.0) +  ",";
                txt += to_string(round(ceil(coupe.ratio/5.0)*5.0*Vbeton)/1000.0) + ",";
                
                string txtTirantPhi = "";
                int nTirants = 0;
                int i = 0;
                for (auto appui: coupe.appuiList) {

                    if (appui.type == appuiType::tirant) {
                        // int nTirants = 0;
                        // if ((coupe.id == pan.coupeId) & (!vectorF::isInVector(tirantOff, pan.id))) {
                        //     nTirants += ceil((pan.length-0.01)/appui.espacement);
                        // }

                        // float angle1 = 0.0;
                        // for (auto t: tirantList) {
                        //     if (vectorF::isInVector(t.pan, pan.id) & vectorF::isInVector(t.lit, appui.lit) & ((ii+1) == t.pos)) {
                        //         angle1 = t.angle;
                        //         // dis = t.dis;
                        //     }
                        // }
                        
                        // txtTirantPhi += to_string(nTirants) + "x" + stringF::ftos(pan.epaisseur*1000.0, 0) + "phi" + stringF::ftos(appui.angle, 0) + "-";
                        

                        if ((coupe.id == pan.coupeId) & (!vectorF::isInVector(tirantOff, pan.id))) {
                            nTirants += ceil((pan.length-0.01)/appui.espacement);

                            for (int ii=0; ii<nTirants; ii++) {
                                float angle1 = 0.0;
                                for (auto t: tirantList) {
                                    if (vectorF::isInVector(t.pan, pan.id) & vectorF::isInVector(t.lit, appui.lit) & ((ii+1) == t.pos)) {
                                        angle1 = t.angle;
                                        // dis = t.dis;
                                    }
                                }
                                // string tirantType = stringF::ftos(coupe.epaisseur, 2) + " | " + stringF::ftos(appui.angle,0) + " | " + stringF::ftos(angle1,1);

                                txtTirantPhi += stringF::ftos(pan.epaisseur*1000.0, 0) + "-phiV=" + stringF::ftos(appui.angle, 0) + "-phiH=" + stringF::ftos(angle1, 0) + " / ";
                            }
                            
                            // std::cout << pan.id << " " << ceil((pan.length-0.01)/appui.espacement) << std::endl;
                        }

                        // txtList[16+i] += to_string(appui.nToronts) + ",";
                        // txtList[17+i] += to_string(nTirants) + ",";
                        // txtList[18+i] += to_string(appui.Ll) + ",";
                        // txtList[19+i] += to_string(appui.Ls) + ",";
                        // std::cout << appui.Ll << " " << appui.Ls << endl;
                        // std::cout << coupe.coupeName << " " << 
                        i+=4;
                    }
                }

                txt +=  txtTirantPhi + ",";
                txt += cageName;

                // // std::cout << i << endl;
                // if (i < 16) {
                //     for (; i<17; i+=4){
                //         // std::cout << i << endl;
                //         txtList[16+i] += "0,";
                //         txtList[17+i] += "0,";
                //         txtList[18+i] += "0,";
                //         txtList[19+i] += "0,";
                //     }
                // }

                txtList.push_back(txt);

                Lpan = 0.0;
                Lmurette = 0.0;
            // }
        // } else {
        //     if (panneauList[i-1].label != pan.label) {
        //         string txt = "";
        //         rido& coupe = coupeList[pan.coupeId];
        //         Lmurette += pan.length;
        //         Lpan += pan.length;
        //         Sforage = pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"]);
        //         Vbeton = pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;
        //         Vdeblais += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"])*coupe.epaisseur;
                
        //         // txt +=  to_string(pan.id) + ",";
        //         txt +=  pan.label + ",";
        //         txt +=  coupe.coupeName + ",";
        //         txt +=  to_string(coupe.epaisseur) + ",";
        //         txt +=  to_string(round(Lpan*100.0)/100.0) + ",";
        //         txt +=  to_string(round(Lmurette*100.0)/100.0) + ",";
        //         txt +=  to_string(nPanneaux) + ",";
        //         txt +=  to_string(round(coupe.params["plateforme"]*100.0)/100.0) +  ",";
        //         txt +=  to_string(round(coupe.params["ASpm"]*100.0)/100.0) +  ",";
        //         txt +=  to_string(round(coupe.params["AIpm"]*100.0)/100.0) +  ",";
        //         txt +=  to_string(round(coupe.params["AIpmH"]*100.0)/100.0) +  ",";
        //         txt +=  to_string(round(coupe.params["FF"]*100.0)/100.0) +  ",";
        //         txt +=  to_string(round(Sforage*100.0)/100.0) + ",";
        //         txt += to_string(round(Vbeton*100.0)/100.0) + ",";
        //         txt += to_string(round(Vdeblais*100.0)/100.0) + ",";
        //         // txtList[14] += "0,";
        //         // txtList[15] += "0,";
        //         txt += to_string(ceil(coupe.ratio/5.0)*5.0) +  ",";
        //         txt += to_string(round(ceil(coupe.ratio/5.0)*5.0*Vbeton)/1000.0) + ",";
                
        //         // int i = 0;
        //         // for (auto appui: coupe.appuiList) {

        //         //     if (appui.type == appuiType::tirant) {
        //         //         int nTirants = 0;
        //         //         for (auto pan: panneauList) {
        //         //             if ((coupe.id == pan.coupeId) & (!vectorF::isInVector(tirantOff, pan.id))) {
        //         //                 // nTirants += ceil((pan.length-0.01)/appui.espacement);
                                
        //         //                 // std::cout << pan.id << " " << ceil((pan.length-0.01)/appui.espacement) << std::endl;
        //         //         }}
                        
        //         //         // txtList[16+i] += to_string(appui.nToronts) + ",";
        //         //         // txtList[17+i] += to_string(nTirants) + ",";
        //         //         // txtList[18+i] += to_string(appui.Ll) + ",";
        //         //         // txtList[19+i] += to_string(appui.Ls) + ",";
        //         //         // std::cout << appui.Ll << " " << appui.Ls << endl;
        //         //         // std::cout << coupe.coupeName << " " << 
        //         //         i+=4;
        //         //     }
        //         // }

        //         // // std::cout << i << endl;
        //         // if (i < 16) {
        //         //     for (; i<17; i+=4){
        //         //         // std::cout << i << endl;
        //         //         txtList[16+i] += "0,";
        //         //         txtList[17+i] += "0,";
        //         //         txtList[18+i] += "0,";
        //         //         txtList[19+i] += "0,";
        //         //     }
        //         // }

        //         txtList.push_back(txt);

        //         Lpan = 0.0;
        //         Lmurette = 0.0;
        //     }
        // }
        i++;
    }

    std::ofstream myfile;
    myfile.open(fileName);
    if (myfile.is_open()) {
        for (string t: txtList) {
            myfile << t << "\n";
        }
        myfile.close();
        std::cout << fileName << " created." << endl;
    } else {
        std::cout << fileName << " didn't write" << std::endl;
    }
}

void rido::paroi::panneautagePrix(string fileName) {

    if (!params.contains("prix")) {
        std:cout << "Pas de prix param??tre" << std::endl;
        return;
    }
    
    std::cout << "panneautagePrix" << std::endl;

    vector<int> tirantOff;
    if (params.contains("tirantOff")) {
        // std::cout << params["tirantOff"].dump(4) << std::endl;
        tirantOff = stringF::str2IdList(params["tirantOff"].get<string>(), ",");
    }

    float LtirantTotal = 0.0;
    int nTirantTotal = 0;
    map<int, float> tirantMetreL;
    map<int, int> tirantMetreN;
    for (int i=3;i<10;i++) {
        tirantMetreL.insert(pair<int, float>(i,0.0));
        tirantMetreN.insert(pair<int, float>(i,0));
    }

    for (auto pan: panneauList) {
        for (auto appui: coupeList[pan.coupeId].appuiList) {
            if ((appui.type == appuiType::tirant) & (!vectorF::isInVector(tirantOff, pan.id))) {
            // if (appui.type == appuiType::tirant) {
                int nTirants = ceil((pan.length-0.01f)/appui.espacement);
                tirantMetreL[appui.nToronts] += nTirants*(appui.Ll + appui.Ls);
                tirantMetreN[appui.nToronts] += nTirants;
                LtirantTotal += nTirants*(appui.Ll + appui.Ls);
                nTirantTotal += nTirants;
            }
        }
    }

    vector<json> prixParam = csv::csv2JsonList(params["prix"], 1, 2);

    string txt = "No,D??signation,Unite,Quantite,Prix Unitaire,Prix Total HT,Coefficient\n";

    float subTotal = 0.0;
    float total = 0.0;
    int section = -1;
    for (auto p: prixParam) {
        // std::cout << p["Designation"].get<string>() << std::endl;
        if (p["Partie"].get<string>() == "Int") {
            if ((p["Prix"].get<int>() % 100) == 0) {
                if (section != -1) {
                    txt += ",Sub total (" + stringF::ftos(section, 0) + "),,,," + to_string(subTotal) + "\n\n";
                }

                subTotal = 0.0;

                txt += stringF::ftos(ceil(p["Prix"].get<int>() / 100), 0) + "," + p["Designation"].get<string>() + ",,,,\n";
                section = ceil(p["Prix"].get<int>() / 100);
            } else {

                if ((p["Prix"].get<int>() > 2001 ) & (p["Prix"].get<int>() < 2011)) {p["Quantite"] = tirantMetreL[p["Prix"].get<int>() - 2001];}
                if ((p["Prix"].get<int>() == 2011 ) | (p["Prix"].get<int>() == 2014 ) | (p["Prix"].get<int>() == 2021 )) {p["Quantite"] = nTirantTotal;}

                float p0 = p["Quantite"].get<float>()*p["Prix Unitaire"].get<float>()*p["Coefficient"].get<float>();

                txt += stringF::ftos(ceil(p["Prix"].get<int>() / 100), 0) + "." + stringF::ftos(p["Prix"].get<int>() % 100, 0) + "," + p["Designation"].get<string>() + "," + p["Unite"].get<string>() + "," + to_string(p["Quantite"].get<float>()) + "," + to_string(p["Prix Unitaire"].get<float>()) + "," + to_string(p0) + "," + to_string(p["Coefficient"].get<float>()) + "\n";

                subTotal += p0;
                total += p0;
            }
            
        }
        
    }
    txt += ",Sub total (" + stringF::ftos(section, 0) + "),,,," + to_string(subTotal) + "\n";
    txt += ",Total,,,," + to_string(total) + "\n";

    // // std::cout << fileName << endl;
    // float p11Etudes = 10000.0;
    // float p15 = 8000.0;

    // float p21 = 89000.0;
    
    // string txt = "No,D??signation,Unite,Quantite,Prix Unitaire,Prix Total HT\n"
    // "1,ETUDES PREPARATION,,,,\n"
    // "1.1,Etudes d'ex??cution,FT,1," + to_string(p11Etudes) + "," + to_string(p11Etudes) + "\n"
    // "1.2,Installation g??n??rale + cantonnement pour notre personnel - balayage en cas de n??cessit??,FT,Non Compris,Non Compris\n"
    // "1.3,Branchement eau diam. 40mm 30m3/h et consommations,FT,Non Compris,Non Compris\n"
    // "1.4,Branchement ??lectrique 140 KVA et consommation,FT,Non Compris,Non Compris\n"
    // "1.5,Nettoyage Hebdomadaire par Balayeuse,FT," + to_string(p15) + "," + to_string(p15) + "\n"
    // ",Sous total 1,,," + to_string(p11Etudes + p15) + "\n"
    // "\n"
    // "2,PAROIS MOULEES,,,,\n"
    // "2.1,Amen??e et repli 1 ateliers de parois moul??e,FT,1," + to_string(p11Etudes) + "," + to_string(p11Etudes) + "\n"
    // "2.2,Amen??e et repli 1 ateliers de parois moul??e,FT,Non Compris,Non Compris\n"
    // "2.3,Branchement eau diam. 40mm 30m3/h et consommations,FT,Non Compris,Non Compris\n"
    // "2.4,Branchement ??lectrique 140 KVA et consommation,FT,Non Compris,Non Compris\n"
    // "2.5,Nettoyage Hebdomadaire par Balayeuse,FT," + to_string(p15) + "," + to_string(p15) + "\n"
    // ",Sous total 1,,," + to_string(p11Etudes + p15) + "\n";

    

    // vector<string> txtList = {
    //     ",", 
    //     "Epaisseur,",
    //     "D??placement maximal,",
    //     "D??placement en t??te,",
    //     "Lin??aire Murette Guide,",
    //     "Nombre de Panneaux,",
    //     "Arase Sup??rieure Plateforme de Travail,",
    //     "Arase Sup??rieure Paroi Moul??e,",
    //     "Arase inf??rieure Paroi Moul??e (m??canique),",
    //     "Arase inf??rieure Paroi Moul??e (hydraulique),",
    //     "Niveau Fond de Fouille,",
    //     "Surface de Forage,",
    //     "Volume de b??ton Th??orique,",
    //     "Volume de D??blais Th??orique,",
    //     "Ratio D'acier,",
    //     "Tonnage d'acier,"
    // };

    // for (int i=0; i<5; i++){
    //     txtList.push_back("Nbre de Torons Lit " + to_string(i+1) + ",");
    //     txtList.push_back("Nombre de Tirants Lit " + to_string(i+1) + ",");
    //     txtList.push_back("Longueur Libre Lit " + to_string(i+1) + ",");
    //     txtList.push_back("Longueur Scell??e Lit " + to_string(i+1) + ",");
    // }
    
    // float VbetonTotal = 0.0;
    // float poidsAcierTotal = 0.0;
    // float SforageTotal = 0.0;

    // for (auto coupe: coupeList) {
    //     int nPanneaux = 0;
    //     float Lmurette = 0.0;
    //     float Sforage = 0.0;
    //     float Vbeton = 0.0;
    //     float Vdeblais = 0.0;
    //     for (auto pan: panneauList) {
    //         if (coupe.id == pan.coupeId) {
    //             nPanneaux++;
    //             Lmurette += pan.length;
    //             Sforage += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"]);
    //             Vbeton += pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;
    //             Vdeblais += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"])*coupe.epaisseur;

    //             SforageTotal += pan.length*(coupe.params["plateforme"] - coupe.params["AIpmH"]);
    //             VbetonTotal += pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;
    //             poidsAcierTotal += ceil(coupe.ratio/5.0)*5.0*pan.length*(coupe.params["ASpm"] - coupe.params["AIpmH"])*coupe.epaisseur;

    //         }
    //         // cout << "id = " << pan.id << ", coupeName: " << coupeList[pan.coupeId].coupeName << ", length = " << pan.length << endl;
    //     }

    //     float dMax=0.0, dTeteMax=0.0;
    //     // string EE = "EE";
    //     for (auto phase: coupe.phases) {
    //         if (mapF::checkKeyInMap(charge::etatName::EE, coupe.etats)) {
    //             if (!vectorF::isInVector(coupe.etats[charge::etatName::EE].phasesList, phase.id)) {
    //                 dMax = max(dMax, phase.dMax);
    //                 dTeteMax = max(dTeteMax, phase.dTeteMax);
    //             }
    //         } else {
    //             dMax = max(dMax, phase.dMax);
    //             dTeteMax = max(dTeteMax, phase.dTeteMax);
    //         }
            
    //     }
    //     txtList[0] += coupe.coupeName + ",";
    //     txtList[1] += to_string(coupe.epaisseur) + ",";
    //     txtList[2] += to_string(round(dMax*100.0)/100.0) + ",";
    //     txtList[3] += to_string(round(dTeteMax*100.0)/100.0) + ",";
    //     txtList[4] += to_string(round(Lmurette*100.0)/100.0) + ",";
    //     txtList[5] += to_string(nPanneaux) + ",";
    //     txtList[6] += to_string(round(coupe.params["plateforme"]*100.0)/100.0) +  ",";
    //     txtList[7] += to_string(round(coupe.params["ASpm"]*100.0)/100.0) +  ",";
    //     txtList[8] += to_string(round(coupe.params["AIpm"]*100.0)/100.0) +  ",";
    //     txtList[9] += to_string(round(coupe.params["AIpmH"]*100.0)/100.0) +  ",";
    //     txtList[10] += to_string(round(coupe.params["FF"]*100.0)/100.0) +  ",";
    //     txtList[11] += to_string(round(Sforage*100.0)/100.0) + ",";
    //     txtList[12] += to_string(round(Vbeton*100.0)/100.0) + ",";
    //     txtList[13] += to_string(round(Vdeblais*100.0)/100.0) + ",";
    //     // txtList[14] += "0,";
    //     // txtList[15] += "0,";
    //     txtList[14] += to_string(ceil(coupe.ratio/5.0)*5.0) +  ",";
    //     txtList[15] += to_string(round(ceil(coupe.ratio/5.0)*5.0*Vbeton)/1000.0) + ",";
    //     int i = 0;
    //     for (auto appui: coupe.appuiList) {

    //         if (appui.type == appuiType::tirant) {
    //             int nTirants = 0;
    //             for (auto pan: panneauList) {
    //                 if ((coupe.id == pan.coupeId) & (!vectorF::isInVector(tirantOff, pan.id))) {
    //                     nTirants += ceil((pan.length-0.01)/appui.espacement);
    //             }}
    //             txtList[16+i] += to_string(appui.nToronts) + ",";
    //             txtList[17+i] += to_string(nTirants) + ",";
    //             txtList[18+i] += to_string(appui.Ll) + ",";
    //             txtList[19+i] += to_string(appui.Ls) + ",";
    //             // std::cout << appui.Ll << " " << appui.Ls << endl;
    //             i+=4;
    //         }
    //     }
    //     // std::cout << i << endl;
    //     if (i < 16) {
    //         for (; i<17; i+=4){
    //             // std::cout << i << endl;
    //             txtList[16+i] += "0,";
    //             txtList[17+i] += "0,";
    //             txtList[18+i] += "0,";
    //             txtList[19+i] += "0,";
    //         }
    //     }
    // }

    // std::cout << "Volumne beton total: " << VbetonTotal << "m3" << std::endl;
    // std::cout << "Surface de forage totale: " << SforageTotal << "m2" << std::endl;
    // std::cout << "Poids d'aciers total: " << ceil(poidsAcierTotal/100.0)/10.0 << "t" << std::endl;

    std::ofstream myfile;
    myfile.open(fileName);
    if (myfile.is_open()) {
        // for (string t: txtList) {
        //     myfile << t << "\n";
        // }
        myfile << txt << "\n";
        myfile.close();
        std::cout << fileName << " created." << endl;
    } else {
        std::cout << fileName << " didn't write" << std::endl;
    }
}
