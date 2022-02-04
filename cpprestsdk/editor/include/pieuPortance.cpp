#include "pieu.h"

void pieu::pieuType::getParams(json pieu) {
    if (pieu["Type"].get<string>() == "barrette") {
        B = pieu["epaisseur"].get<float>();
        if (pieu["perimetre"].type() == json::value_t::number_float) {
            perimetre = pieu["perimetre"].get<float>();
        } else {
            perimetre = B*2.0 + pieu["largeur"].get<float>()*2.0;
        }
        if (pieu["surface"].type() == json::value_t::number_float) {
            surface = pieu["surface"].get<float>();
        } else {
            surface = B*pieu["largeur"].get<float>();
        }
        
    } else {
        B = pieu["diametre"].get<float>();
        if (pieu["perimetre"].type() == json::value_t::number_float) {
            perimetre = pieu["perimetre"].get<float>();
        } else {
            perimetre = PI*B;
        }
        if (pieu["surface"].type() == json::value_t::number_float) {
            surface = pieu["surface"].get<float>();
        } else {
            surface = PI*B*B*0.25;
        }
    }
}


// void pieu::pieuType::getSondages(std::map<std::string, geotech::sondage>& sondagesProjet, json sondagesJson){
//     for (std::string sondageName: sondagesJson) {
//             // std::cout << sondageName << endl;
//             if (mapF::checkKeyInMap(sondageName, sondagesProjet)) {
// 				geotech::sondage newSondage = sondagesProjet[sondageName];
//                 for (auto couche: newSondage.couchesList) {
//                     couche.getKpmax(typeFondation);
//                 }
//                 sondages.push_back(newSondage);
//             } else {
//                 std::cout << "!!!Error: " << sondageName << " not found." << endl;
//                 exit(0);
//             }
//         }
// };

// * ======================================================
// Pression limite nette en point (MPa) (§ F.4.2 (1))
void pieu::element::getPle(geotech::sondage sondage) {
    // Hauteur du pieu dans la formation porteuse
	// for couche in self.coucheList:
	// 	if self.inf >= couche.inf:
	// 		# print self.inf, couche.sup, couche.inf
	// 		break
    
    // geotech::couche& coucheBas;
    // for (auto couche: sondage.couchesList) {
    //     if (inf >= couche.inf) {
    //         coucheBas
    //     }
         
    //     qst += max(min(sup, couche.sup) - max(inf, couche.inf), 0.0f)*couche.qs;
    // }
    geotech::couche couche;
    int i = 0;
    for (auto couche = sondage.couchesList.begin(); (couche != sondage.couchesList.end()) & (inf < couche->inf); ++couche)
        i++;
        
    i = min(i, (int) sondage.couchesList.size() - 1);
    std::cout << sondage.couchesList[i].nom << " " << sondage.couchesList[i].sup << " " << sondage.couchesList[i].inf << " " << inf << std::endl;

	float h = couche.sup - inf;
	float a = max(B/2.0f, 0.5f);
	float b = min(a,h);
	float ple = 0.0;
	float plmin = 100000000.0;
	bool homogene = true;
	// coucheListLocal

	// for (couche in sondage):
	// 	// * ==============================================
	// 	// Si la couche est consideree
	// 	if (min(self.inf+b, couche.sup) - max(self.inf-3.0*a, couche.inf)) > 0.0:
	// 		plmin = min(plmin, couche.pl)
	// 		coucheListLocal.append(couche)
	// 		if len(coucheListLocal) > 1:
	// 			if couche.typeSol != coucheListLocal[len(coucheListLocal)-2].typeSol:
	// 				homogene = False
	// 	self.ple += max(min(self.inf+b, couche.sup) - max(self.inf-3.0*a, couche.inf), 0.0)*couche.pl

    return 0.0;
    // int i=0;
    // for (i=0; (couche != sondage.couchesList.end()) & (inf < couche->inf); ++couche);

    // for (geotech::couche& coucheBas; )

	// if self.norme == "EC":
	
	// 	
		
		
	// 	self.ple /= b + 3.0*a
	// 	if not homogene:
	// 		self.ple = plmin
	// elif self.norme == "DTU":
	// 	self.ple = couche.pl
}

void pieu::element::getQst(geotech::sondage sondage, geotech::typeFondation typeFondation) {
    qst = 0.0;
    for (auto couche: sondage.couchesList) {
        qst += max(min(sup, couche.sup) - max(inf, couche.inf), 0.0f)*couche.qs[typeFondation];
    }
    // return qst;
}

void pieu::element::getKp(geotech::sondage sondage) {
    Def = 0.0;
	for (auto couche: sondage.couchesList) {
        if (inf <= couche.inf) {
            Def += max(min(inf + (float) 10.0*B, min(couche.sup, sup)) - max(inf, couche.inf), (float) 0.0)*couche.pl;
        }
    }

	if ple == 0.0:
		Def = 0.0
	else:
		Def /= self.ple

	// if self.norme == "EC":
	// 	self.gammaRd1['COMP'] = gammaRd1Json['COMP'][couche.typeSol][self.categorie-1]
	// 	self.gammaRd1['TRAC'] = gammaRd1Json['TRAC'][couche.typeSol][self.categorie-1]

	// 	if self.Def > 5.0*self.B :
	// 		self.kp = couche.kpMax
	// 	else:
	// 		self.kp = min(couche.kpMax, 1.0 + (couche.kpMax - 1.0)*(self.Def/self.B)/5.0)
}

void pieu::pieuType::getPortance(vector<charge::charge> chargeList) {
    // map<charge::etatName, float> portanceValue;
	// std::cout << niveauInf << std::endl;
    vector<geotech::typeFondation> fondationForee = {geotech::FS, geotech::FB, geotech::FSR, geotech::FBR, geotech::PU, geotech::barrette};
	for (int i; i<10; i++) {
        float QsTotal = 0.0;

        element newElement(supEff - i*mailleNiveau, supEff - (i+1)*mailleNiveau, B);
        
        for (string sondageNom: sondagesNom) {
            portance newPortance;
            newElement.getQst(sondagesProjet[sondageNom], typeFondation);
            newElement.getKp(sondagesProjet[sondageNom]);
            // newElement.portance[sondageName] = newPortance;
    
            // float coef = 1.0;
            // if ((core::isIn(typeFondation, fondationForee)) & (((newElement.sup*0.5 + newElement.inf) - inf) > 25.0)) {
            //     coef = 0.5;
            // } else {
            //     coef = 1.0;
            // }
            // QsTotal += coef*Ce*newElement.qst*perimetre;
            
		    // float ple = getPle(sondage, B, inf);
        }
        
    }
    
};

void pieu::pieuType::getInf() {
    
};

float pieu::groupeEffet(float d, float B, float m, float n) {
    if (d < B) { return 0.0;}
    if (d >= 3*B) { return 1.0;}
    float Cd = 1.0 - 0.25*(1.0 + d/B);
    return (1.0 - Cd*(2.0-(1.0/m + 1.0/n)));
}