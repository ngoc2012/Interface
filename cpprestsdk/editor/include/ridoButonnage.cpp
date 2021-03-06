#include "rido.h"

using namespace std;

json rido::bout::getJson() {
	
    return json {
        {"id", id},
        {"lit", lit},
        {"cote", cote},
        {"niveau", niveau},
        {"angle", angle},
        {"influence", influence},
        {"F", F},
        {"K", K},
        {"espacement", espacement},
        {"precontrainte", precontrainte},
		{"precontrainte0", precontrainte0},
        {"Lu", Lu},
        {"Ll", Ll},
        {"Ls", Ls},
        {"type", getAppuiTypeString(type)},
        {"fck", fck},
        {"panneauId", panneauId},
        {"P", P},
        {"Fa", Fa},
        {"Kr", Kr},
        {"Ft", Ft},
        {"T", T},
        {"N", N},
        {"Ksol", Ksol},
        {"platine", platine.getJson()},
        {"assemblage", assemblage.getJson()}
    };
};

json rido::butonMetal::getJson() {
    return json {
        {"length", length},
        {"id", id},
        {"label", label},
        {"lit", lit},
        {"profileNom", profileNom},
        {"startPan", startPan},
        {"endPan", endPan},
        {"startCoupe", startCoupe},
        {"endCoupe", endCoupe},
        {"precontrainte", precontrainte},
		{"LA", LA},
        {"GL", GL},
        {"PL", PL},
        {"TG", TG},
		{"TD", TD},
        {"FaMax", FaMax},
        {"poids", poids},
        {"FEdx", FEdx},
        {"FEdz", FEdz},
        {"MEdy", MEdy},
        {"NEd", NEd},
        {"fck", fck},
        {"Dmax", Dmax},
        {"scplim", scplim},
        {"fcd", fcd},
        {"frottementBetonBeton", frottementBetonBeton},
        {"frottementAcierBeton", frottementAcierBeton},
        {"mortierFck", mortierFck},
        {"dilatationT", dilatationT},
        {"startBout", startBout.getJson()},
        {"endBout", endBout.getJson()},
        {"profile", profile.getJson()},
        {"flexionComposee", flexionComposee.getJson()},
		{"butonJson", butonJson},
		{"Kt", Kt},
		{"semelle", semelle.getJson()}
    };
}

void rido::paroi::checkButonList() {
	cout << "Butons métal list: (" << butonList.size() << ")" << endl;
	// for (auto but: butonList) {
	int butonSize = butonList.size();
	for (int i=0; i<butonSize; i++) {
		butonMetal but = butonList[i];
		cout << "id = " << but.id << ", label = " << but.label << ", profile = " << but.profileNom << ", niveau1 = " << but.startBout.niveau << ", platine1.tp = " << but.startBout.platine.tp << endl;
		// cout << "id = " << but.id << ", label = " << but.label << ", profile = " << but.profileNom << ", niveau1 = " << but.startBout.niveau << ", influ1 = " << but.startBout.influence << ", angle1 = " << but.startBout.angle << ", K1 = " << but.startBout.K << ", F1 = " << but.startBout.F << ", lit1 = " << but.startBout.lit << ", pan1.id = " << but.startBout.panneauId << ", pan1Length = " << panneauList[but.startBout.panneauId-1].length << endl;
		// cout << ", niveau2 = " << but.endBout.niveau << ", influ2 = " << but.endBout.influence << ", angle2 = " << but.endBout.angle << ", K2 = " << but.endBout.K << ", F2 = " << but.endBout.F << ", lit2 = " << but.endBout.lit << ", pan2.id = " << but.endBout.panneauId << ", pan2.length = " << panneauList[but.startBout.panneauId-1].length << endl;
	}
}

void rido::paroi::butonnage(string fileButonDispo, json& Cprofiles) {
	cout << "Butonnage : " << endl;
    float toleranceButon = 0.4;
    vector<json> butonListLine = csv::csv2JsonList(fileButonDispo, 1, 1);
	panneau *startBoutPan;
	panneau *endBoutPan;
	rido *startBoutCoupe;
	rido *endBoutCoupe;

	int i = 0;
	int butonId = 0;
	for (json but: butonListLine) {
		i++;

		int startBoutPanId = stoi(stringF::replace(but["pan1"], "P", ""));
		int endBoutPanId = stoi(stringF::replace(but["pan2"], "P", ""));
		// cout << but["pan1"] << " " << startBoutPanId << " " << but["pan2"] << " " << endBoutPanId << endl;

		vector<int> lits1 = stringF::str2IdList(but["lits1"], ",");
		vector<int> lits2 = stringF::str2IdList(but["lits2"], ",");
		
		// std::cout << "Lit buton" << std::endl;
		int j = 0;
		for (int lit1: lits1) {
			butonMetal newButon;
			newButon.startBout.panneauId = startBoutPanId;
			newButon.endBout.panneauId = endBoutPanId;

			newButon.startBout.type = appuiType::buton;
			newButon.endBout.type = appuiType::buton;

			newButon.lit = lit1;
			newButon.label = "B" + to_string(lit1*100 + i);

			// cout << newButon.label << endl;
			// Raideur de sols
			// if (but.contains(but["Ksol1"])) {newButon.startBout.Ksol = but["Ksol1"].get<float>();}
			// if (but.contains(but["Ksol2"])) {newButon.endBout.Ksol = but["Ksol2"].get<float>();}

			newButon.butonJson = but;
			// distanceAuMilieu
			// Largeur d'influence 
			if (but["infl1"].get<float>() == 0.0) {
				newButon.startBout.influence = panneauList[startBoutPanId-1].length;
			} else {
				newButon.startBout.influence = but["infl1"].get<float>();
			}
			if (but["infl2"].get<float>() == 0.0) {
				newButon.endBout.influence = panneauList[endBoutPanId-1].length;
			} else {
				newButon.endBout.influence = but["infl2"].get<float>();
			}

			vectorF::checkSize(panneauList, startBoutPanId - 1, "startBoutPanId", "panneauList");
			vectorF::checkSize(panneauList, endBoutPanId - 1, "endBoutPanId", "panneauList");

			startBoutPan = &panneauList[startBoutPanId-1];
			endBoutPan = &panneauList[endBoutPanId-1];

			newButon.startPan = startBoutPan->label;
			newButon.endPan = endBoutPan->label;

			if (but["panLabel1"].get<int>() != 0) {newButon.startPan = stringF::ftos(but["panLabel1"].get<float>(),0);}
			if (but["panLabel2"].get<int>() != 0) {newButon.endPan = stringF::ftos(but["panLabel2"].get<float>(),0);}

			vectorF::checkSize(coupeList, startBoutPan->coupeId, "startBoutPan->coupeId", "coupeList");
			vectorF::checkSize(coupeList, endBoutPan->coupeId, "endBoutPan->coupeId", "coupeList");

			startBoutCoupe = &coupeList[startBoutPan->coupeId];
			endBoutCoupe = &coupeList[endBoutPan->coupeId];

			newButon.startCoupe = startBoutCoupe->coupeName;
			newButon.endCoupe = endBoutCoupe->coupeName;
			
			// Lit
			if (j > lits2.size()) {
				stringF::printVectorInLine(lits1);
				stringF::printVectorInLine(lits2);
				cout << "size(Lits 1) > size(Lits2);" << endl;
			}

			newButon.startBout.lit = lit1;
			newButon.endBout.lit = lits2[j];

			vectorF::checkSize(startBoutCoupe->appuiList, lit1 - 1, "lit1", "startBoutCoupe->appuiList");
			vectorF::checkSize(endBoutCoupe->appuiList, lits2[j] - 1, "lit1", "startBoutCoupe->appuiList");

			// Niveau
			newButon.startBout.niveau = startBoutCoupe->appuiList[lit1-1].niveau;
			newButon.endBout.niveau = endBoutCoupe->appuiList[lits2[j]-1].niveau;

			// Effort
			newButon.startBout.F = -startBoutCoupe->appuiList[lit1-1].F;
			newButon.endBout.F = -endBoutCoupe->appuiList[lits2[j]-1].F;
			
			// Raideur
			newButon.startBout.K = startBoutCoupe->appuiList[lit1-1].K;
			newButon.endBout.K = endBoutCoupe->appuiList[lits2[j]-1].K;

			// Précontrainte
			newButon.startBout.precontrainte0 = startBoutCoupe->appuiList[lit1-1].precontrainte*but["coefPre"].get<float>();
			newButon.endBout.precontrainte0 = endBoutCoupe->appuiList[lits2[j]-1].precontrainte*but["coefPre"].get<float>();
			
			

			newButon.start.x = startBoutPan->start.x*0.5 + startBoutPan->end.x*0.5 + startBoutPan->vectorUnite.x*but["dis1"].get<float>() + startBoutPan->perpUnite.x*but["face1"].get<float>();
			newButon.start.y = startBoutPan->start.y*0.5 + startBoutPan->end.y*0.5 + startBoutPan->vectorUnite.y*but["dis1"].get<float>() + startBoutPan->perpUnite.y*but["face1"].get<float>();

			newButon.end.x = endBoutPan->start.x*0.5 + endBoutPan->end.x*0.5 + endBoutPan->vectorUnite.x*but["dis2"].get<float>() + endBoutPan->perpUnite.x*but["face2"].get<float>();
			newButon.end.y = endBoutPan->start.y*0.5 + endBoutPan->end.y*0.5 + endBoutPan->vectorUnite.y*but["dis2"].get<float>() + endBoutPan->perpUnite.y*but["face2"].get<float>();

			newButon.getLength();
			newButon.getVectorUnite();
			
			// Angle
			float angle1 = mathF::angle2DVector(startBoutPan->vectorUnite, newButon.vectorUnite)*180.0/PI;
			if (angle1 > 90.0) {angle1 = 180.0 - angle1;}

			float angle2 = mathF::angle2DVector(endBoutPan->vectorUnite, newButon.vectorUnite)*180.0/PI;
			if (angle2 > 90.0) {angle2 = 180.0 - angle2;}

			newButon.startBout.angle = angle1;
			newButon.endBout.angle = angle2;

			newButon.startBout.platine.tp = but["tp1"].get<float>();
			newButon.endBout.platine.tp = but["tp2"].get<float>();

			newButon.startBout.precontrainte = startBoutCoupe->appuiList[lit1-1].precontrainte*newButon.startBout.influence/sin(newButon.startBout.angle*PI/180.0)*but["coefPre"].get<float>();
			newButon.endBout.precontrainte = endBoutCoupe->appuiList[lits2[j]-1].precontrainte*newButon.endBout.influence/sin(newButon.endBout.angle*PI/180.0)*but["coefPre"].get<float>();
			
			newButon.precontrainte = min(newButon.startBout.precontrainte, newButon.endBout.precontrainte);
			// std::cout << newButon.startBout.influence << ", " << newButon.startBout.angle << ", " << sin(newButon.startBout.angle*180.0/PI) << ", " << newButon.startBout.precontrainte << ", " << newButon.endBout.influence << ", " << newButon.endBout.angle << ", " << sin(newButon.endBout.angle/180.0*PI) << ", " << newButon.endBout.precontrainte << std::endl;

			j++;
			butonId++;
			newButon.id = butonId;

			newButon.Cprofiles = &Cprofiles;
			newButon.getMateriaux(materiaux);

			if (but["Dmax"] != "x") {
				
				for (auto s: stringF::splitStr(but["Dmax"], ",")) {
					vector<string> s0 = stringF::splitStr(s, "-");
					if (newButon.lit == stoi(s0[0])) {
						// std::cout << "newButon.Dmax = " << newButon.Dmax << std::endl;
						// std::cout << newButon.lit << std::endl;
						// std::cout << but["Dmax"] << std::endl;
						newButon.Dmax = stringF::stof(s0[1]);
						// std::cout << "newButon.Dmax = " << newButon.Dmax << std::endl;
					}
				}
			}
			newButon.dimButons();

			cout << newButon.label << "  " << newButon.profileNom << endl;

			// std::cout << "End dim" << std::endl;

			butonList.push_back(newButon);

			// std::cout << "End buton dim" << std::endl;
		}
	}

}

void rido::paroi::braconnage(string fileButonDispo, json& Cprofiles) {
	cout << "Braconnage : " << endl;
    float toleranceButon = 0.4;
    vector<json> butonListLine = csv::csv2JsonList(fileButonDispo, 1, 1);
	panneau *startBoutPan;
	panneau *endBoutPan;
	rido *startBoutCoupe;
	rido *endBoutCoupe;

	int i = 0;
	int butonId = 0;
	for (json but: butonListLine) {
		i++;

		// int startBoutPanId = stoi(stringF::replace(but["pan"], "P", ""));
		vector<int> startBoutPanIds = stringF::str2IdList(but["pan"], ",");
		// cout << but["pan1"] << " " << startBoutPanId << " " << but["pan2"] << " " << endBoutPanId << endl;

		vector<int> lits1 = stringF::str2IdList(but["lits"], ",");
		
		// std::cout << "Lit buton" << std::endl;
		int j = 0;
		for (int lit1: lits1) {
			for (int id0: startBoutPanIds) {
				butonMetal newButon;
				newButon.startBout.panneauId = id0;
				newButon.endBout.panneauId = id0;

				newButon.startBout.type = appuiType::bracon;
				newButon.endBout.type = appuiType::bracon;

				newButon.lit = lit1;
				newButon.label = "Br" + to_string(lit1*100 + i);

				
				// Raideur de sols
				// if (but.contains(but["Ksol1"])) {newButon.startBout.Ksol = but["Ksol1"].get<float>();}
				// if (but.contains(but["Ksol2"])) {newButon.endBout.Ksol = but["Ksol2"].get<float>();}

				newButon.butonJson = but;

				startBoutPan = &panneauList[id0-1];
				endBoutPan = &panneauList[id0-1];

				// Largeur d'influence 
				map<std::string, std::string> termes = {{(std::string) "L", to_string(startBoutPan->length)}};
            	newButon.startBout.influence = jsonF::eval(but["infl"].get<std::string>(), termes);
				newButon.endBout.influence = newButon.startBout.influence;
				// vectorF::checkSize(panneauList, startBoutPanId - 1, "startBoutPanId", "panneauList");
				// vectorF::checkSize(panneauList, endBoutPanId - 1, "endBoutPanId", "panneauList");

				
				newButon.startPan = startBoutPan->label;
				newButon.endPan = startBoutPan->label;

				if (but["panLabel"].get<int>() != 0) {
					newButon.startPan = stringF::ftos(but["panLabel"].get<float>(),0);
					newButon.endPan = stringF::ftos(but["panLabel"].get<float>(),0);
				}

				vectorF::checkSize(coupeList, startBoutPan->coupeId, "startBoutPan->coupeId", "coupeList");
				vectorF::checkSize(coupeList, endBoutPan->coupeId, "endBoutPan->coupeId", "coupeList");

				startBoutCoupe = &coupeList[startBoutPan->coupeId];
				endBoutCoupe = &coupeList[endBoutPan->coupeId];

				newButon.startCoupe = startBoutCoupe->coupeName;
				newButon.endCoupe = endBoutCoupe->coupeName;
				
				// Lit
				newButon.startBout.lit = lit1;
				newButon.endBout.lit = lit1;

				vectorF::checkSize(startBoutCoupe->appuiList, lit1 - 1, "lit1", "startBoutCoupe->appuiList");
				
				float Bs = but["Bs"].get<float>();
				float Ls = but["Ls"].get<float>();
				float Hs = but["Hs"].get<float>();
				float De = but["De"].get<float>();
				float alpha = but["alpha"].get<float>();
				float delta = but["delta"].get<float>();
				float beta = 90.0f - alpha - delta;

				float Lo = but["epS"].get<float>()/cos(delta*PI/180.0f);

				// Niveau
				newButon.startBout.niveau = startBoutCoupe->appuiList[lit1-1].niveau;
				newButon.endBout.niveau = endBoutCoupe->params["FF"] - but["De"].get<float>() + Lo*sin(alpha*PI/180.0f);

				float lenBracon = (newButon.startBout.niveau - newButon.endBout.niveau)/sin(alpha*PI/180.0f);

				// Effort
				newButon.startBout.F = -startBoutCoupe->appuiList[lit1-1].F;
				newButon.endBout.F = -endBoutCoupe->appuiList[lit1-1].F;
				
				// Raideur
				newButon.startBout.K = startBoutCoupe->appuiList[lit1-1].K;
				newButon.endBout.K = endBoutCoupe->appuiList[lit1-1].K;

				// Précontrainte
				newButon.startBout.precontrainte0 = startBoutCoupe->appuiList[lit1-1].precontrainte*but["coefPre"].get<float>();
				newButon.endBout.precontrainte0 = endBoutCoupe->appuiList[lit1-1].precontrainte*but["coefPre"].get<float>();
				
				
				newButon.start.x = startBoutPan->start.x*0.5 + startBoutPan->end.x*0.5 + startBoutPan->vectorUnite.x*but["dis"].get<float>() + startBoutPan->perpUnite.x*but["face"].get<float>();
				newButon.start.y = startBoutPan->start.y*0.5 + startBoutPan->end.y*0.5 + startBoutPan->vectorUnite.y*but["dis"].get<float>() + startBoutPan->perpUnite.y*but["face"].get<float>();

                if (but.contains("dis2")) {
                    newButon.end.x = newButon.start.x + endBoutPan->perpUnite.x*lenBracon + startBoutPan->vectorUnite.x*but["dis2"].get<float>();
                    newButon.end.y = newButon.start.y + endBoutPan->perpUnite.y*lenBracon + startBoutPan->vectorUnite.y*but["dis2"].get<float>();
                } else {
                    newButon.end.x = newButon.start.x + endBoutPan->perpUnite.x*lenBracon;
                    newButon.end.y = newButon.start.y + endBoutPan->perpUnite.y*lenBracon;
                }

				newButon.getLength();
				newButon.getVectorUnite();
				
				
				// Angle
				// float angle1 = mathF::angle2DVector(startBoutPan->vectorUnite, newButon.vectorUnite)*180.0/PI;
				// if (angle1 > 90.0) {angle1 = 180.0 - angle1;}

				newButon.startBout.angle = 90.0f - alpha;
				newButon.endBout.angle = 90.0f - delta;

				newButon.startBout.platine.tp = but["tp"].get<float>();
				newButon.endBout.platine.tp = but["tpS"].get<float>();

				newButon.startBout.precontrainte = startBoutCoupe->appuiList[lit1-1].precontrainte*newButon.startBout.influence/sin(newButon.startBout.angle*PI/180.0)*but["coefPre"].get<float>();
				newButon.endBout.precontrainte = newButon.startBout.precontrainte;
				
				newButon.precontrainte = newButon.startBout.precontrainte;
				// std::cout << newButon.startBout.influence << ", " << newButon.startBout.angle << ", " << sin(newButon.startBout.angle*180.0/PI) << ", " << newButon.startBout.precontrainte << ", " << newButon.endBout.influence << ", " << newButon.endBout.angle << ", " << sin(newButon.endBout.angle/180.0*PI) << ", " << newButon.endBout.precontrainte << std::endl;

				newButon.semelle.d_sol = De/sin(PI*0.5f - alpha*PI/180.0f) - 0.5f*Bs;
				newButon.semelle.kp = but["k_p0"].get<float>() + (but["a_sol"].get<float>() + but["b_sol"].get<float>()*De/Bs)*( 1.0f - exp(-but["c_sol"].get<float>()*De/Bs));

				newButon.semelle.i_beta = 1.0f - beta/180.0f*pow(1.0f - newButon.semelle.d_sol/8.0/Bs, 2.0);

				float delta_d = delta/90.0f;

				newButon.semelle.i_delta = pow(1.0f - delta_d, 2.0) - delta_d*(2.0f - 3.0f * delta_d)*exp(-De/Bs);
				newButon.semelle.qnet = newButon.semelle.kp*but["ple"].get<float>()*newButon.semelle.i_beta*newButon.semelle.i_delta;

				newButon.semelle.Rvd = newButon.semelle.qnet*Bs*Ls*100.0/1.40;
				newButon.semelle.taux = newButon.startBout.F*newButon.startBout.influence/cos(alpha*PI/180.0f)/newButon.semelle.Rvd;

				float beta_v = 1.55*pow(Ls/Bs, 0.25f) + 0.8f*pow(Bs/Ls, 0.5f);
				newButon.semelle.Ks = 2.0*but["Em"].get<float>()*100.0f*beta_v*pow(Bs*Ls, 0.5)/(1.0f - 0.3*0.3)*0.5;

				j++;
				butonId++;
				newButon.id = butonId;

				newButon.Cprofiles = &Cprofiles;
				newButon.getMateriaux(materiaux);

				// std::cout << but["Dmax"].get<string>() << std::endl;
				if (but["Dmax"].get<std::string>() != "x") {
					
					for (auto s: stringF::splitStr(but["Dmax"].get<std::string>(), ",")) {
						

						vector<std::string> s0 = stringF::splitStr(s, "-");

						// std::cout << s << "  " << stoi(s0[0]) << " " << newButon.lit << std::endl;

						if (newButon.lit == stoi(s0[0])) {
							// std::cout << "newButon.Dmax = " << newButon.Dmax << std::endl;
							// std::cout << newButon.lit << std::endl;
							// std::cout << but["Dmax"] << std::endl;
							newButon.Dmax = stringF::stof(s0[1]);
							// std::cout << "newButon.Dmax = " << newButon.Dmax << std::endl;
						}
					}
				}
				newButon.dimButons();				

				// newButon.LA = newButon.length - (newButon.startBout.platine.tp/1000.0 + 0.07)/sin(newButon.startBout.angle*PI/180.0) - (newButon.endBout.platine.tp/1000.0 + 0.07)/sin(newButon.endBout.angle*PI/180.0);

				// newButon.GL = newButon.LA + newButon.profile.h/2.0*(1/tan(newButon.startBout.angle*PI/180.0) + 1/tan(newButon.endBout.angle*PI/180.0));

				// newButon.PL = newButon.LA - newButon.profile.h/2.0*(1/tan(newButon.startBout.angle*PI/180.0) + 1/tan(newButon.endBout.angle*PI/180.0));

				// newButon.TG = newButon.profile.h/sin(newButon.startBout.angle*PI/180.0);

				// newButon.TD = newButon.profile.h/sin(newButon.endBout.angle*PI/180.0);

				newButon.semelle.ps = newButon.startBout.F*newButon.startBout.influence/cos(alpha*PI/180.0f)/Bs/Ls;
				newButon.semelle.MEd = 1.35*newButon.semelle.ps*pow(Ls*0.5, 2.0)/2.0;
				newButon.semelle.MEd_reduit = newButon.semelle.MEd - 1.35*newButon.semelle.ps*Bs*Ls/2.0*newButon.profile.h/4.0;
                BAsection::sectionBAprop section;
                section.fcd = 20.0f;
                section.fyd = 500.0/1.15;
                section.b = 1.0f;
                section.d = Hs - 0.1;
                section.Ea = 200.0f;
                section.Eb = 20.0f;
                newButon.semelle.As = BAsection::armELU(newButon.semelle.MEd_reduit/100.0, section);

				// std::cout << "End dim" << std::endl;

				butonList.push_back(newButon);

				// std::cout << "End buton dim" << std::endl;
				
				cout << newButon.label << "  " << newButon.profileNom << endl;

				std::cout << "L = " << newButon.startBout.influence << ", startBoutNiveau = " << newButon.startBout.niveau << ", endBoutNiveau = " << newButon.endBout.niveau << ", d_sol = " << newButon.semelle.d_sol << ", kp = " << newButon.semelle.kp << ", i_beta = " << newButon.semelle.i_beta << ", i_delta = " << newButon.semelle.i_delta << ", qnet = " << newButon.semelle.qnet << ", Rvd = " << newButon.semelle.Rvd << ", taux = " << newButon.semelle.taux << ", Ks = " << newButon.semelle.Ks << ", Kr = " << newButon.startBout.Kr << "  " << newButon.profileNom << ", A= " << newButon.profile.section*100000.0f << " D_max = " << newButon.Dmax << std::endl;

				std::cout << newButon.profile.h << " " << newButon.semelle.MEd << " " << newButon.semelle.MEd_reduit << " " << newButon.semelle.As << std::endl;

			}
			
		}
	}

}

void rido::platine::contraintePlatine(float a, float b) {
	// cout << "contraintePlatine" << endl;
	int maille = 100;
	float pu0 = puEffectif/100.0;	// t/m² => MPa
	float tp0 = tp/1000.0;	// mm => m
	float A, B, C;
	float x0, y0, sa, cosPhi, sinPhi;
	float Ls, L1, L2, h0, d0, section;
	dxf::point point1, point2, point3, point4;
	sectionA.sa = 0.0;

	for (int i=0; i < maille+1; ++i) {
		if (i == 0) {
			// sa = pu*plati.Hp*(plati.Lp*0.5 - a)*(plati.Lp*0.5 - a)/2.0*6.0/(plati.Hp*tp*tp);
			sa = pu0*(LpEffectif*0.5 - a)*(LpEffectif*0.5 - a)*3.0/(tp0*tp0);
			sectionA.angle = 0.0;
			sectionA.sa = sa;
			sectionA.Ls = HpEffectif;
		} else if (i == maille) {
			sa = pu0*(HpEffectif*0.5 - a)*(HpEffectif*0.5 - a)*3.0/(tp0*tp0);
			if (sa > sectionA.sa) {
				sectionA.angle = 90.0;
				sectionA.sa = sa;
				sectionA.Ls = LpEffectif;
			}
		} else {
			cosPhi = cos(i*0.5*PI/maille);
			sinPhi = sin(i*0.5*PI/maille);
			x0 = cosPhi/sqrt(cosPhi*cosPhi/(a*a) + sinPhi*sinPhi/(b*b));
			y0 = sinPhi/sqrt(cosPhi*cosPhi/(a*a) + sinPhi*sinPhi/(b*b));
			// Fonction tangentielle: Ax + By = C
			A = b*b*x0;
			B = a*a*y0;
			C = b*b*x0*x0 + a*a*y0*y0;

			point1.x = 0.5*LpEffectif;
			point2.x = -0.5*LpEffectif;
			point3.y = 0.5*HpEffectif;
			point4.y = -0.5*HpEffectif;

			point1.y = (C - A*point1.x)/B;
			point2.y = (C - A*point2.x)/B;
			point3.x = (C - B*point3.x)/A;
			point4.x = (C - B*point4.x)/A;

			// trapézoidal
			if (point1.x > point4.x) {
				Ls = sqrt(HpEffectif*HpEffectif + pow(point3.x - point4.x ,2));
				L1 = LpEffectif/2 - point3.x;
				L2 = LpEffectif/2 - point4.x;
				h0 = L1/3 + L2*L2/3/(L1 + L2);
				d0 = h0*HpEffectif/Ls;
				section = HpEffectif*(L1/2 + L2/2);
				sa = section*pu0*d0/(Ls*tp0*tp0/4); // Wpl = bh^2/4
				if (sa > sectionA.sa) {
					sectionA.angle = i*90.0/maille;
					sectionA.sa = sa;
					sectionA.Ls = Ls;
				}
			} else if (point2.y < point3.y) {
				Ls = sqrt(HpEffectif*HpEffectif + pow(point2.x - point1.x ,2));
				L1 = LpEffectif/2 - point1.x;
				L2 = LpEffectif/2 - point2.x;
				h0 = L1/3 + L2*L2/3/(L1 + L2);
				d0 = h0*HpEffectif/Ls;
				section = HpEffectif*(L1/2 + L2/2);
				sa = section*pu0*d0/(Ls*tp0*tp0/4); // Wpl = bh^2/4
				if (sa > sectionA.sa) {
					sectionA.angle = i*90.0/maille;
					sectionA.sa = sa;
					sectionA.Ls = Ls;
				}
			// triangle
			} else {
				float y00 = HpEffectif/2.0 - point1.y;
				float x00 = LpEffectif/2.0 - point3.x;
				Ls = sqrt(y00*y00 + x00*x00);
				h0 = x00*y00/Ls;
				d0 = h0/3;
				section = x00*y00/2.0;
				sa = section*pu0*d0/(Ls*tp0*tp0/4); // Wpl = bh^2/4
				if (sa > sectionA.sa) {
					sectionA.angle = i*90.0/maille;
					sectionA.sa = sa;
					sectionA.Ls = Ls;
				}
			}
		}
	}
};

float equation(float a, float b, float c, bool plus) {
	if (b*b < 4*a*c) {
		std::cout << "Equation n'a pas solution" << std::endl;
		return -1000000.0;
	} else {
		if (plus) {
			return (-b + sqrt(b*b-4*a*c))*0.5/a;
		} else {
			return (-b - sqrt(b*b-4*a*c))*0.5/a;
		}
	}
}

void rido::butonMetal::dimPlatine(bout& bout0) {
	platine * p = &bout0.platine;
	p->Hp = profile.h + 0.3;	// Hauteur réelle platine
	p->Lp = profile.h/sin(bout0.angle*PI/180.0) + 0.3; //Largeur réelle platine
	p->HpEffectif = p->Hp;
	p->LpEffectif = p->Lp;
	// cout << "dimPlatine" << endl;
	// vector<float> tpList = {10.0f, 12.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 50.0f, 60.0f};
	vector<float> tpList = {20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 50.0f, 60.0f};
	// float smax = 1000.0; // Contrainte sous platine maximale
	// int i = 0;
	
	// float step = 0.01;
	// platine * p = &bout0.platine;

	p->dt0 = p->dt + 2.0;	// Diamètre trous tige
	p->Hp = profile.h + 0.3;	// Hauteur réelle platine
	p->Lp = profile.h/sin(bout0.angle*PI/180.0) + 0.3; //Largeur réelle platine
	p->pu = bout0.N / p->Hp / p->Lp; //Pression sur platine

	float a = profile.h/2.0/sin(bout0.angle*PI/180.0);
	float b = profile.h/2.0;

	p->HpEffectif = p->Hp;
	p->LpEffectif = p->Lp;
	// p->puEffectif = bout0.N / p->HpEffectif / p->LpEffectif; //Pression sur platine
	// p->tp = 10.0;

	for (int i=0; (p->s1 > profile.fy) & (i < 10); i++) {
		p->tp = tpList[i];
		p->puEffectif = bout0.N / p->HpEffectif / p->LpEffectif; //Pression sur platine

		float DsinAlpha = profile.h/sin(bout0.angle*PI/180.0);
		// std::cout << DsinAlpha << std::endl;
		float LpEffectif = equation(1.0, -(2.0*DsinAlpha + 2.0*profile.fy*p->tp*p->tp*p->HpEffectif/bout0.N*100.0/1000000.0), DsinAlpha*DsinAlpha, true);

		float Wpl = p->tp * p->tp / 4.0;
		
		if ((LpEffectif == -1000000.0) | (LpEffectif < DsinAlpha)) {
			p->s1 = 100000.0;
		} else {
			p->LpEffectif = min(p->Lp, LpEffectif);
			p->puEffectif = bout0.N / p->HpEffectif / p->LpEffectif;

			p->a1 = (p->LpEffectif - DsinAlpha)*0.5;

			p->s1 = p->puEffectif * p->a1 * p->a1 * 0.5/100.0/Wpl*1000000.0;

			// std::cout << p->tp << " " << p->a1 << " " << p->s1 << " " << p->puEffectif << " " << LpEffectif << " " << p->Lp <<  std::endl;
		}

	}

	// for (int i=0; (p->s1 > profile.fy) & (p->sectionA.sa > profile.fy) & (i < 10); i++) {
	// 	// p->sectionA.sa = 1000.0;
	// 	p->tp = tpList[i];
	// 	p->s1 = 2.0*p->pu/100.0*pow(p->a1, 3)/3.0/pow(p->tp/1000.0, 2)/(2*p->a1 - sqrt(2.0)*p->dt0/1000.0);
	// 	// if (p->s1 <= profile.fy) {
	// 	// 	for (int j=0; (p->sectionA.sa > profile.fy) & (j <= (0.3/step)); ++j) {
	// 	// 		p->HpEffectif = p->Hp - j*step;
	// 	// 		for (int k=0; (p->sectionA.sa > profile.fy) & (k <= (0.3/step)); ++k) {
	// 	// 			p->LpEffectif = p->Lp - k*step;
	// 	// 			p->puEffectif = bout0.N / p->HpEffectif / p->LpEffectif; //Pression sur platine
	// 	// 			p->contraintePlatine(a, b);
	// 	// 		}
	// 	// 	}
	// 	// }
	// }
};

void rido::butonMetal::dimAssemblage(bout& bout0) {
	// 	but["assemblage" + cote]["Vfa"] = but["N" + cote]*materiaux["metal"]["beton-beton"]
	// 	but["assemblage" + cote]["Vfb"] = but["N" + cote]*materiaux["metal"]["beton-acier"]
	// 	but["assemblage" + cote]["VA"] = but["T" + cote] - but["assemblage" + cote]["Vfa"]
	// 	but["assemblage" + cote]["VA"] = max(but["assemblage" + cote]["VA"], 0.0)
	// 	but["assemblage" + cote]["V"] = but["FEdx"]*math.cos(math.radians(but["angle" + cote]))
	// 	but["assemblage" + cote]["VB"] = but["assemblage" + cote]["V"] - but["assemblage" + cote]["Vfb"]
	// 	but["assemblage" + cote]["VB"] = max(but["assemblage" + cote]["VB"], 0.0)

	
	bout0.assemblage.VFa = bout0.N*frottementBetonBeton;
	bout0.assemblage.VFb = bout0.N*frottementAcierBeton;
	bout0.assemblage.VA = max(bout0.T - bout0.assemblage.VFa, 0.0f);
	bout0.assemblage.V = FEdx*cos(bout0.angle*PI/180.0);
	bout0.assemblage.VB = max(bout0.assemblage.V - bout0.assemblage.VFb, 0.0f);
	
	// 	k1 = min(2.8*75/but["platine" + cote]["dt0"] - 1.7, 2.5)
	// 	alphad = 75.0/3/but["platine" + cote]["dt0"]
	// 	alphab = min(alphad, 800/510, 1.0)

	float k1 = min(2.8*75/bout0.platine.dt0 - 1.7, 2.5);
	float alphad = 75.0/3/bout0.platine.dt0;
	float alphab = min(alphad, 1.0f);

	// 	but["assemblage" + cote]["tige"]["As"] = math.pi*but["platine" + cote]["dt"]*but["platine" + cote]["dt"]/4.0/1000000.0 # m²
	// 	but["assemblage" + cote]["tige"]["sbadm"] = 3.0*but["fckeff" + cote]["fcd"]["FOND"]
	// 	but["assemblage" + cote]["tige"]["nt"] = max(
	// 		1.25*but["assemblage" + cote]["VA"]/(0.6*800*100*but["assemblage" + cote]["tige"]["As"]), 
	// 		1.25*but["assemblage" + cote]["VA"]/(0.248*800*100*but["assemblage" + cote]["tige"]["As"]),
	// 		1.25*but["assemblage" + cote]["VA"]/(k1*alphab*510*but["platine" + cote]["dt"]*but["platine" + cote]["tp"]/10000),
	// 		2.0*but["assemblage" + cote]["VA"]/but["platine" + cote]["dt"]/(0.4 - but["platine" + cote]["tp"]/1000-0.07)/but["assemblage" + cote]["tige"]["sbadm"], 
	// 		6
	// 	)
	// 	but["assemblage" + cote]["tige"]["nt"] = math.ceil(but["assemblage" + cote]["tige"]["nt"]/2)*2
	// 	but["assemblage" + cote]["tige"]["sb"] = 2.0*but["assemblage" + cote]["VA"]/but["assemblage" + cote]["tige"]["nt"]/but["platine" + cote]["dt"]/(0.4 - but["platine" + cote]["tp"]/1000-0.07)
	// 	but["assemblage" + cote]["tige"]["txt"] = str(but["assemblage" + cote]["tige"]["nt"]) + "$\\phi$" + str(but["platine" + cote]["dt"])
	// 	but["assemblage" + cote]["tige"]["Lt"] = 400.0
	// 	but["assemblage" + cote]["tige"]["espacement"] = (but["platine" + cote]["Lp0"] - 0.15)/(but["assemblage" + cote]["tige"]["nt"]/2 - 1)
	// 	but["assemblage" + cote]["tige"]["espacement"] = math.ceil(but["assemblage" + cote]["tige"]["espacement"]*100)/100

	
	bout0.assemblage.tige.As = PI*bout0.platine.dt*bout0.platine.dt/4.0/1000000.0; // m²
	bout0.assemblage.tige.sbadm = 3.0*fcd;

	bout0.assemblage.tige.n = max(
		bout0.assemblage.tige.n,
		(int) ceil(1.25*bout0.assemblage.VA/(0.6*800*100*bout0.assemblage.tige.As)));

	bout0.assemblage.tige.n = max(
		bout0.assemblage.tige.n, 
		(int) ceil(1.25*bout0.assemblage.VA/(0.248*800*100*bout0.assemblage.tige.As)));

	bout0.assemblage.tige.n = max(
		bout0.assemblage.tige.n,
		(int) ceil(1.25*bout0.assemblage.VA/(k1*alphab*bout0.platine.fu*bout0.platine.dt*bout0.platine.tp/10000.0)));
	
	bout0.assemblage.tige.n = max(
		bout0.assemblage.tige.n,
		(int) ceil(2.0*bout0.assemblage.VA/bout0.platine.dt/(0.4 - bout0.platine.tp/1000-0.07)/bout0.assemblage.tige.sbadm));
	
	// std::cout << bout0.assemblage.tige.n << std::endl;
	bout0.assemblage.tige.n = 2*((int) ceil(((float) bout0.assemblage.tige.n) / 2));

	// std::cout << 1.25*bout0.assemblage.VA/(0.6*800*100*bout0.assemblage.tige.As) << " " << 1.25*bout0.assemblage.VA/(0.248*800*100*bout0.assemblage.tige.As) << " " << 1.25*bout0.assemblage.VA/(k1*alphab*510*bout0.platine.dt*bout0.platine.tp/10000.0) << " " << ceil(2.0*bout0.assemblage.VA/bout0.platine.dt/(0.4 - bout0.platine.tp/1000-0.07)/bout0.assemblage.tige.sbadm) << " " << bout0.assemblage.tige.n << std::endl;
	
	bout0.assemblage.tige.sb = 2.0*bout0.assemblage.VA/bout0.assemblage.tige.n/bout0.platine.dt/(0.4 - bout0.platine.tp/1000-0.07);
	bout0.assemblage.tige.txt = to_string(bout0.assemblage.tige.n) + "$\\phi$" + to_string(bout0.platine.dt);
	bout0.assemblage.tige.espacement = (bout0.platine.Lp - 0.15)/(bout0.assemblage.tige.n/2 - 1);
	bout0.assemblage.tige.espacement = ceil(bout0.assemblage.tige.espacement*100.0)/100.0;
	// std::cout << "dimAssemblage" << std::endl;
	bout0.assemblage.tige.FvbRd = bout0.assemblage.tige.n*min(0.6*800*100*bout0.assemblage.tige.As,0.248*800*100*bout0.assemblage.tige.As)/1.25;
	bout0.assemblage.tige.FbRd = bout0.assemblage.tige.n*k1*alphab*bout0.platine.fu*bout0.platine.dt*bout0.platine.tp/10000.0/1.25;

	// 	but["assemblage" + cote]["mortier"]["d"] = 1000.0*but["platine" + cote]["Lp0"]
	// 	but["assemblage" + cote]["mortier"]["k"] = 1 + pow(200.0/bu["assemblage" + cote]["mortier"]["d"], 0.5)
	// 	# but["assemblage" + cote]["mortier"]["vmin"] = 0.035*po(but["assemblage" + cote]["mortier"]["k"], 1.5)*pow(materiau["metal"]["mortier"]["fck"], 0.5)
	// 	but["assemblage" + cote]["mortier"]["vmin"] = 0.23*po(materiaux["metal"]["mortier"]["fck"], 0.5)
	// 	but["assemblage" + cote]["mortier"]["Ac"] = but["platine" +cote]["Lp0"]*but["platine" + cote]["Hp0"]
	// 	but["assemblage" + cote]["mortier"]["scp"] = but["N" + cote/100.0/but["assemblage" + cote]["mortier"]["Ac"]
	// 	but["assemblage" + cote]["mortier"]["scp"] = min(bu["assemblage" + cote]["mortier"]["scp"], materiaux["metal"["mortier"]["scplim"])
	// 	but["assemblage" + cote]["mortier"]["VRdc"] = (bu["assemblage" + cote]["mortier"]["vmin"] + 0.15*bu["assemblage" + cote]["mortier"]["scp"])*but["platine" + cote["Lp0"]*but["platine" + cote]["Hp0"]*100.0
	// 	# but["assemblage" + cote]["mortier"]["taux"] = "OK"
	// 	# if but["assemblage" + cote]["mortier"]["VRdc"] < but["T"+ cote]:
	// 	# 	but["assemblage" + cote]["mortier"]["taux"] = "Non"
	
	bout0.assemblage.mortier.scplim = 0.2*mortierFck/1.5;
	bout0.assemblage.mortier.fcd = mortierFck/1.5;

	bout0.assemblage.mortier.d = 1000.0*bout0.platine.Lp;
	bout0.assemblage.mortier.k = 1 + pow(200.0/bout0.assemblage.mortier.d, 0.5);
	bout0.assemblage.mortier.vmin = 0.23*sqrt(mortierFck);
	bout0.assemblage.mortier.Ac = bout0.platine.Lp*bout0.platine.Hp;
	bout0.assemblage.mortier.scp = bout0.N/100/bout0.assemblage.mortier.Ac;
	bout0.assemblage.mortier.scp = min(bout0.assemblage.mortier.scp, bout0.assemblage.mortier.scplim);
	bout0.assemblage.mortier.VRdc = (bout0.assemblage.mortier.vmin + 0.15*bout0.assemblage.mortier.scp)*bout0.platine.Lp*bout0.platine.Hp*100.0;

	// 	but["assemblage" + cote]["VBA"] = but["assemblage" + cote]["VB"] - but["assemblage" + cote]["mortier"]["VRdc"]
	// 	but["assemblage" + cote]["VBA"] = max(but["assemblage" +cote]["VBA"], 0.0)
	// 	but["assemblage" + cote]["beche"]["da"] = 0.07
	// 	but["assemblage" + cote]["beche"]["La"] = but["platine" +cote]["Hp0"] - 0.2
	// 	but["assemblage" + cote]["beche"]["MRd1"] = but["fy"]*bu["assemblage" + cote]["beche"]["La"]*0.007*0.007/4.0*100.0
	// 	but["assemblage" + cote]["beche"]["MRd2"] = but["fy"]*bu["assemblage" + cote]["beche"]["La"]*0.016*0.016/4.0*100.0
	// 	but["assemblage" + cote]["beche"]["nbeches"] = max(
	// 		but["assemblage" + cote]["VBA"]*0.007333/bu["assemblage" + cote]["beche"]["MRd1"],
	// 		but["assemblage" + cote]["VBA"]*0.0163333/bu["assemblage" + cote]["beche"]["MRd2"],
	// 		2.0*but["assemblage" + cote]["VB"]/100.0/bu["assemblage" + cote]["beche"]["da"]/but["assemblage" + cote["beche"]["La"]/materiaux["metal"]["mortier"]["fcd"]
	// 	)
		
	// 	but["assemblage" + cote]["beche"]["nbeches"] = math.ceil(bu["assemblage" + cote]["beche"]["nbeches"])
	// 	but["assemblage" + cote]["beche"]["nbeches"] = max(bu["assemblage" + cote]["beche"]["nbeches"], 3, math.ceil(bu["platine" + cote]["Lp0"]/0.3)+1)
	// 	but["assemblage" + cote]["beche"]["sb"] = 2.0*bu["assemblage" + cote]["VB"]/100.0/but["assemblage" + cote["beche"]["da"]/but["assemblage" + cote]["beche"]["La"]/bu["assemblage" + cote]["beche"]["nbeches"]
	// 	but["assemblage" + cote]["beche"]["espacement"] = (bu["platine" + cote]["Lp0"] - 0.2)/(but["assemblage" + cote["beche"]["nbeches"] - 1)

	bout0.assemblage.VBA = max(bout0.assemblage.VB - bout0.assemblage.mortier.VRdc, 0.0f);
	bout0.assemblage.beche.da = 0.07;
	bout0.assemblage.beche.La = bout0.platine.Hp - 0.2;
	bout0.assemblage.beche.MRd1 = profile.fy*bout0.assemblage.beche.La*0.007*0.007/4.0*100.0;
	bout0.assemblage.beche.MRd2 = profile.fy*bout0.assemblage.beche.La*0.016*0.016/4.0*100.0;
	bout0.assemblage.beche.n = max(
		bout0.assemblage.beche.n,
		(int) ceil(bout0.assemblage.VBA*0.007333/bout0.assemblage.beche.MRd1)
	);

	bout0.assemblage.beche.n = max(
		bout0.assemblage.beche.n,
		(int) ceil(bout0.assemblage.VBA*0.0163333/bout0.assemblage.beche.MRd2)
	);

	bout0.assemblage.beche.n = max(
		bout0.assemblage.beche.n,
		(int) ceil(2.0*bout0.assemblage.VB/100.0/bout0.assemblage.beche.da/bout0.assemblage.beche.La/bout0.assemblage.mortier.fcd)
	);

	bout0.assemblage.beche.n = max(
		bout0.assemblage.beche.n,
		(int) ceil(bout0.platine.Lp/0.3 + 1.0)
	);

	bout0.assemblage.beche.sb = 2.0*bout0.assemblage.VB/100.0/bout0.assemblage.beche.da/bout0.assemblage.beche.La/bout0.assemblage.beche.n;
	bout0.assemblage.beche.espacement = (bout0.platine.Lp - 0.2)/(bout0.assemblage.beche.n - 1);

	
	// 	# ======================= Soudures ============================
	// 	r = but["profile"]["h (mm)"]
	// 	R = but["profile"]["h (mm)"]/math.sin(math.radians(but["angle" + cote]))
	// 	but["platine" + cote]["pt"] = math.pi*(3*(R+r) - pow((3*R + r)*(R + 3*r), 0.5))
	// 	but["platine" + cote]["ab"] = pow(3, 0.5)*0.9*1.25*but["T" + cote]*10000.0/510/but["platine" + cote["pt"]
	// 	but["platine" + cote]["ab"] = max(math.ceil(but["platine" + cote]["ab"]), 3)
	// 	but["platine" + cote]["ac"] = pow(31, 0.5)/6.0*0.9*1.25*but["assemblage" + cote]["VB"]*10000.0/510but["assemblage" + cote]["beche"]["La"]/1000.0/but["assemblage" + cote]["beche"]["nbeches"]
	// 	but["platine" + cote]["ac"] = max(math.ceil(but["platine" + cote]["ac"]), 3)

	// 	# ======================= Soudures ============================
	float r = profile.h;
	float R = profile.h/sin(bout0.angle*PI/180.0);
	
	bout0.platine.soudure.pt = PI*(3*(R+r) - sqrt((3*R + r)*(R + 3*r)));

	// std::cout << "bout0.T = " << bout0.T << "r = " << r << " R = " << R << std::endl;

	bout0.platine.soudure.ab = max(
		bout0.platine.soudure.ab,
		(int) ceil(sqrt(3)*bout0.assemblage.beta_w*bout0.assemblage.gamma_M2*bout0.T/100.0*1000.0/bout0.platine.fu/bout0.platine.soudure.pt)
	);
	
	bout0.platine.soudure.ac = max(
		bout0.platine.soudure.ab,
		(int) ceil(sqrt(31)/6.0*bout0.assemblage.beta_w*bout0.assemblage.gamma_M2*bout0.assemblage.VB/100.0*1000.0/(bout0.platine.fu*bout0.assemblage.beche.La*bout0.assemblage.beche.n))
	);

	bout0.platine.soudure.ab = ceil(bout0.platine.soudure.ab);
	bout0.platine.soudure.ac = ceil(bout0.platine.soudure.ac);
	// std::cout << "EndAssemblage" << std::endl;
};

void rido::butonMetal::getCharges() {
	// but["P1"] = but["F1"]*but["influence1"]
	// but["P2"] = but["F2"]*but["influence2"]
	startBout.P = startBout.F*startBout.influence;
	endBout.P = endBout.F*endBout.influence;
	// but["F1_a"] = but["P1"]/math.sin(math.radians(but["angle1"]))
	// but["F2_a"] = but["P2"]/math.sin(math.radians(but["angle2"]))
	startBout.Fa = startBout.P/sin(startBout.angle*PI/180.0);
	endBout.Fa = endBout.P/sin(endBout.angle*PI/180.0);
	// but["Famax"] = max(but["F1_a"], but["F2_a"])
	FaMax = max(startBout.Fa, endBout.Fa);
	// 	but["Ft1"] = dilatationT*but["K1r"]/(1.0 + but["Ksol1"]/but["K1r"])
	// 	but["Ft2"] = dilatationT*but["K1r"]/(1.0 + but["Ksol1"]/but["K1r"])
	startBout.Ft = dilatationT*startBout.Kr/(1.0 + startBout.Ksol/startBout.Kr);
	endBout.Ft = dilatationT*endBout.Kr/(1.0 + endBout.Ksol/endBout.Kr);

	
	// 	but["FEdx"] = 1.35*but["Famax"] + 1.5*max(but["Ft1"], but["Ft2"])
	// 	but["FEdz"] = 1.35*but["poids"]/2.0
	// 	but["MEdy"] = 1.35*but["poids"]*but["length"]/8
	// 	but["NEd"] = but["FEdx"]
	FEdx = 1.35*FaMax + 1.5*max(startBout.Ft, endBout.Ft);
	FEdz = 1.35*poids / 2.0;
	MEdy = 1.35*poids*length/8.0;
	NEd = FEdx;

	// 	but["T1"] = pow(pow(but["FEdx"]*math.cos(math.radians(but["angle1"])), 2) + but["FEdz"]*but["FEdz"], 0.5)
	// 	but["T2"] = pow(pow(but["FEdx"]*math.cos(math.radians(but["angle2"])), 2) + bu["FEdz"]*but["FEdz"], 0.5)
	// 	but["N1"] = but["FEdx"]*math.sin(math.radians(but["angle1"]))
	// 	but["N2"] = but["FEdx"]*math.sin(math.radians(but["angle2"]))
	// 	but["fy"] = materiaux["metal"]["buton"]["fy"]
	startBout.T = sqrt(pow(FEdx*cos(startBout.angle*PI/180.0), 2) + FEdz*FEdz);
	endBout.T = sqrt(pow(FEdx*cos(endBout.angle*PI/180.0), 2) + FEdz*FEdz);
	startBout.N = FEdx*sin(startBout.angle*PI/180.0);
	endBout.N = FEdx*sin(endBout.angle*PI/180.0);

}

bool rido::butonMetal::checkKr() {
	// 	but["K1r"] = but["profile"]["A (cm2)"]/10000.0*21000000.0/but["length"]/but["influence1"] # t/m/m
	// 	but["K2r"] = but["profile"]["A (cm2)"]/10000.0*21000000.0/but["length"]/but["influence2"] # t/m/m
	// startBout.Kr = profile.section*21000000.0/length/startBout.influence;
	// endBout.Kr = profile.section*21000000.0/length/endBout.influence;

	startBout.Kr = profile.section*pow(sin(startBout.angle*PI/180.0), 2)*21000000.0/length/startBout.influence;

	if (semelle.Ks > 0.0) {
		Kt = startBout.Kr*semelle.Ks/startBout.influence/( startBout.Kr + semelle.Ks/startBout.influence );
		endBout.Kr = startBout.Kr;
		return (Kt >= startBout.K);
	} else {
		endBout.Kr = profile.section*pow(sin(endBout.angle*PI/180.0), 2)*21000000.0/length/endBout.influence;
		return (startBout.Kr >= startBout.K) & (endBout.Kr >= endBout.K);
	}

	
}

void rido::butonMetal::getProfile(json& Cprof) {
	// profile.h = Cprofiles[i][0]["h (mm)"].get<float>()/1000.0;		// m
	// profile.section = Cprofiles[i][0]["A (cm2)"].get<float>()/10000.0; // m²
	// profile.Iy = Cprofiles[i][0]["Iy (cm4)"].get<float>()/100000000.0;	// m4
	// profile.tf = Cprofiles[i][0]["tf (mm)"].get<float>()/1000.0;	// m
	// // 	but["poids"] = but["profile"]["G (kg/m)"]*but["length"]/1000.0 # t
	// poids = Cprofiles[i][0]["G (kg/m)"].get<float>() * length/1000.0;
	// profileNom = Cprofiles[i][0]["Profils"].get<string>();

	profile.h = Cprof["h (mm)"].get<float>()/1000.0;		// m
	profile.section = Cprof["A (cm2)"].get<float>()/10000.0; // m²
	profile.Iy = Cprof["Iy (cm4)"].get<float>()/100000000.0;	// m4
	profile.tf = Cprof["tf (mm)"].get<float>()/1000.0;	// m
	
	profile.Iz = Cprof["Iz (cm4)"].get<float>()/100000000.0; // m4
	profile.IT = Cprof["IT (cm4)"].get<float>()/100000000.0; // m4
	profile.Wy = Cprof["Wy (cm3)"].get<float>()/1000000.0; // m3
	profile.iy = Cprof["iy (cm)"].get<float>()/100.0; // m
	// profile.iz = Cprof["iz (cm)"].get<float>()/100.0; // m
	// 	but["poids"] = but["profile"]["G (kg/m)"]*but["length"]/1000.0 # t
	profile.G = Cprof["G (kg/m)"].get<float>(); // m
	poids = Cprof["G (kg/m)"].get<float>() * length/1000.0;
	profileNom = Cprof["Profils"].get<std::string>();

	LA = length - (startBout.platine.tp/1000.0 + 0.07)/sin(startBout.angle*PI/180.0) - (endBout.platine.tp/1000.0 + 0.07)/sin(endBout.angle*PI/180.0);
	GL = LA + profile.h/2.0*(1/tan(startBout.angle*PI/180.0) + 1/tan(endBout.angle*PI/180.0));
	PL = LA - profile.h/2.0*(1/tan(startBout.angle*PI/180.0) + 1/tan(endBout.angle*PI/180.0));
	TG = profile.h/sin(startBout.angle*PI/180.0);
	TD = profile.h/sin(endBout.angle*PI/180.0);
}

void rido::butonMetal::getMateriaux(json materiaux) {
	fck = materiaux["metal"]["mortier"]["fck"].get<float>();
	Dmax = materiaux["metal"]["buton"]["Dmax"].get<float>();
	scplim = 0.2*fck/1.5;
	fcd = fck/1.5;
	dilatationT = materiaux["metal"]["buton"]["dilatation"].get<float>()*materiaux["metal"]["buton"]["dT"].get<float>()*length/2.0;
	profile.Eyoung = materiaux["metal"]["E"].get<float>()*100000.0; // t/m²
	profile.Gyoung = materiaux["metal"]["G"].get<float>()*100000.0; // t/m²
	profile.fy = materiaux["metal"]["buton"]["fy"].get<float>();
	frottementBetonBeton = materiaux["metal"]["beton-beton"].get<float>();
	frottementAcierBeton = materiaux["metal"]["beton-acier"].get<float>();
	mortierFck = materiaux["metal"]["mortier"]["fck"].get<float>();
	if (profile.fy == 235.0) {
		startBout.assemblage.beta_w = 0.8;
		startBout.platine.fu = 360.0;
		endBout.assemblage.beta_w = 0.8;
		endBout.platine.fu = 360.0;
	} if (profile.fy == 355.0) {
		startBout.assemblage.beta_w = 0.9;
		startBout.platine.fu = 510.0;
		endBout.assemblage.beta_w = 0.9;
		endBout.platine.fu = 510.0;
	}
	
	// mortierScplim = materiaux["metal"]["mortier"]["scplim"].get<float>();
}

void rido::butonMetal::dimButons() {

	bool condition = false;
	float C1 = 1.13;
	// std::cout << "Dmax = " << Dmax << std::endl;
	// std::cout << "Cprofiles->size() " << Cprofiles->size() << std::endl;
	for (int i=0; (i < Cprofiles->size()) & (!condition); i++) {
		getProfile(Cprofiles[0][i]);
		getCharges();
		if (profile.h <= Dmax/1000.0) {
			if (checkKr()) {
				profile.getClasse();
				flexionComposee.profile = &profile;
				flexionComposee.length = length;
				if (flexionComposee.checkFlexionComposee(NEd, MEdy)) {
					dimPlatine(startBout);
					dimPlatine(endBout);
					dimAssemblage(startBout);
					dimAssemblage(endBout);
					condition = true;
				}
			}
		}
		// std::cout << "condition: " << condition << std::endl;
	}
}
