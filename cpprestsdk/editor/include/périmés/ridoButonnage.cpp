#include "rido.h"

using namespace std;

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

void rido::paroi::butonnage(string fileButonDispo, json materiaux, json& Cprofiles) {
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
		
		int j = 0;
		for (int lit1: lits1) {
			butonMetal newButon;
			newButon.startBout.panneauId = startBoutPanId;
			newButon.endBout.panneauId = endBoutPanId;
			newButon.lit = lit1;
			newButon.label = "B" + to_string(lit1*100 + i);

			cout << newButon.label << endl;
			// Raideur de sols
			// if (but.contains(but["Ksol1"])) {newButon.startBout.Ksol = but["Ksol1"].get<float>();}
			// if (but.contains(but["Ksol2"])) {newButon.endBout.Ksol = but["Ksol2"].get<float>();}

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

			vectorF::checkSize(coupeList, startBoutPan->coupeId, "startBoutPan->coupeId", "coupeList");
			vectorF::checkSize(coupeList, endBoutPan->coupeId, "endBoutPan->coupeId", "coupeList");

			startBoutCoupe = &coupeList[startBoutPan->coupeId];
			endBoutCoupe = &coupeList[endBoutPan->coupeId];

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
			newButon.startBout.F = startBoutCoupe->appuiList[lit1-1].F;
			newButon.endBout.F = endBoutCoupe->appuiList[lits2[j]-1].F;


			// Raideur
			newButon.startBout.K = startBoutCoupe->appuiList[lit1-1].K;
			newButon.endBout.K = endBoutCoupe->appuiList[lits2[j]-1].K;

			newButon.start.x = startBoutPan->start.x*0.5 + startBoutPan->end.x*0.5  + startBoutPan->vectorUnite.x*but["dis1"].get<float>() + startBoutPan->perpUnite.x*but["face1"].get<float>();

			newButon.start.y = startBoutPan->start.y*0.5 + startBoutPan->end.y*0.5  + startBoutPan->vectorUnite.y*but["dis1"].get<float>() + startBoutPan->perpUnite.y*but["face1"].get<float>();

			newButon.end.x = endBoutPan->start.x*0.5 + endBoutPan->end.x*0.5  + endBoutPan->vectorUnite.x*but["dis1"].get<float>() + endBoutPan->perpUnite.x*but["face1"].get<float>();

			newButon.end.y = endBoutPan->start.y*0.5 + endBoutPan->end.y*0.5  + endBoutPan->vectorUnite.y*but["dis1"].get<float>() + endBoutPan->perpUnite.y*but["face1"].get<float>();

			newButon.getLength();
			newButon.getVectorUnite();
			
			// Angle
			float angle1 = mathF::angle2DVector(startBoutPan->vectorUnite, newButon.vectorUnite)*180.0/PI;
			if (angle1 > 90.0) {angle1 = 180.0 - angle1;}

			float angle2 = mathF::angle2DVector(endBoutPan->vectorUnite, newButon.vectorUnite)*180.0/PI;
			if (angle2 > 90.0) {angle2 = 180.0 - angle2;}

			newButon.startBout.angle = angle1;
			newButon.endBout.angle = angle2;

			j++;
			butonId++;
			newButon.id = butonId;

			newButon.Cprofiles = &Cprofiles;
			newButon.getMateriaux(materiaux);
			newButon.dimButons();

			butonList.push_back(newButon);
		}
	}

	// return butonList;

	// cout << "smt2" << endl;
	// cout << butonList.size() << endl;
	// i = 0;
	// for (auto& but: butonList) {
	// 	i++;
	// 	// if ()
	// 	cout << i << " ";
	// 	but.id = i;
	// }
	// cout << "smt" << endl;
    // return butonList;
}

// def contraintePlatine(Lp, Hp, a, b, pu, tp):
	// import math
	// # print(Lp, Hp, a, b, pu, tp)
	// step = 0.01
	// output = {
	// 	'sa': 0.0
	// }
	// pu /= 100.0		# t/m² => MPa
	// tp /= 1000.0	# mm => m

void contraintePlatine(rido::platine& plati, float a, float b) {
	cout << "contraintePlatine" << endl;
	int maille = 100;
	float pu = plati.pu/100.0;	// t/m² => MPa
	float tp = plati.tp/1000.0;	// mm => m
	float A, B, C;
	float x0;
	float y0;
	float sa;

	dxf::point point1, point2, point3, point4;

	for (int i=0; i < maille+1; ++i) {
		x0 = min(i*step, a);
		y0 = b*pow( 1 - x0*x0/a/a , 0.5);
		// Fonction tangentielle: Ax + By = C
		A = b*b*x0;
		B = a*a*y0;
		C = b*b*x0*x0 + a*a*y0*y0;
	
		// 	pointList = []
		// 	# P1
		// 	x1 = Lp/2.0
		// 	# P4
		// 	x4 = -Lp/2.0
		// 	if B == 0:
		// 		y1 = b
		// 		y4 = b
		// 	else:
		// 		y1 = (C - A*x1)/B
		// 		y4 = (C - A*x4)/B
		// 	# P2
		// 	y2 = Hp/2.0
		// 	# P3
		// 	y3 = -Hp/2.0
		// 	if A == 0:
		// 		x2 = a
		// 		x3 = a
		// 	else:
		// 		x2 = (C - B*y2)/A
		// 		x3 = (C - B*y3)/A

		float x1 = plati.Lp/2.0;
		float x4 = -plati.Lp/2.0;
		if (B == 0) {
			y1 = b;
			y4 = b;
		} else {
			y1 = (C - A*x1)/B;
			y4 = (C - A*x4)/B;
		}

		y2 = plati.Hp/2.0;
		y3 = -plati.Hp/2.0;

		if (A == 0) {
			x2 = a;
			x3 = a;
		} else {
			x2 = (C - B*y2)/A;
			x3 = (C - B*y3)/A;
		}

		// if ((-Hp/2.0) <= y1) & (y1 <= (Hp/2.0)):
		// 	pointList.append(1)
		// if ((-Lp/2.0) <= x3) & (x3 <= (Lp/2.0)):
		// 	pointList.append(3)
		// if ((-Lp/2.0) <= x2) & (x2 <= (Lp/2.0)):
		// 	pointList.append(2)
		// if ((-Hp/2.0) <= y4) & (y4 <= (Hp/2.0)):
		// 	pointList.append(4)
		
		// if (((-plati.Hp/2.0) <= y1) & (y1 <= (plati.Hp/2.0))) {plati.a2.pointList.push_back(1);}
		// if (((-plati.Lp/2.0) <= x3) & (x3 <= (plati.Lp/2.0))) {plati.a2.pointList.push_back(3);}
		// if (((-plati.Lp/2.0) <= x2) & (x2 <= (plati.Lp/2.0))) {plati.a2.pointList.push_back(2);}
		// if (((-plati.Hp/2.0) <= y4) & (y4 <= (plati.Hp/2.0))) {plati.a2.pointList.push_back(4);}

		point1 = ((-plati.Hp/2.0) <= y1) & (y1 <= (plati.Hp/2.0));
		point3 = ((-plati.Lp/2.0) <= x3) & (x3 <= (plati.Lp/2.0));
		point2 = ((-plati.Lp/2.0) <= x2) & (x2 <= (plati.Lp/2.0));
		point4 = ((-plati.Hp/2.0) <= y4) & (y4 <= (plati.Hp/2.0));

		// 	# print(x0, y0, a, b, x0*x0/a/a + y0*y0/b/b, pointList)
		// 	if x0 == a:
		// 		print("smt1", x3, y3)
		// 	if pointList == [3, 2]:
		// 		# pointList0 = [
		// 		# 	{'x': x3, 'y': y3},
		// 		# 	{'x': Lp/2.0, 'y': -Hp/2.0},
		// 		# 	{'x': Lp/2.0, 'y': Hp/2.0},
		// 		# 	{'x': x2, 'y': y2}
		// 		# ]
		// 		Ls = pow(Hp*Hp + pow(x3 - x2 ,2) , 0.5)
		// 		L1 = Lp/2 - x3
		// 		L2 = Lp/2 - x2
		// 		h0 = L1/3 + L2*L2/3/(L1 + L2)
		// 		d0 = h0*Hp/Ls
		// 		section = Hp*(L1/2 + L2/2)
		// 		sa = section*pu*d0/(Ls*tp*tp/4) # Wpl = bh^2/4
		// 		if sa > output['sa']:
		// 			output['sa'] = sa
		// 			output['Ls'] = Ls
		// 			output['pointList'] = pointList
		// 			output['angle'] = round(math.atan((x3-x2)/Lp)*180/math.pi*10.0)/10.0
		// 			output['section'] = section
		// 			output['d0'] = d0
		// 			output['coor'] = [
		// 				{'x': x2, 'y': y2},
		// 				{'x': x3, 'y': y3}
		// 			]
		// if x0 == a:
		// 		print("smt1", x3, y3)
		float Ls, L1, L2, h0, d0, section;
		if (point3 & point2) {
			Ls = sqrt(plati.Hp*plati.Hp + pow(x3 - x2 ,2));
			L1 = plati.Lp/2 - x3;
			L2 = plati.Lp/2 - x2;
			h0 = L1/3 + L2*L2/3/(L1 + L2);
			d0 = h0*plati.Hp/Ls;
			section = plati.Hp*(L1/2 + L2/2);
			sa = section*pu*d0/(Ls*tp*tp/4); // Wpl = bh^2/4
			if (sa > plati.a2.sa) {
				plati.a2.sa = sa;
				plati.a2.Ls = Ls;
				plati.a2.angle = round(atan((x3-x2)/plati.Lp)*180/PI*10.0)/10.0;
				plati.a2.section = section;
				plati.a2.d0 = d0;
				// output['coor'] = [
				// 	{'x': x2, 'y': y2},
				// 	{'x': x3, 'y': y3}
				// ]
			}
				
		}
			
		// 		if x0 == a:
		// 			print("smt", {
		// 				'sa': sa,
		// 				'Ls': Ls,
		// 				'pointList': pointList,
		// 				'angle': 90.0 - round(math.atan((x3-x2)/Lp)*180/math.pi*10.0)/10.0,
		// 				'section': section,
		// 				'd0': d0,
		// 				'coor': [
		// 					{'x': x2, 'y': y2},
		// 					{'x': x3, 'y': y3}
		// 				]
		// 			})
		// 	elif pointList == [1, 2]:
		// 		# pointList0 = [
		// 		# 	{'x': x1, 'y': y1},
		// 		# 	{'x': Lp/2.0, 'y': Hp/2.0},
		// 		# 	{'x': x2, 'y': y2}
		// 		# ]
		// 		y00 = Hp/2.0 - y1
		// 		x00 = Lp/2.0 - x2
		// 		Ls = pow(y00*y00 + x00*x00, 0.5)
		// 		h0 = x00*y00/Ls
		// 		d0 = h0/3
		// 		section = x00*y00/2.0
		// 		sa = section*pu*d0/(Ls*tp*tp/4) # Wpl = bh^2/4
		// 		if sa > output['sa']:
		// 			output['sa'] = sa
		// 			output['Ls'] = Ls
		// 			output['pointList'] = pointList
		// 			output['angle'] = round(math.atan(y00/x00)*180/math.pi*10.0)/10.0
		// 			output['section'] = section
		// 			output['d0'] = d0
		// 			output['coor'] = [
		// 				{'x': x1, 'y': y1},
		// 				{'x': x2, 'y': y2}
		// 			]
		if (point1 & point2) {
			float y00 = plati.Hp/2.0 - y1;
			float x00 = plati.Lp/2.0 - x2;
			Ls = sqrt(y00*y00 + x00*x00);
			h0 = x00*y00/Ls;
			d0 = h0/3;
			section = x00*y00/2.0;
			sa = section*pu*d0/(Ls*tp*tp/4); // Wpl = bh^2/4
			if (sa > plati.a2.sa) {
				plati.a2.sa = sa;
				plati.a2.Ls = Ls;
				plati.a2.angle = round(atan(y00/x00)*180/PI*10.0)/10.0;
				plati.a2.section = section;
				plati.a2.d0 = d0;
				// output['coor'] = [
				// 	{'x': x1, 'y': y1},
		// 			{'x': x2, 'y': y2}
				// ]
			}
		}
		// 	elif pointList == [1, 4]:
		// 		# pointList0 = [
		// 		# 	{'x': x1, 'y': y1},
		// 		# 	{'x': Lp/2.0, 'y': Hp/2.0},
		// 		# 	{'x': -Lp/2.0, 'y': Hp/2.0},
		// 		# 	{'x': x4, 'y': y4}
		// 		# ]
		// 		Ls = pow(Lp*Lp + pow(y4 - y1 ,2) , 0.5)
		// 		L1 = Hp/2 - y4
		// 		L2 = Hp/2 - y1
		// 		h0 = L1/3 + L2*L2/3/(L1 + L2)
		// 		d0 = h0*Lp/Ls
		// 		section = Hp*(L1/2 + L2/2)
		// 		sa = section*pu*d0/(Ls*tp*tp/4) # Wpl = bh^2/4
		// 		if sa > output['sa']:
		// 			output['sa'] = sa
		// 			output['Ls'] = Ls
		// 			output['pointList'] = pointList
		// 			output['angle'] = round(math.atan((y4-y1)/Lp)*180/math.pi*10.0)/10.0
		// 			output['section'] = section
		// 			output['d0'] = d0
		// 			output['coor'] = [
		// 				{'x': x1, 'y': y1},
		// 				{'x': x4, 'y': y4}
		// 			]
		if (point1 & point4) {
			Ls = sqrt(plati.Lp*plati.Lp + pow(y4 - y1 ,2));
			L1 = plati.Hp/2 - y4;
			L2 = plati.Hp/2 - y1;
			h0 = L1/3 + L2*L2/3/(L1 + L2);
			d0 = h0*plati.Lp/Ls;
			section = plati.Hp*(L1/2 + L2/2);
			sa = section*pu*d0/(Ls*tp*tp/4); // Wpl = bh^2/4
			if (sa > plati.a2.sa) {
				plati.a2.sa = sa;
				plati.a2.Ls = Ls;
				plati.a2.angle = round(atan((y4-y1)/plati.Lp)*180/PI*10.0)/10.0;
				plati.a2.section = section;
				plati.a2.d0 = d0;
				// output['coor'] = [
				// 	{'x': x1, 'y': y1},
		// 			{'x': x4, 'y': y4}
				// ]
			}
		}
	}

	
};

void rido::butonMetal::dimPlatine(bout& bout0) {
	// cout << "dimPlatine" << endl;

	// 	smax = 1000.0
	// 	iPlatine = 0
	vector<int> tpList = {10, 12, 15, 20, 25, 30, 35, 40, 50, 1000};
	float smax = 1000.0; // Contrainte sous platine maximale
	int i = 0;
	
	// 	while (smax > but["fy"]) & (iPlatine < len(tpList)):
	// 		but["platine" + cote]["tp"] = tpList[iPlatine]
	// 		but["platine" + cote]["s1"] = 2.0*but["platine" + cote]["pu0"]/100.0*pow(bu["platine" + cote]["a1"], 3)/3.0/pow(but["platine" + cote]["tp"]/1000.0, 2)/(2*but["platine"+ cote]["a1"] - pow(2.0, 0.5)*but["platine" + cote]["dt0"]/1000.0)
	// 		step = 0.01
	// 		j = 0
	float step = 0.01;
	platine * p = &bout0.platine;

	p->dt0 = p->dt + 2.0;	// Diamètre trous tige
	p->Hp = profile.h + 0.3;	// Hauteur réelle platine
	p->Lp = profile.h/sin(bout0.angle*PI/180.0) + 0.3; //Largeur réelle platine
	p->pu = bout0.N / p->Hp / p->Lp; //Pression sur platine

	/*
	for (int i=0; (smax > profile.fy) & (i < 10); i++) {
		p->tp = tpList[i];
		p->s1 = 2.0*p->pu/100.0*pow(p->a1, 3)/3.0/pow(p->tp/1000.0, 2)/(2*p->a1 - sqrt(2.0)*p->dt0/1000.0);
		for (int j=0; (p->s2 > p->s3) & (j <= (0.3/step)) & (p->s1 > profile.fy) & (p->s2 > profile.fy) & (p->s3 > profile.fy); ++j) {
			p->HpEffectif = profile.h/1000.0 + 0.3 - j*step;
			for (int k=0; (p->s1 > profile.fy) & (p->s2 > profile.fy) & (p->s3 > profile.fy) & (k <= (0.3/step)); ++k) {
				p->LpEffectif = profile.h/sin(bout0.angle*PI/180.0) + 0.3 - i*step;
				p->puEffectif = bout0.N / p->HpEffectif / p->LpEffectif;
				// contraintePlatine(but["platine" + cote]["Lp"], but["platine" + cote]["Hp", h_profile/2.0/sin(math.radians(but["angle" + cote])), but["profile"["h (mm)"]/2000.0,but["platine" + cote]["pu"], but["platine" + cote]["tp"])
				
				// contraintePlatine(bout.platine, h_profile/2.0/sin(bout.angle*PI/180.0), h_profile/2.0);
				
				// p->Ls = c["Ls"]
				// p->d0 = c["d0"]
				// p->angle = c["angle"]
				// p->aire = c["section"]
				// p->s2 = c["sa"]
				p->a3 = 0.3 - i*step;
				p->s3 = 2.0*p->pu/100.0*pow(p->a3, 2)/pow(p->tp/1000.0, 2);
			}
		}
	}
	*/

	// 		while (but["platine" + cote]["s2"] > but["platine" + cote]["s3"]) & (j <= (0.3/step))& (max([but["platine" + cote]["s1"], but["platine" + cote]["s2"], but["platine" + cote]["s3"]) > but["fy"]):
	// 			but["platine" + cote]["Hp"] = but["profile"]["h (mm)"]/1000.0 + 0.3 - j*step
	// 			i = 0
	// 			while (max([but["platine" + cote]["s1"], but["platine" + cote]["s2"], bu["platine" + cote]["s3"]]) > but["fy"]) & (i <= (0.3/step)):
	// 				but["platine" + cote]["Lp"] = but["profile"]["h (mm)"]/1000.0/math.sin(mathradians(but["angle" + cote])) + 0.3 - i*step
	// 				but["platine" + cote]["pu"] = but["N" + cote]/but["platine" + cote]["Hp"]/bu["platine" + cote]["Lp"]
	// 				c = contraintePlatine(but["platine" + cote]["Lp"], but["platine" + cote]["Hp", but["profile"]["h (mm)"]/2000.0/math.sin(math.radians(but["angle" + cote])), but["profile"["h (mm)"]/2000.0, but["platine" + cote]["pu"], but["platine" + cote]["tp"])
					
	// 				but["platine" + cote]["Ls"] = c["Ls"]
	// 				but["platine" + cote]["d0"] = c["d0"]
	// 				but["platine" + cote]["angle"] = c["angle"]
	// 				but["platine" + cote]["aire"] = c["section"]
	// 				but["platine" + cote]["s2"] = c["sa"]
	// 				but["platine" + cote]["a3"] = 0.3 - i*step
	// 				but["platine" + cote]["s3"] = 2.0*but["platine" + cote]["pu"]/100.0*pow(bu["platine" + cote]["a3"], 2)/pow(but["platine" + cote]["tp"]/1000.0, 2)
	// 				i += 1
	// 			j += 1
	

	// 		smax = max([but["platine" + cote]["s1"], but["platine" + cote]["s2"], but["platine" +cote]["s3"]])
			
	// 		iPlatine += 1
};

void rido::butonMetal::dimAssemblage(bout& bout0) {

};
// 	but["assemblage" + cote]["Vfa"] = but["N" + cote]*materiaux["metal"]["beton-beton"]
		// 	but["assemblage" + cote]["Vfb"] = but["N" + cote]*materiaux["metal"]["beton-acier"]
		// 	but["assemblage" + cote]["VA"] = but["T" + cote] - but["assemblage" + cote]["Vfa"]
		// 	but["assemblage" + cote]["VA"] = max(but["assemblage" + cote]["VA"], 0.0)
		// 	but["assemblage" + cote]["V"] = but["FEdx"]*math.cos(math.radians(but["angle" + cote]))
		// 	but["assemblage" + cote]["VB"] = but["assemblage" + cote]["V"] - but["assemblage" + cote]["Vfb"]
		// 	but["assemblage" + cote]["VB"] = max(but["assemblage" + cote]["VB"], 0.0)
			
		// 	k1 = min(2.8*75/but["platine" + cote]["dt0"] - 1.7, 2.5)
		// 	alphad = 75.0/3/but["platine" + cote]["dt0"]
		// 	alphab = min(alphad, 800/510, 1.0)
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
		// 	but["assemblage" + cote]["mortier"]["d"] = 1000.0*but["platine" + cote]["Lp0"]
		// 	but["assemblage" + cote]["mortier"]["k"] = 1 + pow(200.0/but["assemblage" + cote]["mortier"]["d"], 0.5)
		// 	# but["assemblage" + cote]["mortier"]["vmin"] = 0.035*pow(but["assemblage" + cote]["mortier"]["k"], 1.5)*pow(materiaux["metal"]["mortier"]["fck"], 0.5)
		// 	but["assemblage" + cote]["mortier"]["vmin"] = 0.23*pow(materiaux["metal"]["mortier"]["fck"], 0.5)
		// 	but["assemblage" + cote]["mortier"]["Ac"] = but["platine" + cote]["Lp0"]*but["platine" + cote]["Hp0"]
		// 	but["assemblage" + cote]["mortier"]["scp"] = but["N" + cote]/100.0/but["assemblage" + cote]["mortier"]["Ac"]
		// 	but["assemblage" + cote]["mortier"]["scp"] = min(but["assemblage" + cote]["mortier"]["scp"], materiaux["metal"]["mortier"]["scplim"])
			
		// 	but["assemblage" + cote]["mortier"]["VRdc"] = (but["assemblage" + cote]["mortier"]["vmin"] + 0.15*but["assemblage" + cote]["mortier"]["scp"])*but["platine" + cote]["Lp0"]*but["platine" + cote]["Hp0"]*100.0
		// 	# but["assemblage" + cote]["mortier"]["taux"] = "OK"
		// 	# if but["assemblage" + cote]["mortier"]["VRdc"] < but["T" + cote]:
		// 	# 	but["assemblage" + cote]["mortier"]["taux"] = "Non"
		// 	but["assemblage" + cote]["VBA"] = but["assemblage" + cote]["VB"] - but["assemblage" + cote]["mortier"]["VRdc"]
		// 	but["assemblage" + cote]["VBA"] = max(but["assemblage" + cote]["VBA"], 0.0)
		// 	but["assemblage" + cote]["beche"]["da"] = 0.07
		// 	but["assemblage" + cote]["beche"]["La"] = but["platine" + cote]["Hp0"] - 0.2
		// 	but["assemblage" + cote]["beche"]["MRd1"] = but["fy"]*but["assemblage" + cote]["beche"]["La"]*0.007*0.007/4.0*100.0
		// 	but["assemblage" + cote]["beche"]["MRd2"] = but["fy"]*but["assemblage" + cote]["beche"]["La"]*0.016*0.016/4.0*100.0
		// 	but["assemblage" + cote]["beche"]["nbeches"] = max(
		// 		but["assemblage" + cote]["VBA"]*0.007333/but["assemblage" + cote]["beche"]["MRd1"],
		// 		but["assemblage" + cote]["VBA"]*0.0163333/but["assemblage" + cote]["beche"]["MRd2"],
		// 		2.0*but["assemblage" + cote]["VB"]/100.0/but["assemblage" + cote]["beche"]["da"]/but["assemblage" + cote]["beche"]["La"]/materiaux["metal"]["mortier"]["fcd"]
		// 	)
			
		// 	but["assemblage" + cote]["beche"]["nbeches"] = math.ceil(but["assemblage" + cote]["beche"]["nbeches"])
		// 	but["assemblage" + cote]["beche"]["nbeches"] = max(but["assemblage" + cote]["beche"]["nbeches"], 3, math.ceil(but["platine" + cote]["Lp0"]/0.3)+1)
		// 	but["assemblage" + cote]["beche"]["sb"] = 2.0*but["assemblage" + cote]["VB"]/100.0/but["assemblage" + cote]["beche"]["da"]/but["assemblage" + cote]["beche"]["La"]/but["assemblage" + cote]["beche"]["nbeches"]
		// 	but["assemblage" + cote]["beche"]["espacement"] = (but["platine" + cote]["Lp0"] - 0.2)/(but["assemblage" + cote]["beche"]["nbeches"] - 1)
		// 	# ======================= Soudures ============================
		// 	r = but["profile"]["h (mm)"]
		// 	R = but["profile"]["h (mm)"]/math.sin(math.radians(but["angle" + cote]))
		// 	but["platine" + cote]["pt"] = math.pi*(3*(R+r) - pow((3*R + r)*(R + 3*r), 0.5))
		// 	but["platine" + cote]["ab"] = pow(3, 0.5)*0.9*1.25*but["T" + cote]*10000.0/510/but["platine" + cote]["pt"]
		// 	but["platine" + cote]["ab"] = max(math.ceil(but["platine" + cote]["ab"]), 3)
		// 	but["platine" + cote]["ac"] = pow(31, 0.5)/6.0*0.9*1.25*but["assemblage" + cote]["VB"]*10000.0/510/but["assemblage" + cote]["beche"]["La"]/1000.0/but["assemblage" + cote]["beche"]["nbeches"]
		// 	but["platine" + cote]["ac"] = max(math.ceil(but["platine" + cote]["ac"]), 3)

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
	startBout.Kr = profile.section*21000000.0/length/startBout.influence;
	endBout.Kr = profile.section*21000000.0/length/endBout.influence;
	return (startBout.Kr >= startBout.K) & (endBout.Kr >= endBout.K);
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
	poids = Cprof["G (kg/m)"].get<float>() * length/1000.0;
	profileNom = Cprof["Profils"].get<string>();
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
}

void rido::butonMetal::dimButons() {

	// cout << "dimButons" << endl;

	getCharges();
	bool condition = false;
	float C1 = 1.13;

	for (int i=0; (i < Cprofiles->size()) & (!condition); i++) {
		getProfile(Cprofiles[0][i]);
		if (profile.h <= Dmax) {
			if (checkKr()) {
				profile.getClasse();
				flexionComposee.profile = &profile;
				flexionComposee.length = length;
				if (flexionComposee.checkFlexionComposee(NEd, MEdy)) {
					dimPlatine(startBout);
					
					condition = true;
				}
			}
		}
		
	}

	cout << "NbRd = " << flexionComposee.flambement.NbRd << endl;
		
		
		
			
		// 	but["platine" + cote]["dt0"] = but["platine" + cote]["dt"] + 2
		// 	but["platine" + cote]["Hp0"] = but["profile"]["h (mm)"]/1000.0 + 0.3
		// 	but["platine" + cote]["Lp0"] = but["profile"]["h (mm)"]/1000.0/math.sin(math.radians(but["angle" + cote])) + 0.3
		// 	but["platine" + cote]["pu0"] = but["N" + cote]/but["platine" + cote]["Hp0"]/but["platine" + cote]["Lp0"]
		/*
		i++;
		
		dimPlatine(but.startBout, profile["h (mm)"].get<float>()/1000.0, fy);
		*/

		// 	smax = 1000.0
		// 	iPlatine = 0
		// 	while (smax > but["fy"]) & (iPlatine < len(tpList)):
		// 		but["platine" + cote]["tp"] = tpList[iPlatine]
		// 		but["platine" + cote]["s1"] = 2.0*but["platine" + cote]["pu0"]/100.0*pow(but["platine" + cote]["a1"], 3)/3.0/pow(but["platine" + cote]["tp"]/1000.0, 2)/(2*but["platine" + cote]["a1"] - pow(2.0, 0.5)*but["platine" + cote]["dt0"]/1000.0)
		// 		step = 0.01
		// 		j = 0
		// 		while (but["platine" + cote]["s2"] > but["platine" + cote]["s3"]) & (j <= (0.3/step)) & (max([but["platine" + cote]["s1"], but["platine" + cote]["s2"], but["platine" + cote]["s3"]]) > but["fy"]):
		// 			but["platine" + cote]["Hp"] = but["profile"]["h (mm)"]/1000.0 + 0.3 - j*step
		// 			i = 0
		// 			while (max([but["platine" + cote]["s1"], but["platine" + cote]["s2"], but["platine" + cote]["s3"]]) > but["fy"]) & (i <= (0.3/step)):
		// 				but["platine" + cote]["Lp"] = but["profile"]["h (mm)"]/1000.0/math.sin(math.radians(but["angle" + cote])) + 0.3 - i*step
		// 				but["platine" + cote]["pu"] = but["N" + cote]/but["platine" + cote]["Hp"]/but["platine" + cote]["Lp"]
		// 				c = contraintePlatine(but["platine" + cote]["Lp"], but["platine" + cote]["Hp"], but["profile"]["h (mm)"]/2000.0/math.sin(math.radians(but["angle" + cote])), but["profile"]["h (mm)"]/2000.0, but["platine" + cote]["pu"], but["platine" + cote]["tp"])
						
		// 				but["platine" + cote]["Ls"] = c["Ls"]
		// 				but["platine" + cote]["d0"] = c["d0"]
		// 				but["platine" + cote]["angle"] = c["angle"]
		// 				but["platine" + cote]["aire"] = c["section"]
		// 				but["platine" + cote]["s2"] = c["sa"]
		// 				but["platine" + cote]["a3"] = 0.3 - i*step
		// 				but["platine" + cote]["s3"] = 2.0*but["platine" + cote]["pu"]/100.0*pow(but["platine" + cote]["a3"], 2)/pow(but["platine" + cote]["tp"]/1000.0, 2)
		// 				i += 1
		// 			j += 1
		

		// 		smax = max([but["platine" + cote]["s1"], but["platine" + cote]["s2"], but["platine" + cote]["s3"]])
				
		// 		iPlatine += 1
		// 	but["assemblage" + cote] = {
		// 		"tige": {},
		// 		"beche": {},
		// 		"mortier": {}
		// 	}
		
		// # !!!!!!!!!!!!!!!! Attention: calculs ne compte de différence niveau !!!!!!!!!!!!!!!!!
		// but["section"] = but["profile"]["Profils"]
		// but["d"] = but["profile"]["h (mm)"]
		// but["t"] = but["profile"]["tw (mm)"]
		// but["Lpg"] = but["platine1"]["Lp0"]*1000.0
		// but["Lpd"] = but["platine2"]["Lp0"]*1000.0
		// but["tpg"] = but["platine1"]["tp"]
		// but["tpd"] = but["platine2"]["tp"]
		// but["LA"] = but["length"]*1000.0 - (but["tpg"] + 70.0)/math.sin(math.radians(but["angle1"])) - (but["tpd"] + 70.0)/math.sin(math.radians(but["angle2"]))
		// but["GL"] = but["LA"] + but["profile"]["h (mm)"]/2.0*(1/math.tan(math.radians(but["angle1"])) + 1/math.tan(math.radians(but["angle2"])))
		// but["PL"] = but["LA"] - but["profile"]["h (mm)"]/2.0*(1/math.tan(math.radians(but["angle1"])) + 1/math.tan(math.radians(but["angle2"])))
		// but["TG"] = but["profile"]["h (mm)"]/math.sin(math.radians(but["angle1"]))
		
	// }
	// cout << "End dimButons" << endl;
}
