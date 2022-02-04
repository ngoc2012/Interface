#include "const.h"
#include "rido.h"
#include "geotech.h"

// using namespace std;

void rido::rido::getMateriaux(json param) {
    // cout << "betonMateriaux" << endl;
    betonMateriaux.getJsonParam(param["materiaux"]["beton"]);

    // beton::fondationProfonde fondation = {
    //     geotech::classeFondation[geotech::typeFondation::FB],
    //     model.params["Eppm"],
    //     model.params["ASpm"] - model.params["AIpm"],
    //     true,   // armé
    //     false,   // contrôle renforcé
    // };

    beton::fondationProfonde fondation;
    fondation.classeFondation = geotech::classeFondation[geotech::typeFondation::FB];
    fondation.B = epaisseur;
    fondation.L = params["ASpm"] - params["AIpm"];
    fondation.arme = true;
    fondation.controleRenforce = false;

    betonMateriaux.getFckEffective(fondation);
    
    betonMateriaux.checkBeton();
    armaMateriaux.checkArma();

    contrainteDTU.contrainteDTU2000(armaMateriaux.fyk, betonMateriaux.fck, arma::nappe::EB, false);
}

/*
string getCoteInitial(rido::coteParoi cote) {
    if (cote == rido::coteParoi::fouille) {return "F";}
    if (cote == rido::coteParoi::terre) {return "T";}
    return "";
}

json getJsonData(string data, rido::ridoEtat& etat) {
    // dataName = "ebmaxF" + charge::getEtatNameString(etat.first);
    // output["graphData"][dataName] = json::object();
    // output["graphData"][dataName]["type"] = "ebmax" + charge::getEtatNameString(etatfirst);
    // output["graphData"][dataName]["data"] = json::array();
    // for (auto niv: etat.second.tableDecalee)
    //     output["graphData"][dataName]["data"].push_back(json::array({
    //         round(niv.second.output[coteParoi::fouille].ebMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
    //     }));
    string dataName = "ebmaxF" + charge::getEtatNameString(etat.nom);
    output["graphData"][dataName] = json::object();
    output["graphData"][dataName]["type"] = "ebmax" + charge::getEtatNameString(etatfirst);
    output["graphData"][dataName]["data"] = json::array();
    for (auto niv: etat.second.tableDecalee)
        output["graphData"][dataName]["data"].push_back(json::array({
            round(niv.second.output[coteParoi::fouille].ebMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
        }));
}
*/

void rido::rido::getJsonOuput(string fileName){

    auto output = json::object();
    output["type"] = "graphRidoBA";
    // output["phases"] = json::object();

    output["graphData"] = json::object();
    
    output["graphData"]["armaFDispo"] = json::object();
    output["graphData"]["armaFDispo"]["type"] = "armaDispo";
    output["graphData"]["armaFDispo"]["data"] = json::array();
    map<float, float> arma1 = rido::rido::getArmaDispoLines(coteParoi::fouille);
    for (auto a: arma1) {
        output["graphData"]["armaFDispo"]["data"].push_back(json::array({
            round(a.second*1000.0)/1000.0, round(a.first*1000.0)/1000.0
        }));
    }

    output["graphData"]["armaTDispo"] = json::object();
    output["graphData"]["armaTDispo"]["type"] = "armaDispo";
    output["graphData"]["armaTDispo"]["data"] = json::array();
    arma1 = rido::rido::getArmaDispoLines(coteParoi::terre);
    for (auto a: arma1) {
        output["graphData"]["armaTDispo"]["data"].push_back(json::array({
            -round(a.second*1000.0)/1000.0, round(a.first*1000.0)/1000.0
        }));
    }

    vector<barLine> barList = rido::rido::getBarLines();
    output["graphData"]["barDispo"] = json::object();
    output["graphData"]["barDispo"]["type"] = "barDispo";
    output["graphData"]["barDispo"]["lines"] = json::array();

    output["graphData"]["barTextDispo"] = json::object();
    output["graphData"]["barTextDispo"]["type"] = "barText";
    output["graphData"]["barTextDispo"]["text"] = json::array();
    for (auto b: barList) {
        output["graphData"]["barDispo"]["lines"].push_back(json::array({
            b.x+1.0, round(b.sup*1000.0)/1000.0, b.x+1.0, round(b.inf*1000.0)/1000.0
        }));
        json textElement = json::object();
        textElement["value"] = b.text;
        textElement["coordinate"] = json::array({b.x, round((b.sup*0.5 + b.inf*0.5)*1000.0)/1000.0});
        textElement["angle"] = 90.0;
        output["graphData"]["barTextDispo"]["text"].push_back(textElement);
        json textSup = json::object();
        textSup["value"] = round(b.sup*1000.0)/1000.0;
        textSup["coordinate"] = json::array({b.x, round(b.sup*1000.0)/1000.0});
        textSup["angle"] = 0.0;
        output["graphData"]["barTextDispo"]["text"].push_back(textSup);
        json textInf = json::object();
        textInf["value"] = round(b.inf*1000.0)/1000.0;
        textInf["coordinate"] = json::array({b.x, round(b.inf*1000.0)/1000.0});
        textInf["angle"] = 0.0;
        output["graphData"]["barTextDispo"]["text"].push_back(textInf);
    }

    output["graphData"]["Amin"] = json::object();
    output["graphData"]["Amin"]["type"] = "barDispo";
    output["graphData"]["Amin"]["lines"] = json::array();
    output["graphData"]["Amin"]["lines"].push_back(json::array({
        16.7*(params["Eppm"] - enrobage - 0.015), params["AIpm"], 16.7*(params["Eppm"] - enrobage - 0.015), params["ASpm"]
    }));
    output["graphData"]["Amin"]["lines"].push_back(json::array({
        -16.7*(params["Eppm"] - enrobage - 0.015), params["AIpm"], -16.7*(params["Eppm"] - enrobage - 0.015), params["ASpm"]
    }));

    // section d'armatures disposées
    output["graphData"]["AdispoF"] = json::object();
    output["graphData"]["AdispoF"]["type"] = "Adispo";
    output["graphData"]["AdispoF"]["data"] = json::array();
    for (auto niv: Adispo[coteParoi::fouille])
        output["graphData"]["AdispoF"]["data"].push_back(json::array({
            round(niv.second*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
        }));
    
    output["graphData"]["AdispoT"] = json::object();
    output["graphData"]["AdispoT"]["type"] = "Adispo";
    output["graphData"]["AdispoT"]["data"] = json::array();
    for (auto niv: Adispo[coteParoi::terre])
        output["graphData"]["AdispoT"]["data"].push_back(json::array({
            round(niv.second*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
        }));

    string dataName;
    for (auto etat: etats) {
        
        // Etats limite ultime
        if (vectorF::isInVector(ELU, etat.first)) {
            // ebmax
            dataName = "ebmaxF" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName] = json::object();
            output["graphData"][dataName]["type"] = "ebmax" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName]["data"] = json::array();
            for (auto niv: etat.second.tableDecalee)
                output["graphData"][dataName]["data"].push_back(json::array({
                    // round(niv.second.output[coteParoi::fouille].ebMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                    niv.second.output[coteParoi::fouille].ebMax, round(niv.first*1000.0)/1000.0
                }));


                // output["graphData"][dataName]["data"].push_back(json::array({
                //     niv.second.output[coteParoi::fouille].ebMax, niv.first
                // }));
            
            // for (auto niveau: etat.second.tableDecalee)
            //     std::cout << "ridoArma 160: " << niveau.first << ", Mmin = " << niveau.second.Mmin << ", ebMaxF = " << niveau.second.output[coteParoi::fouille].ebMax << ", Mmax = " << niveau.second.Mmax << ", ebMaxT = " << niveau.second.output[coteParoi::terre].ebMax << endl;

                // output["graphData"][dataName]["data"].push_back(json::array({
                    // round(niv.second.output[coteParoi::fouille].ebMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                // }));
            
            dataName = "ebmaxT" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName] = json::object();
            output["graphData"][dataName]["type"] = "ebmax" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName]["data"] = json::array();
            for (auto niv: etat.second.tableDecalee)
                output["graphData"][dataName]["data"].push_back(json::array({
                    // round(niv.second.output[coteParoi::terre].ebMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                    niv.second.output[coteParoi::terre].ebMax, round(niv.first*1000.0)/1000.0
                }));

                // output["graphData"][dataName]["data"].push_back(json::array({
                //     niv.second.output[coteParoi::terre].ebMax, niv.first
                // }));
            
                // output["graphData"][dataName]["data"].push_back(json::array({
                //     round(niv.second.output[coteParoi::terre].ebMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                // }));
        }

        if (vectorF::isInVector(ELS, etat.first)) {
            // famax
            dataName = "faminF" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName] = json::object();
            output["graphData"][dataName]["type"] = "famin" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName]["data"] = json::array();
            for (auto niv: etat.second.tableDecalee)
                output["graphData"][dataName]["data"].push_back(json::array({
                    round(niv.second.output[coteParoi::fouille].faMin*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                }));
            
            dataName = "faminT" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName] = json::object();
            output["graphData"][dataName]["type"] = "famin" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName]["data"] = json::array();
            for (auto niv: etat.second.tableDecalee)
                output["graphData"][dataName]["data"].push_back(json::array({
                    round(niv.second.output[coteParoi::terre].faMin*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                }));
            
            // fbmax
            dataName = "fbmaxF" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName] = json::object();
            output["graphData"][dataName]["type"] = "fbmax" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName]["data"] = json::array();
            for (auto niv: etat.second.tableDecalee)
                output["graphData"][dataName]["data"].push_back(json::array({
                    round(niv.second.output[coteParoi::fouille].fbMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                }));
            
            dataName = "fbmaxT" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName] = json::object();
            output["graphData"][dataName]["type"] = "fbmax" + charge::getEtatNameString(etat.first);
            output["graphData"][dataName]["data"] = json::array();
            for (auto niv: etat.second.tableDecalee)
                output["graphData"][dataName]["data"].push_back(json::array({
                    round(niv.second.output[coteParoi::terre].fbMax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
                }));
        }
        
        // Armatures calculées
        dataName = "armaF" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName] = json::object();
        output["graphData"][dataName]["type"] = "arma" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName]["data"] = json::array();
        for (auto niv: etat.second.tableDecalee)
            output["graphData"][dataName]["data"].push_back(json::array({
                round(niv.second.A[coteParoi::fouille]*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
            }));
        
        dataName = "armaT" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName] = json::object();
        output["graphData"][dataName]["type"] = "arma" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName]["data"] = json::array();
        for (auto niv: etat.second.tableDecalee)
            output["graphData"][dataName]["data"].push_back(json::array({
                round(niv.second.A[coteParoi::terre]*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
            }));

        // Moments initials
        dataName = "momentMax" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName] = json::object();
        output["graphData"][dataName]["type"] = "moment" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName]["data"] = json::array();
        for (auto niv: etat.second.table)
            output["graphData"][dataName]["data"].push_back(json::array({
                round(niv.second.Mmax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
            }));
        
        dataName = "momentMin" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName] = json::object();
        output["graphData"][dataName]["type"] = "moment" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName]["data"] = json::array();
        for (auto niv: etat.second.table)
            output["graphData"][dataName]["data"].push_back(json::array({
                round(niv.second.Mmin*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
            }));
        
        // Moments décalés
        dataName = "momentDecaleMax" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName] = json::object();
        output["graphData"][dataName]["type"] = "momentDecale" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName]["data"] = json::array();
        for (auto niv: etat.second.tableDecalee)
            output["graphData"][dataName]["data"].push_back(json::array({
                round(niv.second.Mmax*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
            }));
        
        dataName = "momentDecaleMin" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName] = json::object();
        output["graphData"][dataName]["type"] = "momentDecale" + charge::getEtatNameString(etat.first);
        output["graphData"][dataName]["data"] = json::array();
        for (auto niv: etat.second.tableDecalee)
            output["graphData"][dataName]["data"].push_back(json::array({
                round(niv.second.Mmin*1000.0)/1000.0, round(niv.first*1000.0)/1000.0
            }));
        
    }
    ofstream o(fileName);
    o << output.dump(4) << endl;

    cout << fileName << " created" << endl;
}

void rido::ridoEtat::getSectionBA(rido coupe) {
    sectionBA.b = 1.0;
    sectionBA.h = coupe.epaisseur;
    sectionBA.d = coupe.epaisseur - 0.1;
    
    // sectionBA.method = BAsection::method::exact;
    sectionBA.method = BAsection::method::simplified;
    
    sectionBA.Ea = coupe.armaMateriaux.Eyoung;
    sectionBA.Ecm = coupe.betonMateriaux.Ecm;

    if (core::isIn(nom, ELS)) {
        sectionBA.fcd = coupe.betonMateriaux.fcd[charge::etatName::CARA];
        sectionBA.Eb = coupe.betonMateriaux.Eyoung[charge::etatName::CARA];
        if (nom == charge::etatName::EE) {
            sectionBA.fcd = coupe.betonMateriaux.fck/1.2;
        }
    } else {
        sectionBA.fcd = coupe.betonMateriaux.fcd[nom];
        sectionBA.Eb = coupe.betonMateriaux.Eyoung[nom];
    }
    
    sectionBA.fyd = coupe.armaMateriaux.fyd[nom];
    // std::cout << "ridoArma: 291 getSectionBA " << charge::getEtatNameString(nom) << "sectionBA.fcd" << sectionBA.fcd << std::endl;

    // std::cout << "ridoArma: getSectionBA " << charge::getEtatNameString(nom) << " sectionBA.fyd = " << sectionBA.fyd << std::endl;
    // sectionBA.showSectionBAprop();
}

float rido::ridoEtat::getArmaNiveau(rido coupe, float M, float N, float niveau, coteParoi cote) {
    // std::cout << niveau << " ";
    if ((M != 0) | (N < 0)) {
        // if (N < 0) {

        // }
        // DTU 2020
        if (core::isIn(nom, EAU)) {
            return BAsection::armELStraction(M/100.0, N/100.0, sectionBA);
        } else if (core::isIn(nom, ELU)) {
            return BAsection::armELUtraction(M/100.0, N/100.0, sectionBA);
        // DTU 2000
        } else if (core::isIn(nom, ELS)) {
            if (mapF::checkKeyInMap(nom, coupe.contrainteArmaturesELS)) {
                sectionBA.fyd = coupe.contrainteArmaturesELS[nom][cote][niveau];
            }
            return BAsection::armELStraction(M/100.0, N/100.0, sectionBA);
        }
    }
    return 0.0;
}

void rido::rido::getRatio() {
    // for (auto armaLine: armaDispo.armaDispo)
    //     cout << armaLine.coupeName << ", " << armaLine.cote << ", " << armaLine.n << ", " << armaLine.HA << ", " << armaLine.sup << ", " << armaLine.inf << endl;

    float coef = 1.05;

    float poidsBars = 0.0;
    for (auto armaLine: armaDispo.armaDispo) {
        poidsBars += armaLine.n*PI*armaLine.HA*armaLine.HA/400.0*(armaLine.sup - armaLine.inf)*0.785;
    }
    
    // std::cout << "Ratio (bars long) : " << coef*poidsBars/epaisseur/(params["ASpm"] - params["AIpm"]) << std::endl;

    // Armatures transversales
    // Cadre extérieur
    // Lcage = 2,5m pour une longueur 3m, e = 25cm
    poidsBars += (2.5f + epaisseur-0.15 + 15.0*12.0/1000.0)*2.0 * PI*12.0*12.0/400.0*0.785 /0.25 /3.0 * (params["ASpm"] - params["AIpm"]);
    // Cadre intérieur
    poidsBars += 4*(0.5f + epaisseur-0.15 + 15.0*10.0/1000.0)*2.0 * PI*10.0*10.0/400.0*0.785 /0.25 /3.0 * (params["ASpm"] - params["AIpm"]);

    // Renfort liernes
    // getSectionBA(charge::etatName::FOND);
    // At = BAsection::armELU(Mt/100.0, coupe.sectionBA);

    std::cout << "Ratio (bars trans) : " << coef*poidsBars/epaisseur/(params["ASpm"] - params["AIpm"]) << std::endl;

	// # ======================================================================
	// # Poids des renfort de la poutre lierne:
	
	// for lit in param['litButons']:
	// 	Fanc = max(coupe.output['ELU_Reactions'][coupe.output['ELU_Reactions']['i'] == lit]['Fanc'])
	// 	R = Fanc*unite/1000.0*param['panLength']/param['nAppuis']
	// 	M = Fanc*unite/1000.0*pow(param['panLength']/param['nAppuis']/2.0, 2)/2.0
	// 	# print(Fanc, M, R/2.0*param['platine']/4.0)
	// 	(A, Ap) = armELU(M - R/2.0*param['platine']['tirant']/4.0, 
	// 		param['largeur'], 
	// 		coupe.Eppm, 
	// 		coupe.Eppm - materiaux.beton.enrobage/100.0 - encombrBar(param['diaTrans'])/1000.0 - encombrBar(diaF['max'])/2000.0, 
	// 		materiaux.beton.fcd['FOND'],
	// 		materiaux.arma.fyd['FOND'],
	// 		param['EaEb']
	// 	)
	// 	n, HA = renfort(A)
	// 	# coupe.ratio += n * param['panLength'] * poidsBar(HA) / param['panLength'] / coupe.Eppm / Lpan
	// 	coupe.ratio += n * poidsBar(HA) / coupe.Eppm / Lpan
	// 	n, HA = renfort(Ap)
	// 	coupe.ratio += n * poidsBar(HA) / coupe.Eppm / Lpan
	// 	print("Renfort lit ", lit, ": côté terre ", renfort(A), round(A,2), ", côté fouille ", renfort(Ap), round(Ap,2))
	// 	# print(coupe.output['ELU_Reactions'][(coupe.output['ELU_Reactions']['i'] == lit) & (abs(coupe.output['ELU_Reactions']['Fanc']) > 0.0)])
	// 	# print("smt", coupe.output['ELU_Reactions'][coupe.output['ELU_Reactions']['i'] == lit]['k'])
	// 	# if coupe.output['ELU_Reactions'][coupe.output['ELU_Reactions']['i'] == lit]['k'] == 1:
	// 	# 	tirants.append({'Fanc': Fanc})

    float liernes = 0.0;
    if (appuiList.size() > 0) {
        liernes = 10.0;
    }

    ratio = coef*poidsBars/epaisseur/(params["ASpm"] - params["AIpm"]) + liernes;

    std::cout << "Ratio (bars trans) + liernes: " << ratio << std::endl;
}

float getEffortMin(string e, json coupeParam) {
    float ddc = 0.0;
    // std::cout << "e.length() :" << e.length() << std::endl;
    for (auto& el : coupeParam.items()) {
        if ((stringF::substr(el.key(), 0, 3) == "ddc") & (stringF::substr(el.key(), 3, e.length()) == e)) {
            ddc = min(ddc, coupeParam[el.key()].get<float>());
        }
    }
    return ddc;
}

void rido::rido::getArmatures(json coupeParam, std::string fileName) {
    // Armatures disposées
    // rido::armaDispo armaDispo;
    // cout << param["panneautage"][paroiName]["armaDispose"] << endl;
    // armaDispo.getArmaDispo(fileName, coupeName);
    // model.armaDispo.showArma(model.armaDispo.getArmaCoupe("COUPE1_2butons_Note", coteParoi::fouille));
    // model.armaDispo.showArma();

    // getRatio();

    // vector<string> etatList = {"CARA", "FOND", ""}
    // enum etatName { QP, FREQ, CARA, FOND, ACC, EB, EC, EH, EE };
    for ( int eInt = charge::etatName::QP; eInt != charge::etatName::last; eInt++ ) {
        charge::etatName e = static_cast<charge::etatName>(eInt);
        if (coupeParam.contains(charge::getEtatNameString(e))) {
            if (e == charge::etatName::CARA) {
                getContrainteDTU(contrainteDTU.contraintes, e);
            }
            getArmaturesEtat(e, getEffortMin(charge::getEtatNameString(e), coupeParam));

            // std::cout << "ridoArma: " << charge::getEtatNameString(e) << ", N= " << getEffortMin(charge::getEtatNameString(e), coupeParam) << std::endl;
        }
    }
}

void rido::rido::getArmaturesEtat(charge::etatName nom, float N) {
    // charge::etatName etatName = charge::getEtatName(nom);
    float epaisseur = params["Eppm"];
    // etatList.push_back(etatName);
    etats[nom].getSectionBA(*this);
    // std::cout << charge::getEtatNameString(nom) << std::endl;
    for (auto& line: etats[nom].tableDecalee) {
        etats[nom].sectionBA.fyd = armaMateriaux.fyd[nom];
        line.second.A[coteParoi::terre] = -etats[nom].getArmaNiveau(*this, line.second.Mmax, N, line.first, coteParoi::terre);
        if (core::isIn(nom, ELS) & impermeable) {
            etats[nom].sectionBA.fyd = 200.0f;
        }
        line.second.A[coteParoi::fouille] = etats[nom].getArmaNiveau(*this, -line.second.Mmin, N, line.first, coteParoi::fouille);
        etats[nom].sectionBA.fyd = armaMateriaux.fyd[nom];
        // std::cout << line.first << " " << line.second.A[coteParoi::terre] << " " << line.second.A[coteParoi::fouille] << " " << etats[nom].sectionBA.fyd << std::endl;
    }
    
}

void rido::rido::getArmaturesMinimal(bool traction){
    if (traction) {
        Amin = 0.0025*100.0*(epaisseur*100.0 - 10.0); // cm²/m
    } else {
        Amin = 0.00167*100.0*(epaisseur*100.0 - 10.0); // cm²/m
    }
}

float getValue(string value, map<string, float> params) {
    if (stringF::isRealNumber(value)) {
        return stof(value);
    } else {
        for(auto& p : params) {
            stringF::replaceInplace(value, p.first, to_string(p.second));
        }
        const char * c = value.c_str();
        return te_interp(c, 0);
    }
}

void getParams(string line, map<string, float> params) {
    // cout << line.substr(1) << endl;
    int pos = line.find("#");
    vector<string> expr = stringF::splitStr(line.substr(pos+1), "=");
    // cout << expr[0] << ", " << expr[1] << endl;
};

void rido::armaDispo::getArmaDispo(string fileName, string coupeName, map<string, float> params) {
    // cout << "ridoArma - 182: " << fileName << endl;
    std::vector<json> armaDisposeJson = csv::csv2JsonList(fileName, 1, 2);
    bool isCoupeExist = false;
    for(json a : armaDisposeJson) {
        // check valide ...
        // cout << a["Coupe"] << " " << a["n"] << " " << a["HA"] << " " << a["sup"] << " " << a["inf"] << endl;
        if (a["Coupe"] == coupeName) {
            armaDispo.push_back(armaParoi{
                a["Coupe"].get<string>(),
                getCote(a["Cote"].get<string>()),
                a["n"].get<float>(),
                a["HA"].get<float>(),
                a["sup"].get<float>(),
                a["inf"].get<float>(),
                a["n"].get<float>()
                // getValue(a["sup"].get<string>(), params),
                // getValue(a["inf"].get<string>(), params)
            });
            isCoupeExist = true;
        }
    }
    if (!isCoupeExist) {
        std::cout << "Coupe " << coupeName << " does not exist in the file " << fileName << std::endl;
        exit(0);    }
};

void rido::armaDispo::showArmaLine(int i) {
    cout << armaDispo[i].coupeName << ", " << armaDispo[i].cote << ", " << armaDispo[i].n << ", " << armaDispo[i].HA << ", " << armaDispo[i].sup << ", " << armaDispo[i].inf << endl;
};

vector<rido::armaParoi> rido::armaDispo::getArmaCoupe(string coupeName, coteParoi cote) {
    // auto isCoupe = [coupeName]( const arma &i ) { return i.coupeName == coupeName; };
    // auto n = count_if( armaDispo.begin(), armaDispo.end(), isCoupe );
    // vector<arma> armaList;
    // armaList.reserve( n );  // Requests that the vector capacity be at least enough to contain n elements.
    // // cout << armaList << endl;
    // copy_if( armaDispo.begin(), armaDispo.end(), back_inserter( armaList ), armaList );
    vector<armaParoi> armaList;
    for (auto& a: armaDispo) {
        if ((a.coupeName == coupeName) & (a.cote == cote)) {
            armaList.push_back(a);
        }
    }
        
    return armaList;
};
        
void rido::armaDispo::showArma() {
    for (auto armaLine: armaDispo)
        cout << armaLine.coupeName << ", " << armaLine.cote << ", " << armaLine.n << ", " << armaLine.HA << ", " << armaLine.sup << ", " << armaLine.inf << endl;
};

float rido::armaDispo::getAeffectif(float niv, coteParoi cote, float ancrage) {
    float Aeffectif = 0.0;
    for (auto a: armaDispo) {
        if (a.cote == cote) {
            float Lr = ancrage*a.HA/1000.0;
            Aeffectif += arma::getCoefficientEffectif(a.sup, a.inf, niv, Lr)*a.n*a.HA*a.HA*PI/400.0;
        }
    }
    return Aeffectif;
}

float rido::armaDispo::getAeffectif1(float niv, coteParoi cote, map<float, float> ancrageList) {
    float Aeffectif = 0.0;
    for (auto a: armaDispo) {
        if (a.cote == cote) {
            float Lr = ancrageList[a.HA]*a.HA/1000.0;
            Aeffectif += arma::getCoefficientEffectif(a.sup, a.inf, niv, Lr)*a.n*a.HA*a.HA*PI/400.0;
        }
    }
    return Aeffectif;
}

set<float> rido::rido::getNiveauxArmaDispo(coteParoi cote) {
    set<float> niveaux;
    niveaux.insert(params["ASpm"]);
    niveaux.insert(params["AIpm"]);
    float Lr;
    for (auto a: armaDispo.armaDispo) {
        if (a.cote == cote) {
            // Lr = ancrage*a.HA/1000.0;
            Lr = ancrageList[a.HA]*a.HA/1000.0;
            if ((a.sup < params["ASpm"]) & (a.sup > params["AIpm"])) {
                niveaux.insert(a.sup);
            }
            if ((a.inf < params["ASpm"]) & (a.inf > params["AIpm"])) {
                niveaux.insert(a.inf);
            }
            if (((a.sup - Lr) > a.inf) & ((a.sup - Lr) < params["ASpm"]) & ((a.sup - Lr) > params["AIpm"])) {
                niveaux.insert(a.sup - Lr);
            }
            if (((a.inf + Lr) < a.sup) & ((a.inf + Lr) < params["ASpm"]) & ((a.inf + Lr) > params["AIpm"])) {
                niveaux.insert(a.inf + Lr);
            }
        }
    }
    return niveaux;
}

map<float, float> rido::rido::getArmaDispoLines(coteParoi cote) {
    map<float, float> armaCote;

    for (float niv: getNiveauxArmaDispo(cote)) {
        
        armaCote.insert(pair<float,float>(niv, armaDispo.getAeffectif1( niv, cote, ancrageList)));
    }
     
    return armaCote;
};

vector<rido::barLine> rido::rido::getBarLines() {
    vector<barLine> barList;
    map<float, float> armaF = rido::rido::getArmaDispoLines(coteParoi::fouille);
    map<float, float> armaT = rido::rido::getArmaDispoLines(coteParoi::terre);
    float Asup, Ainf;
    for (auto a: armaDispo.armaDispo) {
        barLine newBar;
        newBar.sup = min(params["ASpm"], a.sup);
        newBar.inf = max(params["AIpm"], a.inf);
        if (a.cote == coteParoi::fouille) {
            if (a.sup > params["ASpm"]) {
                Asup = 0.0;
            } else {
                Asup = armaF[newBar.sup];
            }
            if (a.inf < params["AIpm"]) {
                Ainf = 0.0;
            } else {
                Ainf = armaF[newBar.inf];
            }
            // newBar.x = min(Asup, Ainf) + a.n*a.HA*a.HA*PI/400.0 - 5.0;
            newBar.x = armaDispo.getAeffectif1( a.sup - 3.0*ancrageList[a.HA]*a.HA/4000.0, coteParoi::fouille, ancrageList);
        } else if (a.cote == coteParoi::terre) {
            if (a.sup > params["ASpm"]) {
                Asup = 0.0;
            } else {
                Asup = -armaT[newBar.sup];
            }
            if (a.inf < params["AIpm"]) {
                Ainf = 0.0;
            } else {
                Ainf = -armaT[newBar.inf];
            }
            // cout << "ridoArma: 311 - Terre: Asup =" << Asup << ", Ainf =" << Ainf << ", n =" << a.n << ", A =" << a.n*a.HA*a.HA*PI/400.0 << endl;
            // newBar.x = max(Asup, Ainf) - a.n*a.HA*a.HA*PI/400.0 + 5.0;
            
            newBar.x = -armaDispo.getAeffectif1( a.sup - 3.0*ancrageList[a.HA]*a.HA/4000.0, coteParoi::terre, ancrageList);
        }
        
        stringstream stream;
        stream << fixed << setprecision(2) << a.n;
        newBar.text = stream.str() + "HA" + to_string((int) a.HA);
        barList.push_back(newBar);
    }
    return barList;
};

void rido::rido::getContrainteDTU(map<float, float> contrainteDTU, charge::etatName etatName) {
    // charge::etatName etatName = charge::getEtatName(nomEtat);
    vector<coteParoi> coteList = {coteParoi::fouille, coteParoi::terre};
    map<coteParoi, map<float, float>> listCote;

    float diaMax;
    for (coteParoi cote: coteList) {
        map<float, float> listContrainteNiveau;
        for (auto& line: etats[etatName].tableDecalee) {
            diaMax = 5.0;
            for (auto armaLine: armaDispo.armaDispo) {
                if ((armaLine.cote == cote) & (armaLine.sup >= line.second.niveau) & (armaLine.inf <= line.second.niveau)) {
                    diaMax = max(diaMax, armaLine.HA);
                }
            }

            listContrainteNiveau.insert(pair<float, float>(line.first, contrainteDTU[diaMax]));
        }
        listCote.insert(pair<coteParoi, map<float, float>>(cote, listContrainteNiveau));
    }

    contrainteArmaturesELS.insert(pair<charge::etatName, map<coteParoi, map<float, float>>>(etatName, listCote));
}

void rido::rido::checkContrainteArmaturesELS(charge::etatName etatName, coteParoi cote) {
    for (auto line: contrainteArmaturesELS[etatName][cote]) {
        cout << "Niveau: " << line.first << ", Contrainte limite: " << line.second << endl;
    }
};

void rido::rido::getAdispo(charge::etatName etatName) {
    map<float, float> AdispoF, AdispoT;
    
    for (auto niveau: etats[etatName].tableDecalee) {
        AdispoF.insert(pair<float, float>(niveau.first, armaDispo.getAeffectif1(niveau.first, coteParoi::fouille, ancrageList)));
        AdispoT.insert(pair<float, float>(niveau.first, armaDispo.getAeffectif1(niveau.first, coteParoi::terre, ancrageList)));
    }

    Adispo[coteParoi::fouille] = AdispoF;
    Adispo[coteParoi::terre] = AdispoT;
}

vector<BAsection::element> rido::rido::getAdispoT(float niveau, string coorBars, string dispoBars) {
    // map<float, float> AdispoF, AdispoT;
    std::vector<vector<float>> coorBarsCsv = csv::csv2List(coorBars);
    // std::cout << "coorBarsCsv.size()" << coorBarsCsv.size() << std::endl;

    vector<BAsection::element> bars;
    std::vector<json> dispoBarsJson = csv::csv2JsonList(dispoBars, 1, 2);
    for (auto line: dispoBarsJson) {
        // std::cout << line << std::endl;
        // output.dump(4) << endl;
        for (int i: stringF::str2IdList(line["Point"], ",")) {
            if (i>coorBarsCsv.size()) {
                std::cout << "Bar coor n° " << i << " n'existe pas" << std::endl;
            } else {
                float HA = line["HA"].get<float>();
                float sup = line["sup"].get<float>();
                float inf = line["inf"].get<float>();
                float Lr = ancrageList[HA]*HA/1000.0;

                // std::cout << i << std::endl;
                // std::cout << coorBarsCsv[i-1].size() << std::endl;
                // std::cout << coorBarsCsv[i-1][0] << " ";
                // std::cout << coorBarsCsv[i-1][1] << std::endl;

                BAsection::element newElement(coorBarsCsv[i-1][0], coorBarsCsv[i-1][1],
                arma::getCoefficientEffectif(sup, inf, niveau, Lr)*HA*HA*PI/4000000.0);

                // std::cout << niveau << " " << arma::getCoefficientEffectif(sup, inf, niveau, Lr) << " " << arma::getCoefficientEffectif(sup, inf, niveau, Lr)*HA*HA*PI/4000000.0 << std::endl;

                bars.push_back(newElement);
            }
        }
    }
    // for (auto niveau: etats[etatName].tableDecalee) {
    //     AdispoF.insert(pair<float, float>(niveau.first, armaDispo.getAeffectif1(niveau.first, coteParoi::fouille, ancrageList)));
    //     AdispoT.insert(pair<float, float>(niveau.first, armaDispo.getAeffectif1(niveau.first, coteParoi::terre, ancrageList)));
    // }

    // Adispo[coteParoi::fouille] = AdispoF;
    // Adispo[coteParoi::terre] = AdispoT;
    return bars;
}

struct pointCoor {
    float x, y;
    pointCoor(float x0, float y0) {
        x = x0;
        y = y0;
    }
};

float distancePoint2(pointCoor point1, pointCoor point2) {
    return sqrt(pow((point1.x - point2.x),2)+pow((point1.y - point2.y),2));
}

float distancePointLine2(pointCoor point, pointCoor pLine1, pointCoor pLine2) {
    float a = pLine1.y - pLine2.y;
	float b = pLine2.x - pLine1.x;
	float c = pLine1.x*pLine2.y - pLine1.y*pLine2.x;
	if ((a*a + b*b) < 0.00001) {
        return 0.0;
    }
	// float output = (a*point.x + b*point.y + c)/sqrt(a*a + b*b);
	return (a*point.x + b*point.y + c)/sqrt(a*a + b*b);
}
	
vector<BAsection::element> rido::rido::getBetonT(string points, string elements) {
    vector<BAsection::element> sectionBetion;
    std::vector<vector<float>> pointsCsv = csv::csv2List(points);
    std::vector<vector<float>> elementsCsv = csv::csv2List(elements);

    float surfaceTot;
    for (auto e: elementsCsv) {
        
        float s = distancePointLine2( pointCoor(pointsCsv[e[0]][0], pointsCsv[e[0]][1]), pointCoor(pointsCsv[e[1]][0], pointsCsv[e[1]][1]), pointCoor(pointsCsv[e[2]][0], pointsCsv[e[2]][1]))*distancePoint2(pointCoor(pointsCsv[e[1]][0], pointsCsv[e[1]][1]), pointCoor(pointsCsv[e[2]][0], pointsCsv[e[2]][1]))/2.0;

        // std::cout << e[0] << " " << e[0] << " " << e[0] << " " << s << std::endl;
		surfaceTot += s;

        BAsection::element newElement((float) pointsCsv[e[0]][0]/3.0 + pointsCsv[e[1]][0]/3.0 + pointsCsv[e[2]][0]/3.0, (float) pointsCsv[e[0]][1]/3.0 + pointsCsv[e[1]][1]/3.0 + pointsCsv[e[2]][1]/3.0, s);

        sectionBetion.push_back(newElement);
    }

    // std::cout << "surfaceTot=" << surfaceTot << std::endl;
    return sectionBetion;
}

// void rido::rido::verifArmatures(charge::etatName etatName) {
//     // coupe.etats[etatName].getSectionBA(etatName);
//     std::cout << "ridoArma 583 : verifArmatures" << charge::getEtatNameString(etatName) << std::endl;
//     BAsection::rectangle newSection;
//     newSection.beton = betonMateriaux;
//     newSection.arma = armaMateriaux;
//     newSection.b = 1.0;
//     newSection.h = epaisseur;
//     newSection.charge.etat = etatName;
//     newSection.dsup = 0.1;
//     newSection.dinf = 0.1;
//     newSection.charge.My = 0.0;
//     newSection.charge.N = 0.0;

//     newSection.mailleSection();

//     for (auto& niveau: etats[etatName].tableDecalee) {
//         // niveau.second.Adispo[coteParoi::fouille] = armaDispo.getAeffectif(niveau.first, coteParoi::fouille, ancrage);
//         // niveau.second.Adispo[coteParoi::terre] = armaDispo.getAeffectif(niveau.first, coteParoi::terre, ancrage);
        
//         // std::cout << niveau.first << " " << niveau.second.Adispo[coteParoi::fouille] << " " << niveau.second.Adispo[coteParoi::terre] << std::endl;

//         newSection.Asup = Adispo[coteParoi::fouille][niveau.first] / 10000.0;
//         newSection.Ainf = Adispo[coteParoi::terre][niveau.first] / 10000.0;
//         newSection.getArmatures();

//         // std::cout << niveau.first << " " << niveau.second.Mmin << " " << niveau.second.Mmax << std::endl;
//         if ((niveau.second.Mmin < 0.0) & (newSection.Asup > 0.0)) {
//             // newSection.reset();
//             newSection.charge.Mx = niveau.second.Mmin/100.0;
//             newSection.verif();
//             // newSection.getMaxMin<();
//             // newSection.checkCharges();
//             // newSection.checkMateriaux();
//             niveau.second.output[coteParoi::fouille].getOutput(newSection);

//             // std::cout << "ridoArma 626: " << niveau.first << ", Mmin = " << newSection.charge.Mx << ", ebMax = " << newSection.output.ebMax << ", ebMax = " << niveau.second.output[coteParoi::fouille].ebMax << endl;
//         }
//         if ((niveau.second.Mmax > 0.0) & (newSection.Ainf > 0.0)) {
//             // std::cout << "Niveau: " << niveau.first << ", Mmax = " << niveau.second.Mmax << std::endl;
//             // newSection.reset();
//             newSection.charge.Mx = niveau.second.Mmax/100.0;
//             newSection.verif();
//             // newSection.getMaxMin();
//             // newSection.checkSection();
//             // newSection.checkCharges();
//             // newSection.checkMateriaux();
//             niveau.second.output[coteParoi::terre].getOutput(newSection);

//             // std::cout << "ridoArma 638: Niveau = " << niveau.first << ", Mmax = " << newSection.charge.Mx << ", ebMax = " << newSection.output.ebMax << ", ebMax = " << niveau.second.output[coteParoi::terre].ebMax << ", faMin = " << niveau.second.output[coteParoi::terre].faMin << endl;
//         }

//         // std::cout << niveau.first << " " << niveau.second.Mmin << " " << niveau.second.Mmax << std::endl;
//     }
    
//     // for (auto& niveau: etats[etatName].tableDecalee)
//     //     std::cout << "ridoArma 653: " << niveau.first << ", Mmin = " << niveau.second.Mmin << ", ebMaxF = " << niveau.second.output[coteParoi::fouille].ebMax << ", Mmax = " << niveau.second.Mmax << ", ebMaxT = " << niveau.second.output[coteParoi::terre].ebMax << endl;
// }

void rido::rido::verifArmatures(json coupeParam, charge::etatName etatName) {
    // coupe.etats[etatName].getSectionBA(etatName);
    std::cout << "ridoArma 837 : verifArmatures - " << charge::getEtatNameString(etatName) << std::endl;
    

    for (auto& niveau: etats[etatName].tableDecalee) {
        niveau.second.Adispo[coteParoi::fouille] = armaDispo.getAeffectif(niveau.first, coteParoi::fouille, ancrage);
        niveau.second.Adispo[coteParoi::terre] = armaDispo.getAeffectif(niveau.first, coteParoi::terre, ancrage);
        
        // std::cout << niveau.first << " " << niveau.second.Adispo[coteParoi::fouille] << " " << niveau.second.Adispo[coteParoi::terre] << std::endl;

        // std::cout << niveau.second.Mmin << " " << niveau.second.Mmax << " " << Adispo[coteParoi::fouille][niveau.first] << " " << Adispo[coteParoi::terre][niveau.first] << std::endl;

        // std::cout << niveau.first << " " << niveau.second.Mmin << " " << niveau.second.Mmax << std::endl;
        if ((niveau.second.Mmin < 0.0) & (Adispo[coteParoi::fouille][niveau.first] > 0.0)) {

            BAsection::rectangle newSection;
            newSection.beton = betonMateriaux;
            newSection.arma = armaMateriaux;
            newSection.b = 1.0;
            newSection.h = epaisseur;
            newSection.charge.etat = etatName;
            newSection.dsup = 0.1;
            newSection.dinf = 0.1;
            newSection.charge.My = 0.0;
            newSection.charge.N = 0.0;

            newSection.mailleSection();
            // std::cout << "newSection.getArmatures() " << std::endl;
            newSection.Asup = Adispo[coteParoi::fouille][niveau.first] / 10000.0;
            newSection.Ainf = Adispo[coteParoi::terre][niveau.first] / 10000.0;
            newSection.getArmatures();

            // newSection.reset();
            newSection.charge.Mx = niveau.second.Mmin/100.0;
            newSection.charge.N = getEffortMin(charge::getEtatNameString(etatName), coupeParam)/100.0;
            newSection.verif();
            // newSection.getMaxMin<();
            // newSection.checkCharges();
            // newSection.checkMateriaux();
            niveau.second.output[coteParoi::fouille].getOutput(newSection);

            // std::cout << "ridoArma 877: " << niveau.first << ", Mmin = " << newSection.charge.Mx << ", ebMax = " << newSection.output.ebMax << ", ebMax = " << niveau.second.output[coteParoi::fouille].ebMax << endl;
        }
        if ((niveau.second.Mmax > 0.0) & (Adispo[coteParoi::terre][niveau.first] > 0.0)) {

            BAsection::rectangle newSection;
            newSection.beton = betonMateriaux;
            newSection.arma = armaMateriaux;
            newSection.b = 1.0;
            newSection.h = epaisseur;
            newSection.charge.etat = etatName;
            newSection.dsup = 0.1;
            newSection.dinf = 0.1;
            newSection.charge.My = 0.0;
            newSection.charge.N = 0.0;

            // std::cout << "newSection.mailleSection() " << epaisseur << " " << newSection.maille << std::endl;
            newSection.mailleSection();
            // std::cout << "newSection.getArmatures() " << std::endl;
            newSection.Asup = Adispo[coteParoi::fouille][niveau.first] / 10000.0;
            newSection.Ainf = Adispo[coteParoi::terre][niveau.first] / 10000.0;
            newSection.getArmatures();

            // std::cout << "Niveau: " << niveau.first << ", Mmax = " << niveau.second.Mmax << std::endl;
            // newSection.reset();
            newSection.charge.Mx = niveau.second.Mmax/100.0;
            newSection.charge.N = getEffortMin(charge::getEtatNameString(etatName), coupeParam)/100.0;
            newSection.verif();
            // newSection.getMaxMin();
            // newSection.checkSection();
            // newSection.checkCharges();
            // newSection.checkMateriaux();
            niveau.second.output[coteParoi::terre].getOutput(newSection);

            // std::cout << "ridoArma 909: Niveau = " << niveau.first << ", Mmax = " << newSection.charge.Mx << ", ebMax = " << newSection.output.ebMax << ", ebMax = " << niveau.second.output[coteParoi::terre].ebMax << ", faMin = " << niveau.second.output[coteParoi::terre].faMin << endl;
        }

        // std::cout << niveau.first << " " << niveau.second.Mmin << " " << niveau.second.Mmax << std::endl;
    }
    
    // for (auto& niveau: etats[etatName].tableDecalee)
    //     std::cout << "ridoArma 653: " << niveau.first << ", Mmin = " << niveau.second.Mmin << ", ebMaxF = " << niveau.second.output[coteParoi::fouille].ebMax << ", Mmax = " << niveau.second.Mmax << ", ebMaxT = " << niveau.second.output[coteParoi::terre].ebMax << endl;
}

void rido::rido::verifArmaturesT(json coupeParam, charge::etatName etatName, string points, string elements, string coorBars, string dispoBars) {
    // coupe.etats[etatName].getSectionBA(etatName);
    std::cout << "ridoArma 583 : verifArmaturesT " << charge::getEtatNameString(etatName) << std::endl;
    
    vector<BAsection::element> sectionBetion = getBetonT(points, elements);
    // BAsection::sectionBetonProp(sectionBetion);

    for (auto& niveau: etats[etatName].tableDecalee) {
        // niveau.second.Adispo[coteParoi::fouille] = armaDispo.getAeffectif(niveau.first, coteParoi::fouille, ancrage);
        // niveau.second.Adispo[coteParoi::terre] = armaDispo.getAeffectif(niveau.first, coteParoi::terre, ancrage);
        
        // std::cout << niveau.first << " " << niveau.second.Adispo[coteParoi::fouille] << " " << niveau.second.Adispo[coteParoi::terre] << std::endl;

        // std::cout << niveau.first << " " << niveau.second.Mmin << " " << niveau.second.Mmax << std::endl;
        // if ((niveau.second.Mmin < 0.0) & (Adispo[coteParoi::fouille][niveau.first] > 0.0)) {
        if (niveau.second.Mmin < 0.0) {

            BAsection::BAsection2D newSection;
            newSection.beton = betonMateriaux;
            newSection.arma = armaMateriaux;
            newSection.charge.etat = etatName;
            newSection.charge.My = 0.0;
            newSection.charge.N = 0.0;

            newSection.charge.Mx = niveau.second.Mmin/100.0;
            newSection.charge.N = getEffortMin(charge::getEtatNameString(etatName), coupeParam)/100.0;

            newSection.sectionBeton = sectionBetion;

            // vector<BAsection::element> dispoArma = getAdispoT(niveau.first, coorBars, dispoBars);
            newSection.dispoArma = getAdispoT(niveau.first, coorBars, dispoBars);

            float Atotal = 0.0;
            for (auto b: newSection.dispoArma)
                Atotal += b.surface;
            if (Atotal > 0.0) {
                newSection.verif();
            }
            // 
            // // newSection.getMaxMin();
            // // newSection.checkSection();
            // // newSection.checkCharges();
            // // newSection.checkMateriaux();
            niveau.second.output[coteParoi::fouille].getOutput(newSection);

            // std::cout << "ridoArma 626: " << niveau.first << ", Atotal = " << Atotal << ", Mmin = " << newSection.charge.Mx << ", ebMax = " << newSection.output.ebMax << ", ebMax = " << niveau.second.output[coteParoi::fouille].ebMax << endl;
        }
        if (niveau.second.Mmax > 0.0) {

            BAsection::BAsection2D newSection;
            newSection.beton = betonMateriaux;
            newSection.arma = armaMateriaux;
            newSection.charge.etat = etatName;
            newSection.charge.My = 0.0;
            newSection.charge.N = 0.0;

            // std::cout << "Niveau: " << niveau.first << ", Mmax = " << niveau.second.Mmax << std::endl;
            // newSection.reset();
            newSection.charge.Mx = niveau.second.Mmax/100.0;
            newSection.charge.N = getEffortMin(charge::getEtatNameString(etatName), coupeParam)/100.0;

            newSection.sectionBeton = sectionBetion;
            newSection.dispoArma = getAdispoT(niveau.first, coorBars, dispoBars);

            float Atotal = 0.0;
            for (auto b: newSection.dispoArma)
                Atotal += b.surface;
            if (Atotal > 0.0) {
                newSection.verif();
            }

            // newSection.verif();
            // // newSection.getMaxMin();
            // // newSection.checkSection();
            // // newSection.checkCharges();
            // // newSection.checkMateriaux();
            niveau.second.output[coteParoi::terre].getOutput(newSection);

            // std::cout << "ridoArma 638: Niveau = " << niveau.first << ", Atotal = " << Atotal << ", Mmax = " << newSection.charge.Mx << ", ebMax = " << newSection.output.ebMax << ", ebMax = " << niveau.second.output[coteParoi::terre].ebMax << ", faMin = " << niveau.second.output[coteParoi::terre].faMin << endl;
        }

        // std::cout << niveau.first << " " << niveau.second.Mmin << " " << niveau.second.Mmax << std::endl;
    }
    
    // for (auto& niveau: etats[etatName].tableDecalee)
    //     std::cout << "ridoArma 653: " << niveau.first << ", Mmin = " << niveau.second.Mmin << ", ebMaxF = " << niveau.second.output[coteParoi::fouille].ebMax << ", Mmax = " << niveau.second.Mmax << ", ebMaxT = " << niveau.second.output[coteParoi::terre].ebMax << endl;
}
