#include "const.h"
#include "rido.h"

void rido::lierne::getMoment() {
    Ma1 = -p*L1*L1/2.0;
    Ma2 = -p*L3*L3/2.0;

    if (L2 < 0.1) {
        Ra1 = p*(L1 + L2 + L3);
        Ra2 = p*(L1 + L2 + L3);
        Mt = min(Ma1, Ma2);

        Va1g = -p*L1;
        Va2d = p*L3;
        Va1d = Va2d;
        Va2g = Va1g;

        if (L1 < 0.1) {
            Va1ge = Va1g;
        } else {
            Va1ge = Va1g*(L1-a1/2.0)/L1;
        }

        if (L3 < 0.1) {
            Va2de = Va2d;
        } else {
            Va2de = Va2d*(L3-a2/2.0)/L3;
        }
        
        Va1de = Va2de;
        Va2ge = Va1ge;

    } else {
        // Ra1 = p/L2*((L1+L2+L3)/2.0 - L3);
        // Ra2 = p/L2*((L1+L2+L3)/2.0 - L1);

        Ra1 = p*(L1+L2+L3)/L2*((L1+L2+L3)/2.0 - L3);
        Ra2 = p*(L1+L2+L3)/L2*((L1+L2+L3)/2.0 - L1);

        float B = (Ma2-Ma1)/L2-p*L2/2.0;
        // Mt = round((Ma1 + B*B/2.0/p)*1000.0)/1000.0;
        // M(x) = R1*(x - L1) - p*x²/2
        // M'(x) = R1 - p * x = 0 => x = R1/p
        // Mmax = R1 (R1/p - L1) - R1²/2/p = R1²/2p - R1*L1
         
        Mt = round((Ra1*Ra1/2.0/p - Ra1*L1)*1000.0)/1000.0;

        Va1g = -p*L1;
        Va1d = Ra1 + Va1g;
        Va2d = p*L2;
        Va2g = Va2d - Ra2;

        if (L1 < 0.1) {
            Va1ge = Va1g;
        } else {
            Va1ge = Va1g*(L1-a1/2.0)/L1;
        }
        
        Va2de = Va2d*(L2-a2/2.0)/L2;

        Va1de = Va1d*(L2-a1/2.0)/L2 + Va2g*a1/(L2*2.0);
        Va2ge = Va2g*(L2-a2/2.0)/L2 + Va1g*a2/(L2*2.0);

    }

    Vmax = max(max(max(-Va1ge, Va1de), Va2de), -Va2ge);

    Ma1e = Ma1 + Ra1*a1/8.0;
    Ma2e = Ma2 + Ra2*a2/8.0;

    
}

void rido::lierne::getArmatures(rido coupe, charge::etatName etatName) {
    // coupe.getSectionBA(etatName);
    
    float b0 = coupe.etats[etatName].sectionBA.b + 0.0f;

    coupe.etats[etatName].sectionBA.b = b;

    // std::cout << coupe.coupeName << " " << coupe.etats[etatName].sectionBA.d << std::endl;
    // DTU 2020
    if (core::isIn(etatName, EAU)) {
        coupe.etats[etatName].sectionBA.fyd = coupe.armaMateriaux.fyd[etatName];
        Aa1 = BAsection::armELS(-Ma1/100.0, coupe.etats[etatName].sectionBA);
        Aa2 = BAsection::armELS(-Ma2/100.0, coupe.etats[etatName].sectionBA);

        if (coupe.impermeable) {
            coupe.etats[etatName].sectionBA.fyd = 200.0f;
        }

        At = BAsection::armELS(Mt/100.0, coupe.etats[etatName].sectionBA);
        // coupe.etats[etatName].sectionBA.fyd = coupe.armaMateriaux.fyd[etatName];

    } else if (core::isIn(etatName, ELU)) {
        // Aa1 = BAsection::armELU(-Ma1/100.0, coupe.etats[etatName].sectionBA);
        // Aa2 = BAsection::armELU(-Ma2/100.0, coupe.etats[etatName].sectionBA);
        // At = BAsection::armELU(Mt/100.0, coupe.etats[etatName].sectionBA);
        // At = BAsection::armELU(Mt/100.0, coupe.etats[etatName].sectionBA);

        std::tuple<float, float> Aa10 = BAsection::armELUAp(-Ma1/100.0, coupe.etats[etatName].sectionBA);
        Aap1 = std::get<0>(Aa10);
        Aa1= std::get<1>(Aa10);

        std::tuple<float, float> Aa20 = BAsection::armELUAp(-Ma2/100.0, coupe.etats[etatName].sectionBA);
        Aap2 = std::get<0>(Aa20);
        Aa2= std::get<1>(Aa20);

        if (Mt > 0.0) {
            std::tuple<float, float> A = BAsection::armELUAp(Mt/100.0, coupe.etats[etatName].sectionBA);
            Atp = std::get<0>(A);
            At = std::get<1>(A);
        }

    } else if (core::isIn(etatName, ELS)) {
        coupe.etats[etatName].sectionBA.fyd = coupe.contrainteDTU.contraintes[HAF];
        Aa1 = BAsection::armELS(-Ma1/100.0, coupe.etats[etatName].sectionBA);
        Aa2 = BAsection::armELS(-Ma2/100.0, coupe.etats[etatName].sectionBA);
        coupe.etats[etatName].sectionBA.fyd = coupe.contrainteDTU.contraintes[HAT];
        if (Mt > 0.0) {
            At = BAsection::armELS(Mt/100.0, coupe.etats[etatName].sectionBA);
        } else {
            At = 0.0;
        }
        
    }

    fyd = coupe.armaMateriaux.fyd[etatName];
    fydF = coupe.etats[etatName].sectionBA.fyd;
    fcd = coupe.etats[etatName].sectionBA.fcd;
    if (coupe.impermeable & core::isIn(etatName, EAU)) {
        fydF = 200.0f;
    }

    coupe.etats[etatName].sectionBA.b = b0;
}

void rido::lierne::verifArmatures(rido coupe, charge::etatName etatName) {
    // coupe.etats[etatName].getSectionBA(etatName);
    
    
    if (Ma1 < 0.0) {
        BAsection::rectangle newSection;
        newSection.beton = coupe.betonMateriaux;
        newSection.arma = coupe.armaMateriaux;
        newSection.b = b;
        newSection.h = coupe.epaisseur;
        newSection.charge.etat = etatName;
        newSection.dsup = 0.1;
        newSection.dinf = 0.1;
        newSection.charge.My = 0.0;
        newSection.charge.N = 0.0;

        newSection.mailleSection();

        // Appuis
        newSection.Asup = nT*PI*HAT*HAT/4000000.0;
        newSection.Ainf = nF*PI*HAF*HAF/4000000.0;

        newSection.getArmatures();

        // newSection.reset();
        newSection.charge.Mx = Ma1/100.0;
        newSection.verif();
        newSection.getMaxMin();
        #ifdef CHECKLierne
        newSection.checkCharges();
        // newSection.checkMateriaux();
        #endif
        outputA1.getOutput(newSection);
    }
    
    if (Ma2 < 0.0) {

        BAsection::rectangle newSection;
        newSection.beton = coupe.betonMateriaux;
        newSection.arma = coupe.armaMateriaux;
        newSection.b = b;
        newSection.h = coupe.epaisseur;
        newSection.charge.etat = etatName;
        newSection.dsup = 0.1;
        newSection.dinf = 0.1;
        newSection.charge.My = 0.0;
        newSection.charge.N = 0.0;

        newSection.mailleSection();

        // Appuis
        newSection.Asup = nT*PI*HAT*HAT/4000000.0;
        newSection.Ainf = nF*PI*HAF*HAF/4000000.0;

        newSection.getArmatures();

        // newSection.reset();
        newSection.charge.Mx = Ma2/100.0;
        newSection.verif();
        newSection.getMaxMin();
        #ifdef CHECKLierne
        newSection.checkCharges();
        // newSection.checkMateriaux();
        #endif
        outputA2.getOutput(newSection);
    }

    // Travées
    if (Mt > 0.0) {
        BAsection::rectangle newSection;
        newSection.beton = coupe.betonMateriaux;
        newSection.arma = coupe.armaMateriaux;
        newSection.b = b;
        newSection.h = coupe.epaisseur;
        newSection.charge.etat = etatName;
        newSection.dsup = 0.1;
        newSection.dinf = 0.1;
        newSection.charge.My = 0.0;
        newSection.charge.N = 0.0;

        newSection.mailleSection();

        // Appuis
        // newSection.Asup = nT*PI*HAT*HAT/4000000.0;
        // newSection.Ainf = nF*PI*HAF*HAF/4000000.0;

        // newSection.getArmatures();
        
        newSection.Asup = nT*PI*HAT*HAT/4000000.0;
        newSection.Ainf = nF*PI*HAF*HAF/4000000.0;
        newSection.getArmatures();

        // newSection.reset();
        newSection.charge.Mx = Mt/100.0;
        newSection.verif();
        newSection.getMaxMin();
        #ifdef CHECKLierne
        newSection.checkCharges();
        // newSection.checkMateriaux();
        #endif

        // std::cout << "ridoLierne: eBmax" << newSection.output.ebMax << std::endl;
        outputT.getOutput(newSection);
    }
}


float getNbarLierne(float n) {
    // Taux 2% de plus
    // std::cout << "getNbarLierne1 " << n << std::endl;
    if (n <= 7.0) {
        // std::cout << "getNbarLierne2 " << ceil(n) << std::endl;
        return max((float) ceil(n*1.02), 4.0f);
    } else {
        // std::cout << "getNbarLierne3 " << 2.0f*ceil(n*0.5f) << std::endl;
        return (float) 2.0f*ceil(n*0.5f*1.02);
    }
}

void rido::paroi::getNewLierne(int coupeId, json param) {
    if (!mapF::checkKeyInMap(coupeList[coupeId].coupeName, lierneList)) {
        lierneList.insert(std::pair<std::string, vector<lierne>>(coupeList[coupeId].coupeName, {}));
    }
    int lit = param["Lit"].get<int>();
    // exit(0);
    // std::cout << param << endl;

    for (auto etat: coupeList[coupeId].etats) {

        // stringF::printVectorInLine(etat.second.phasesList);

        float coef = 1.0;
        if (etat.first == charge::etatName::FOND) {
            coef = 1.35;
        }
        bool isInEtat = false;
        float p = 0.0;
        
        for (auto F: coupeList[coupeId].appuiList[lit-1].Flist) {
            // cout << F.first << endl;
            if (vectorF::isInVector(etat.second.phasesList, F.first)) {
                isInEtat = true;
                p = min(p, F.second);
            }
        }
        // std::cout << coupeList[coupeId].coupeName << ", lit " << lit << " " << charge::getEtatNameString(etat.first) << ", espacement=" << coupeList[coupeId].appuiList[lit-1].espacement << ", coef" << coef << ", p= " << p << ", angle" << coupeList[coupeId].appuiList[lit-1].angle << endl;

        // std::cout << p << std::endl;
        p*=-coef/coupeList[coupeId].appuiList[lit-1].espacement*cos(coupeList[coupeId].appuiList[lit-1].angle*PI/180.0);



        if (isInEtat) {
            // cout << etat.first << param  << endl;
            lierne newLierne;
            newLierne.panID = param["Pan"].get<int>();
            newLierne.lit = param["Lit"].get<int>();
            // newLierne.etatName = etat.first;
            newLierne.etatName = charge::getEtatNameString(etat.first);
            newLierne.type = getAppuiTypeString(coupeList[coupeId].appuiList[newLierne.lit-1].type);
            
            newLierne.label = cageList[panneauList[newLierne.panID-1].cageList[0]-1].label;

            float panLength = panneauList[newLierne.panID-1].length;
            map<std::string, std::string> termes = {{(std::string) "L", to_string(panLength)}};

            newLierne.L1 = jsonF::eval(param["L1"].get<std::string>(), termes);
            newLierne.L2 = jsonF::eval(param["L2"].get<std::string>(), termes);
            newLierne.L3 = jsonF::eval(param["L3"].get<std::string>(), termes);

            // std::cout << "ridoLiernes " << coupeList[coupeId].coupeName << ", lit " << lit << ", L1= " << jsonF::eval(param["L1"].get<std::string>(), termes) << ", L2 =" << jsonF::eval(param["L2"].get<std::string>(), termes) << ", L3=" << jsonF::eval(param["L3"].get<std::string>(), termes) << endl;

            newLierne.a1 = param["a1"].get<float>();
            newLierne.a2 = param["a2"].get<float>();
            //newLierne.nF = param["nF"].get<float>();
            newLierne.HAF = param["HAF"].get<float>();
            //newLierne.nT = param["nT"].get<float>();
            newLierne.HAT = param["HAT"].get<float>();
            
            newLierne.b = 1.0;
            // newLierne.L2 = panLength - newLierne.L1 - newLierne.L3;
            
            newLierne.p = p;

            if (param["p"].get<std::string>() != "x") {
                vector<std::string> values = stringF::splitStr(param["p"].get<std::string>(), "/");
                if (etat.first == charge::etatName::EB) {
                    newLierne.p = stringF::stof(values[0]);
                }
                if (etat.first == charge::etatName::EH) {
                    newLierne.p = stringF::stof(values[1]);
                }
                if (etat.first == charge::etatName::EE) {
                    newLierne.p = stringF::stof(values[2]);
                }
                if (etat.first == charge::etatName::FOND) {
                    newLierne.p = coef*stringF::stof(values[3]);
                }
                // std::cout << newLierne.p << std::endl;
            }

            // std::cout << "fck* =" << coupeList[coupeId].betonMateriaux.fck << std::endl;
            // std::cout << "vmin =" << vmin << std::endl;
            // std::cout << "k =" << k << std::endl;
            // std::cout << "VRdc =" << newLierne.VRdc << std::endl;

            newLierne.niveau = coupeList[coupeId].appuiList[newLierne.lit-1].niveau;
            newLierne.getMoment();
            // std::cout << etat.first << endl;

            
            // charge::etatName etatName = charge::getEtatName(etat.first);
            newLierne.getArmatures(coupeList[coupeId], etat.first);

            // if (param["nF"].get<std::string>() == "x") {
            if (param["nF"].type() == json::value_t::string) {
                // std::cout << "param['nF'].type() " << newLierne.HAF << " " << param["nF"].get<std::string>() << " " << newLierne.At << std::endl;
                newLierne.nF = getNbarLierne(max(newLierne.At, max(newLierne.Aap1,newLierne.Aap2)) *400.0/PI/(newLierne.HAF*newLierne.HAF));
            } else {
                newLierne.nF = param["nF"].get<float>();
            }

            // if (param["nT"].get<std::string>() == "x") {
            if (param["nT"].type() == json::value_t::string) {
                // std::cout << "param['nT'].type() " << newLierne.HAT << " " << param["nT"].get<std::string>() << " " << max(newLierne.Aa1, newLierne.Aa2) << std::endl;
                newLierne.nT = getNbarLierne(max(newLierne.Aa1, max(newLierne.Aa2, newLierne.Atp))*400.0/PI/(newLierne.HAT*newLierne.HAT));
            } else {
                newLierne.nT = param["nT"].get<float>();
            }

            float vmin = 0.23*sqrt(coupeList[coupeId].betonMateriaux.fck);
            float k = 1 + sqrt(200.0f/(coupeList[coupeId].epaisseur*0.9*1000.0));
            float Asl = max(newLierne.nT*PI*newLierne.HAT*newLierne.HAT/4000000.0, newLierne.nF*PI*newLierne.HAF*newLierne.HAF/4000000.0);
            float rhoL = Asl/(coupeList[coupeId].epaisseur*0.9);

            newLierne.VRdc = max(vmin, 0.12f*k*pow(100.0f*rhoL*coupeList[coupeId].betonMateriaux.fck, 0.3333f))*coupeList[coupeId].epaisseur*0.9*100.0;
            
            // if (etat.first == charge::etatName::FOND) {
            newLierne.b = max(1.0f, newLierne.Vmax/newLierne.VRdc);
            // } else {
                // newLierne.b = 1.0;
            // }
            newLierne.VRdc = newLierne.VRdc*newLierne.b;

            // =============== Refaire ======================

            // charge::etatName etatName = charge::getEtatName(etat.first);
            newLierne.getArmatures(coupeList[coupeId], etat.first);

            // if (param["nF"].get<std::string>() == "x") {
            if (param["nF"].type() == json::value_t::string) {
                // std::cout << "param['nF'].type() " << newLierne.HAF << " " << param["nF"].get<std::string>() << " " << newLierne.At << std::endl;
                newLierne.nF = getNbarLierne(max(newLierne.At, max(newLierne.Aap1,newLierne.Aap2)) *400.0/PI/(newLierne.HAF*newLierne.HAF));
            } else {
                newLierne.nF = param["nF"].get<float>();
            }

            // if (param["nT"].get<std::string>() == "x") {
            if (param["nT"].type() == json::value_t::string) {
                // std::cout << "param['nT'].type() " << newLierne.HAT << " " << param["nT"].get<std::string>() << " " << max(newLierne.Aa1, newLierne.Aa2) << std::endl;
                newLierne.nT = getNbarLierne(max(newLierne.Aa1, max(newLierne.Aa2, newLierne.Atp))*400.0/PI/(newLierne.HAT*newLierne.HAT));
            } else {
                newLierne.nT = param["nT"].get<float>();
            }

            vmin = 0.23*sqrt(coupeList[coupeId].betonMateriaux.fck);
            k = 1 + sqrt(200.0f/(coupeList[coupeId].epaisseur*0.9*1000.0));
            Asl = max(newLierne.nT*PI*newLierne.HAT*newLierne.HAT/4000000.0, newLierne.nF*PI*newLierne.HAF*newLierne.HAF/4000000.0);
            rhoL = Asl/(coupeList[coupeId].epaisseur*0.9);

            newLierne.VRdc = max(vmin, 0.12f*k*pow(100.0f*rhoL*coupeList[coupeId].betonMateriaux.fck, 0.3333f))*coupeList[coupeId].epaisseur*0.9*100.0;
            
            // if (etat.first == charge::etatName::FOND) {
            newLierne.b = max(1.0f, newLierne.Vmax/newLierne.VRdc);
            // } else {
                // newLierne.b = 1.0;
            // }
            newLierne.VRdc = newLierne.VRdc*newLierne.b;

            // =============== Refaire ======================

            newLierne.ATdispo = newLierne.nT*PI*newLierne.HAT*newLierne.HAT/400;
            newLierne.AFdispo = newLierne.nF*PI*newLierne.HAF*newLierne.HAF/400;

            newLierne.verifArmatures(coupeList[coupeId], etat.first);

            lierneList[coupeList[coupeId].coupeName].push_back(newLierne);
        }
    }

    
}

void rido::paroi::getLiernes(std::string fileName) {
    vector<json> liernesJson = csv::csv2JsonList(fileName, 1, 2);
    
    for(json a : liernesJson) {
        int coupeId = getCoupeByName(a["Coupe"].get<std::string>());
        if (coupeId > -1) {
            // std::cout << coupeId << " " << coupeList[coupeId].coupeName << endl;
            // std::cout << a << endl;
            getNewLierne(coupeId, a);
        }
    }
    
}

