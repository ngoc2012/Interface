#include "BAsection.h"

using namespace std;

void BAsection::BAsection::getMaxMin() {
    // std::cout << "getMaxMin " << fa.size() << std::endl;
    // std::cout << " " << fa[0] << " " << fa[1] << "END" << std::endl;
    // std::cout << "END2" << std::endl;
    for (float a: fa) {
        output.faMax = max(output.faMax, a);
        output.faMin = min(output.faMin, a);
        // std::cout << "Bsection 9 : " << a << " " << output.faMin << std::endl;
    }
    for (float b: fb) {
        output.fbMax = max(output.fbMax, b);
    }
    
    for (float b: eb)
        output.ebMax = max(output.ebMax, b);
    
    for (float a: ea)
        output.eaMin = min(output.eaMin, a);
}

vector<BAsection::element> BAsection::maillerSectionBetonCercle(float Rext, float Rint = 0.0, int numberElements = 1000) {
    
    float surfaceElement = PI*(Rext*Rext - Rint*Rint)/numberElements;
    
    float epaisseurAnneau = sqrt(surfaceElement);
    int numberCircles = round((Rext-Rint)/epaisseurAnneau);
    epaisseurAnneau = (Rext-Rint)/numberCircles;

    int numberSlides;

    vector<element> sectioBeton;
    float Rexti, Rinti, Rmoy, phi, Rcdg;
    int j;

    // Maillage
    for (int i=0; i<numberCircles; i++) {
        
        Rexti = Rext - i*epaisseurAnneau;
        Rinti = Rext - (i + 1)*epaisseurAnneau;
        Rmoy = (Rexti + Rinti)/2;
        numberSlides = round(2*PI*Rmoy/epaisseurAnneau);

        surfaceElement = PI*(Rexti*Rexti-Rinti*Rinti)/numberSlides;
        phi = PI/numberSlides;
        Rcdg = (Rexti+Rinti-Rexti*Rinti/(Rexti+Rinti))*2.0*sin(phi)/(3.0*phi*cos(phi/2.0));
        
        for (j=0; j < numberSlides; j++) {
            element newElement;
            newElement.x = Rcdg*cos((j + 0.5)/numberSlides*2.0*PI);
            newElement.y = Rcdg*sin((j + 0.5)/numberSlides*2.0*PI);
            newElement.surface = surfaceElement;

            sectioBeton.push_back(newElement);
        }
    }
    
    return sectioBeton;
}

void BAsection::sectionBetonProp(vector<element> sectionBeton) {
    float inertieX = 0.0, inertieY = 0.0;
    float xcdg, ycdg, sxcdg = 0.0, sycdg = 0.0;
    float surface = 0.0;
    for (auto e: sectionBeton) {
        surface += e.surface; 
        sxcdg += e.surface*e.x;
        sycdg += e.surface*e.y;
    }
    xcdg = sxcdg/surface;
    ycdg = sycdg/surface;
    for (auto e: sectionBeton) {
        inertieY += e.surface*(e.x - xcdg)*(e.x - xcdg);
        inertieX += e.surface*(e.y - ycdg)*(e.y - ycdg);
    }
    cout << "surface: " << surface << " xcdg: " << xcdg << "ycdg: " << ycdg << endl;
    cout << "inertie Y: " << inertieY << " inertie X: " << inertieX << endl;
}

vector<BAsection::element> BAsection::dispoArmaCercle(int nBars, float diaBars, float distanceDuCentre) {

    vector<element> dispoArma;

    float phi;
    for (int i; i < nBars; i++) {
        phi = (0.5 + i)*360.0/nBars;

        element newElement;
        newElement.x = sin(phi*PI/180.0) * distanceDuCentre;
        newElement.y = cos(phi*PI/180.0) * distanceDuCentre;
        newElement.surface = PI*diaBars*diaBars/4.0;

        dispoArma.push_back(newElement);
    }

    // float surface;
    // for (auto e: dispoArma) {
    //     surface += e.surface*10000.0; 
    //     cout << "x: " << e.x << ", y: " << e.y << endl;
    // }
    // cout << "surface totale armatures: " << surface << " cm²" << endl;

    return dispoArma;
}

void BAsection::BAsection2D::verif() {

    // BAsection::output outputResultats;
    // float e0 = 0.0;    // °/oo
	// float ax = 0.0;    // °/oo
	// float ay = 0.0;    // °/oo

	nBoucles = 0;

    // cout << beton.Eyoung[charge.etat] << endl;
    
	Eb = beton.Eyoung[charge.etat];
	fyd = arma.fyd[charge.etat];
	fcd = beton.fcd[charge.etat];
    Ea = arma.Eyoung;
    
	if ((abs(charge.Mx) < 0.001) & (abs(charge.My) < 0.001) & (abs(charge.N) < 0.001)) {
        return;
    }

    // vector<float> ea, eb, dfa, dfb;
    for (auto& b: sectionBeton) {
        fb.push_back(0.0);
        eb.push_back(0.0);
        dfb.push_back(0.0);
    }
    
    for (auto& a: dispoArma) {
        fa.push_back(0.0);
        ea.push_back(0.0);
        dfa.push_back(0.0);
    }

    mathF::matrix J, Ja, Jb;

    vector<float> de = {0.0, 0.0, 0.0};
    vector<float> F = {0.0, 0.0, 0.0};
 
    vector<charge::etatName> ELS = {charge::etatName::QP, charge::etatName::CARA, charge::etatName::FREQ};
    vector<charge::etatName> ELU = {charge::etatName::FOND, charge::etatName::ACC};

    // vector<float> Xa, Ya, Sa;
    for (auto& a: dispoArma) {
        Xa.push_back(a.x);
        Ya.push_back(a.y);
        Sa.push_back(a.surface);
    }

    // vector<float> Xb, Yb, Sb;
    for (auto& b: sectionBeton) {
        Xb.push_back(b.x);
        Yb.push_back(b.y);
        Sb.push_back(b.surface);
    }

    float eue, neta;
    int i;
	while ((dif > tolerence) & (nBoucles < bouclesLimite) & (output.ebMax < ebMaxLimite) & (abs(det) > 0)) {
        
        nBoucles++;
        
        getFa();
        getFb();
        
        Ja = {
            {mathF::sumprod3(Xa, Sa, dfa),  mathF::sumprod4(Xa, Xa, Sa, dfa),   mathF::sumprod4(Xa, Ya, Sa, dfa)},
            {mathF::sumprod3(Ya, Sa, dfa),  mathF::sumprod4(Ya, Xa, Sa, dfa),   mathF::sumprod4(Ya, Ya, Sa, dfa)},
            {mathF::sumprod2(Sa, dfa),      mathF::sumprod3(Xa, Sa, dfa),       mathF::sumprod3(Ya, Sa, dfa)}
        };

        Jb = {  {mathF::sumprod3(Xb, Sb, dfb),  mathF::sumprod4(Xb, Xb, Sb, dfb),   mathF::sumprod4(Xb, Yb, Sb, dfb)},
                {mathF::sumprod3(Yb, Sb, dfb),  mathF::sumprod4(Yb, Xb, Sb, dfb),   mathF::sumprod4(Yb, Yb, Sb, dfb)},
                {mathF::sumprod2(Sb, dfb),      mathF::sumprod3(Xb, Sb, dfb),       mathF::sumprod3(Yb, Sb, dfb)}};
        
        J = {{0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}};
        mathF::sum2matrix(Ja, Jb, J);
        
        F[0] = charge.My - mathF::sumprod3(Xa, Sa, fa) - mathF::sumprod3(Xb, Sb, fb);
        F[1] = charge.Mx - mathF::sumprod3(Ya, Sa, fa) - mathF::sumprod3(Yb, Sb, fb);
        F[2] = charge.N - mathF::sumprod2(Sa, fa) - mathF::sumprod2(Sb, fb);

        det = mathF::getMatrixDeterminant(J);

        // cout << "Ja: ";
        // mathF::showMatrix(Ja);
        // cout << "Jb: ";
        // mathF::showMatrix(Jb);
        // cout << "J: ";
        // mathF::showMatrix(J);
        // cout << "det =" << det << endl;

        // de = {0.0, 0.0, 0.0};
        if (abs(det) > 0.0) {
            mathF::dotProductMatrixColumn( mathF::getInverseMatrix(J, det), F, de);
        }
        
        e0 += de[0];
		ax += de[1];
		ay += de[2];
        for (i=0; i < ea.size(); i++)
		    ea[i] = e0 + ax*Xa[i] + ay*Ya[i];	// °/oo
        
        output.ebMax = 0.0;
        for (i=0; i < eb.size(); i++) {
            eb[i] = e0 + ax*Xb[i] + ay*Yb[i];	// °/oo
            output.ebMax = max(output.ebMax, eb[i]);
        }
		    

		dif = max(abs(F[0]), max(abs(F[1]), abs(F[2])));
        // cout <<  "det =" << det << ",e0 = "<< e0 << ",ax = "<< ax << ",ay = "<< ay << endl;
    }
    // sumMy = charge.My - F[0];
    // sumMx = charge.Mx - F[1];
    // sumN = charge.N - F[2];
    getMaxMin();
    // stringF::printVectorInLine(fa);
}

void BAsection::rectangle::getArmatures() {
    Ya[0] = h/2.0 - dsup;
    Sa[0] = Asup;
    Ya[1] = -(h/2.0-dinf);
    Sa[1] = Ainf;
}

void BAsection::rectangle::mailleSection() {
    // vector<float> Ya, Sa;
    
    // fa.push_back(0.0);
    // ea.push_back(0.0);
    // dfa.push_back(0.0);
    // fa.push_back(0.0);
    // ea.push_back(0.0);
    // dfa.push_back(0.0);

    fa = {0.0, 0.0};
    ea = {0.0, 0.0};
    dfa = {0.0, 0.0};

    // Xa.push_back(a.x);
    Ya.push_back(0.0);
    Sa.push_back(0.0);
    Ya.push_back(0.0);
    Sa.push_back(0.0);

    // vector<float> Yb, Sb;
    // maille = 10;
    for (int i=0; i<maille; i++) {
        // std::cout << i << endl;
        fb.push_back(0.0);
        eb.push_back(0.0);
        dfb.push_back(0.0);
        // Xb.push_back(b.x);
        Yb.push_back((0.5+i)*h/maille - h/2.0);
        Sb.push_back(b*h/maille);
    }

    // std::cout << "maille: " << maille << endl;;

    // std::cout << "Ya: ";
    // for (auto y: Ya) {std::cout << y << " ";}
    // std::cout << endl << "Sa: ";
    // for (auto y: Sa) {std::cout << y << " ";}
    // std::cout << endl;

    float Sbeton = 0.0;
    float Ibeton = 0.0;
    int i = 0;
    for (auto sbi: Sb) {
        Sbeton += sbi;
        Ibeton += sbi*Yb[i]*Yb[i];
        i++;
    }
    // std::cout << "fa.size(): " << fa.size() << endl;
    // std::cout << "Sb.size(): " << Sb.size() << endl;
    // std::cout << "b : " << b << ", h : " << h << endl;
    // std::cout << "Sbeton : " << Sbeton << ", Ibeton : " << Ibeton << endl;

}

void BAsection::rectangle::checkCharges() {

    // float sumMx=0.0, sumN=0.0;
    // cout << "Mx = " << charge.Mx << "MN, My " << charge.My << "MN, N = " << charge.N << "MN" << endl;
    // vector<float> Ya, Sa;

    // fa.push_back(0.0);
    // ea.push_back(0.0);
    // dfa.push_back(0.0);
    // fa.push_back(0.0);
    // ea.push_back(0.0);
    // dfa.push_back(0.0);
    // Xa.push_back(a.x);
    // Ya.push_back(h/2.0 - dsup);
    // Sa.push_back(Asup);
    // Ya.push_back(-(h/2.0-dinf));
    // Sa.push_back(Ainf);
    // vector<float> Yb, Sb;
    // maille = 10;
    // for (int i=0; i<maille; i++) {
    //     // std::cout << i << endl;
    //     // fb.push_back(0.0);
    //     // eb.push_back(0.0);
    //     // dfb.push_back(0.0);
    //     // Xb.push_back(b.x);
    //     Yb.push_back((0.5+i)*h/maille - h/2.0);
    //     Sb.push_back(b*h/maille);
    // }
    float sumMx = mathF::sumprod3(Ya, Sa, fa) + mathF::sumprod3(Yb, Sb, fb);
    float sumN = mathF::sumprod2(Sa, fa) + mathF::sumprod2(Sb, fb);

    std::cout << "b = " << b << ", h = " << h;
    std::cout << ", Y[0] = " << Ya[0] << ", Y[1] = " << Ya[1];
    std::cout << ", Sa[0] = " << Sa[0] << ", Sa[1] = " << Sa[1];
    std::cout << ", fa[0] = " << fa[0] << ", fa[1] = " << fa[1];
    std::cout << ", fbMax = " << output.fbMax;
    std::cout << ", faMin = " << output.faMin;
    std::cout << ", Mx = " << charge.Mx << ", N = " << charge.N << ", sumMx = " << sumMx  << ", sumN = " << sumN << std::endl;

    cout <<  "det =" << det << ",e0 = "<< e0 << ",ay = "<< ay << endl;
    cout << "dif: " << dif << ", tolerence: " << tolerence << ", nBoucles " << nBoucles << ", bouclesLimite " << bouclesLimite << ",  ebMax: " << output.ebMax << ", ebMaxLimite : " << ebMaxLimite << endl;
};

void BAsection::rectangle::checkSection() {
    // for (auto y: Ya) {std::cout << y << " " << endl;}
    // Ya.push_back(h/2.0 - dsup);
    // Sa.push_back(Asup);

    // Yb.push_back((0.5+i)*h/maille - h/maille);
    // Sb.push_back(b*h/maille);

    float inertieX = 0.0, inertieY = 0.0;
    float xcdg, ycdg, sxcdg = 0.0, sycdg = 0.0;
    float surface = 0.0;
    int i = 0;
    for (auto s: Sb) {
        surface += s;
        // sxcdg += s*Xb[i];
        sycdg += s*Yb[i];
        i++;
    }
    xcdg = sxcdg/surface;
    ycdg = sycdg/surface;

    i = 0;
    for (auto s: Sb) {
        // inertieY += s*(Xb[i] - xcdg)*(Xb[i] - xcdg);
        inertieX += s*(Yb[i] - ycdg)*(Yb[i] - ycdg);
        i++;
    }
    cout << "surface: " << surface << " xcdg: " << xcdg << "ycdg: " << ycdg << endl;
    cout << "inertie Y: " << inertieY << " inertie X: " << inertieX << endl;

}

void BAsection::BAsection::reset() {
    // for (auto y: Ya) {std::cout << y << " " << endl;}
    // Ya.push_back(h/2.0 - dsup);
    // Sa.push_back(Asup);

    // Yb.push_back((0.5+i)*h/maille - h/maille);
    // Sb.push_back(b*h/maille);
    output.faMax = 0.0;
    output.faMin = 0.0;
    output.fbMax = 0.0;
    output.ebMax = 0.0;
    output.eaMin = 0.0;

    for (auto a: fa)
        a = 0.0;
    for (auto a: ea)
        a = 0.0;
    for (auto a: dfa)
        a = 0.0;
    
    for (auto a: fb)
        a = 0.0;
    for (auto a: eb)
        a = 0.0;
    for (auto a: dfb)
        a = 0.0;
    
    e0 = 0.0;    // °/oo
    ax = 0.0;    // °/oo
    ay = 0.0;    // °/oo

    dif = 100.0;
    det = 100.0;
    nBoucles = 0;

}

const vector<charge::etatName> ELS = {charge::etatName::QP, charge::etatName::CARA, charge::etatName::FREQ, charge::etatName::EB, charge::etatName::EH, charge::etatName::EE};

const vector<charge::etatName> ELU = {charge::etatName::FOND, charge::etatName::ACC};

void BAsection::BAsection::getFa() {

    for (int i=0; i < ea.size(); i++) {
        if (core::isIn(charge.etat, ELS)) {
            fa[i] = Ea*ea[i]; // MPa = GPa * %o
        }
        else if (core::isIn(charge.etat, ELU)) {
            float eue = fyd/Ea;	// %o	
			if (arma.palier == arma::palierName::horizontal) {
                if (abs(ea[i]) < eue) {
                    dfa[i] = Ea;
                } else {
                    dfa[i] = 0;
                }
					
				fa[i] = mathF::sign(ea[i])*min(Ea*abs(ea[i]), fyd);  // MN
            }
			else if (arma.palier == arma::palierName::incline) {
                if (abs(ea[i]) < eue) {
                    fa[i] = mathF::sign(ea[i])*min(Ea*abs(ea[i]), fyd);	//MPa
					dfa[i] = Ea;
                }
				else if (abs(ea[i]) > arma.eud) {
                    fa[i] = 0.0;
					dfa[i] = 0.0;
                }
				else {
                    fa[i] = mathF::sign(ea[i])*(fyd+(arma.k-1)*fyd*(abs(ea[i])-eue)/(arma.euk-eue));
					dfa[i] = (arma.k-1)*fyd/(arma.euk - eue);
                }
					
            }	
        }
    }
}

void BAsection::BAsection::getFb() {

    if (core::isIn(charge.etat, ELS)) {
        for (int i=0; i < eb.size(); i++) {
            dfb[i] = Eb;	// MPa
            fb[i] = 0.0;
            if (eb[i] > 0) {
                fb[i] = eb[i]*Eb; // MPa = GPa * %o
            }
        }
    }
    else if (beton.diagramme == beton::diagrammeName::paraboleRectangle) {
        for (int i=0; i < eb.size(); i++) {
            if ((eb[i] < 0) | (eb[i] > beton.ecu2)) {
                dfb[i] = 0.0;	// MPa
				fb[i] = 0.0;	// MPa
            }
			else if (eb[i] > beton.ec2) {
                dfb[i] = 0.0;	// MPa
				fb[i] = fcd;    // MPa
            }	
			else {
                dfb[i] = pow(1-eb[i]/beton.ec2, (beton.n - 1.0))*beton.n*fcd/beton.ec2;	// MPa
				fb[i] = (1-pow(1-eb[i]/beton.ec2, beton.n))*fcd;	// MPa
            }
        }
    }
    else if (beton.diagramme == beton::diagrammeName::analyseStructurale) {
        for (int i=0; i < eb.size(); i++) {
            if ((eb[i] < 0) | (eb[i] > beton.ecu1)) {
                dfb[i] = 0.0;	// MPa
				fb[i] = 0.0;	// MPa
            }
			else {
                float neta = eb[i]/beton.ec1;
				dfb[i] = (-(beton.k - 2.0)*neta*neta - 2.0*neta + beton.k)/pow(1.0 +(beton.k - 2.0)*neta, 2.0)*beton.fcm/beton.ec1; // MPa
				fb[i] = (beton.k*neta - neta*neta)/(1.0 + (beton.k - 2.0)*neta)*beton.fcm;     // MPa
            }
        }
    }
}

void BAsection::rectangle::verif() {

	nBoucles = 0;

    // cout << beton.Eyoung[charge.etat] << endl;
    
	Eb = beton.Eyoung[charge.etat]/1000.0;  // GPa
	fyd = arma.fyd[charge.etat];    // MPa
	fcd = beton.fcd[charge.etat];   // MPa
    Ea = arma.Eyoung/1000.0;   // GPa
    
	if ((abs(charge.Mx) < 0.001) & (abs(charge.N) < 0.001)) {
        return;
    }

    mathF::matrix J, Ja, Jb;

    vector<float> de = {0.0, 0.0};
    vector<float> F = {0.0, 0.0};
 
    reset();

    det = 100.0;
    // std::cout << "abs(det): " << abs(det) << ", ebMaxLimite: " << ebMaxLimite << endl;

    // std::cout << "BAsection 579: Mx =" << charge.Mx << ", N =" << charge.N << ", Ya[0] =" << Ya[0] << ", Sa[0] =" << Sa[0] << ", Ya[1] =" << Ya[1] << ", Sa[1] =" << Sa[1] << std::endl;
    // std::cout << "BAsection dif = " << dif << std::endl;
    int i;
	while ((dif > tolerence) & (nBoucles < bouclesLimite) & (output.ebMax < ebMaxLimite) & (abs(det) > 0)) {
        
        nBoucles++;
		// Acier
		
		getFa();
        getFb();
        
        Ja = {
            {mathF::sumprod3(Ya, Sa, dfa),   mathF::sumprod4(Ya, Ya, Sa, dfa)},
            {mathF::sumprod2(Sa, dfa),       mathF::sumprod3(Ya, Sa, dfa)}
        };

        Jb = {
            {mathF::sumprod3(Yb, Sb, dfb), mathF::sumprod4(Yb, Yb, Sb, dfb)},
            {mathF::sumprod2(Sb, dfb),     mathF::sumprod3(Yb, Sb, dfb)}
        };
        
        J = {{0.0,0.0}, {0.0,0.0}};
        mathF::sum2matrix(Ja, Jb, J);
        
        F[0] = charge.Mx - mathF::sumprod3(Ya, Sa, fa) - mathF::sumprod3(Yb, Sb, fb);
        F[1] = charge.N - mathF::sumprod2(Sa, fa) - mathF::sumprod2(Sb, fb);

        det = mathF::getMatrixDeterminant(J);

        // cout << "Ja: ";
        // mathF::showMatrix(Ja);
        // cout << "Jb: ";
        // mathF::showMatrix(Jb);
        // cout << "J: ";
        // mathF::showMatrix(J);
        // cout << "det =" << det << endl;

        // cout << "inv: ";
        // mathF::showMatrix(mathF::getInverseMatrix(J, det));

        // de = {0.0, 0.0, 0.0};
        if (abs(det) > 0.0) {
            mathF::dotProductMatrixColumn( mathF::getInverseMatrix(J, det), F, de);
        }
        
        e0 += de[0];
		ay += de[1];
        for (i=0; i < ea.size(); i++)
		    ea[i] = e0 + ay*Ya[i];	// °/oo
        
        output.ebMax = 0.0;
        for (i=0; i < eb.size(); i++) {
            eb[i] = e0 + ay*Yb[i];	// °/oo
            output.ebMax = max(output.ebMax, eb[i]);
        }
		    

		dif = max(abs(F[0]), abs(F[1]));
        // std::cout << "arma.eud " << arma.eud << ", Ea = " << Ea << ", fyd = " << fyd << std::endl;
        // std::cout << "fa: ";
        // stringF::printVectorInLine(fa);
        // std::cout << std::endl;
        // std::cout << "Sa: ";
        // stringF::printVectorInLine(Sa);
        // std::cout << std::endl;
        // std::cout << "Ya: ";
        // stringF::printVectorInLine(Ya);
        // std::cout << "ea: ";
        // stringF::printVectorInLine(ea);
        // std::cout << std::endl;

        // cout <<  "det =" << det << ",e0 = "<< e0 << ",ay = "<< ay << endl;
        // cout << "dif: " << dif << ", tolerence: " << tolerence << ", nBoucles " << nBoucles << ", bouclesLimite " << bouclesLimite << ",  ebMax: " << output.ebMax << ", ebMaxLimite : " << ebMaxLimite << endl;
    }
    
    // std::cout << "BAsection 653: " << F[0] << " " << F[1] << std::endl;

    // std::cout << "BAsection 535: " << mathF::sumprod3(Ya, Sa, fa) + mathF::sumprod3(Yb, Sb, fb) << " " << mathF::sumprod2(Sa, fa) + mathF::sumprod2(Sb, fb) << std::endl;

    // sumMx = charge.Mx - F[0];
    // sumN = charge.N - F[1];
    // std::cout << "BAsection 538: Mx = " << mathF::sumprod3(Ya, Sa, fa) + mathF::sumprod3(Yb, Sb, fb) << ", N = " << mathF::sumprod2(Sa, fa) + mathF::sumprod2(Sb, fb) << ", ebMax = " << output.ebMax << std::endl;

    // std::cout << "BAsection 636: " << fa.size() << std::endl;
    getMaxMin();
    // std::cout << "BAsection 638: " << fa.size() << std::endl;
    // std::cout << "BAsection 639: " << fa[0] << " " << fa[1] << " " << output.faMin << std::endl;

    // for (float a: fa) {
    //     output.faMax = max(output.faMax, a);
    //     output.faMin = min(output.faMin, a);
    //     // std::cout << "Bsection 9 : " << a << " " << output.faMin << std::endl;
    // }
    // for (float b: fb) {
    //     output.fbMax = max(output.fbMax, b);
    // }
    
    // for (float b: eb)
    //     output.ebMax = max(output.ebMax, b);
    
    // for (float a: ea)
    //     output.eaMin = min(output.eaMin, a);

    // stringF::printVectorInLine(fa);
}

float BAsection::armELS(float Mser, sectionBAprop section) {
    // ==================================================
    // Vérification besoin d'armatures comprimées
    float EaEb = section.Ea/section.Eb;
    float d = section.d;
    float sbc = section.fcd;
    float alpha1_lim = section.fcd/(section.fcd + section.fyd/EaEb);
    float Mrb = alpha1_lim/2*(1-alpha1_lim/3)*section.b*d*d*section.fcd;   // MNm
    // print("alpha1_lim = " + str(alpha1_lim))
    // print("Mrb = " + str(Mrb))
    float Ap = 0.0;
    float A = 0.0;
    float z;
    if (Mser > Mrb) {
        
        // print("Besoin d'armatures comprimés :")
        float dp = section.h - section.d;
        float delta = dp/section.d;
        // print(d, dp)
        // print delta
        float sac = 0.0;
        float proportion = 0.9;
        float sa = section.fyd + 0.0;
        if (alpha1_lim < delta) {
            // Acier comprimé dans la zone tendue => réduire sa, au moins delta = proportion*alpha1_lim
            while (Ap >= A) {
                proportion -= 0.01;
                sa = EaEb*sbc*(d-dp/proportion)*proportion/dp;
                alpha1_lim = sbc/(sbc+sa/EaEb);
                sac = EaEb*sbc*(alpha1_lim-delta)/alpha1_lim;
                Ap = (Mser-Mrb)/sac/(d-dp);    // cm²
                A = Mrb/d/(1-alpha1_lim/3)/sa + Ap*sac/sa;
            }
        }
            
        sac = EaEb*sbc*(alpha1_lim-delta)/alpha1_lim;
        Ap = (Mser-Mrb)/sac/(d-dp);
        A = Mrb/d/(1-alpha1_lim/3)/sa + Ap*sac/sa;

        std::cout << "Error: Mser = " << Mser << " > Mrb = " << Mrb << ", d=" << section.d << ", A=" << A*10000.0 << std::endl;

        return A*10000.0; // cm²
        
        
        // return 1000000.0;
    } else {
        if (section.method == method::simplified) {
            z = d*(1.0 - alpha1_lim/3.0);
            A = Mser/z/section.fyd;
        } else {
            // ==================================================
            // Pivot A: sb <= sbc
            // Méthode Newton - Raphson
            float muyser = Mser/(section.b*d*d*section.fyd);
            float alpha1 = 1.5 * (1 - pow(1.0 - 2.0 * muyser, 0.5));
            float alpha2 = 1.0;
            int n = 0;
            while ((abs(alpha1 - alpha2) > 0.0001) & (n < 100)) {
                n = n + 1;
                alpha2 = alpha1;
                alpha1 = alpha2 - (alpha1*alpha1*(1.0 - alpha1/3.0)/30.0/(1.0 - alpha1) - muyser)/(alpha1*(alpha1*alpha1 - 3.0*alpha1 + 3.0)/45.0/(1.0 - alpha1)/(1.0 - alpha1));
            }
            float sb = alpha1/(1-alpha1)*section.fyd/EaEb;
            if (sb <= sbc) {
                // print("Armatures ELS pivot A :")
                alpha1 = alpha1_lim;
            }
            // ==================================================
            // Pivot B: sb > sbc
            else {
                // print("Armatures ELS pivot B :")
                alpha1 = 1.5 - 3*pow(0.25 - 4.0*muyser/6, 0.5);
            }
                
            z = d*(1.0 - alpha1/3.0);
            A = Mser/z/section.fyd;
        }
    }

    // cout << "Mser = " << Mser << "MN, z = " << z << ", fyd = " << section.fyd << "MPa, A = " << A*10000.0 << "cm2" << endl;
    return A*10000.0;    // cm² 
};
    
float BAsection::armELU(float MEd, sectionBAprop section) {
    float muybu = MEd/(section.b*section.d*section.d*section.fcd);
    float EaEb = section.Ea/section.Eb;
    // float alpha1_lim = section.fcd/(section.fcd + section.fyd/EaEb);
    // float Mrb = alpha1_lim/2*(1-alpha1_lim/3)*(section.b*section.d*section.d*section.fcd);   // MNm
    
    if (muybu > 0.253) {
        // print("Besoin d'armatures comprimés :")
        std::cout << "Error: muybu = " << muybu << ", MEd = " << MEd << ", d=" << section.d << std::endl;
        return 1000000.0;
    }
    else {
        float alpha = 1.25*(1.0 - pow(1.0 - 2.0*muybu, 0.5));
        float z = section.d*(1.0 - 0.4*alpha);
        return MEd/z/section.fyd*10000.0;
    }
}

float BAsection::armELStraction(float Mser, float Nser, sectionBAprop section) {
    // ==================================================
    // Vérification besoin d'armatures comprimées
    float d = section.d;
    float h = section.h;
   
    if (Nser == 0) {
        // std::cout << "Nser=0" << std::endl;
        return armELS(Mser, section);
    } else {
        // Section totalement tendue
        if (abs(Mser/Nser) <= (d - h*0.5)) {
            // std::cout << "Section totalement tendue: " << ", Nser=" << Nser << " , sans N: " << armELS(Mser, section) << ", avec N:" << (Mser/(2*d - h) - Nser)/section.fyd*10000.0 << std::endl;
            return (Mser/(2*d - h) - Nser)/section.fyd*10000.0;
        }
        // Section partiellement tendue
        else {
            // std::cout << "Section partiellement tendue: " << ", Nser=" << Nser << " , sans N: " << armELS(Mser, section) << ", avec N: " << armELS(Mser + Nser*(d -0.5*h), section) - Nser/section.fyd*10000.0 << std::endl;
            return armELS(Mser + Nser*(d -0.5*h), section) - Nser/section.fyd*10000.0;
        }
    }

    return 0.0; 
};
    
float BAsection::armELUtraction(float MEd, float NEd, sectionBAprop section) {
    float d = section.d;
    float h = section.h;
   
    if (NEd == 0) {
        // std::cout << "NEd=0" << std::endl;
        return armELS(MEd, section);
    } else {
        // Section totalement tendue
        if (abs(MEd/NEd) <= (d - h*0.5)) {
            // std::cout << "Section totalement tendue: " << ", NEd=" << NEd << " , sans N: " << armELS(MEd, section) << ", avec N: " << (MEd/(2*d - h) - NEd)/section.fyd*10000.0 << std::endl;
            return (MEd/(2*d - h) - NEd)/section.fyd*10000.0;
        }
        // Section partiellement tendue
        else {
            // std::cout << "Section partiellement tendue: " << ", NEd=" << NEd << " , sans N: " << armELS(MEd, section) << ", avec N: " << armELS(MEd + NEd*(d -0.5*h), section) - NEd/section.fyd*10000.0 << std::endl;
            return armELS(MEd + NEd*(d -0.5*h), section) - NEd/section.fyd*10000.0;
        }
    }

    return 0.0; 
}


std::tuple<float, float> BAsection::armELUAp(float MEd, sectionBAprop section) {
    float muybu = MEd/(section.b*section.d*section.d*section.fcd);
    float EaEb = section.Ea/section.Eb;
    // float alpha1_lim = section.fcd/(section.fcd + section.fyd/EaEb);
    // float Mrb = alpha1_lim/2*(1-alpha1_lim/3)*(section.b*section.d*section.d*section.fcd);   // MNm
    float es;

    // std::cout << "BAsection: h=" << section.h << ", b=" << section.b << ", d=" << section.d << ", fcd=" << section.fcd << ", MEd=" << MEd << ", muybu=" << muybu << std::endl;
    // if (muybu <= 0.0567) {
    //     // Fonction approximative        
    //     return std::make_tuple(0.0, (1.035*muybu - 0.0005)*section.b*section.d*section.fcd*10000.0/471.0);
    // }
    // else {
        if (muybu <= 0.371) {
            float alpha = 1.20*(1.0 - pow(1.0 - 2.06*muybu, 0.5));
            es = 0.0035*(1 - alpha)/alpha;
            float z = section.d*(1.0 - 0.416*alpha);
            // std::cout << "BAsection::armELUAp1 " << 0.0 << " " << MEd/(z*section.fyd)*10000.0 << std::endl;
            return std::make_tuple(0.0, MEd/(z*section.fyd)*10000.0);
        } else {
            float alpha = 0.617;
            float dp = section.h - section.d;
            es = 0.0035*(alpha - dp/section.d)/alpha;
            float Mlim = 0.371*section.b*section.d*section.d*section.fcd;
            float Asc = (MEd - Mlim)/(section.fyd*(section.d - dp));
            float z = 0.743*section.d;
            float A = MEd/(z*section.fyd) + Asc;

            // std::cout << "BAsection::armELUAp2 " << Asc*10000.0 << " " << A*10000.0 << std::endl;
            return std::make_tuple(Asc*10000.0, A*10000.0);
        }
    // }

    return std::make_tuple(0.0, 0.0);
}

// std::tuple<double, char, std::string> get_student(int id)
// {
//     if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
//     if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
//     if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
//     throw std::invalid_argument("id");
// }

float BAsection::fiss(sectionBAprop section, BAsection2D sectionBAoutput, charge::charge charge) {
    /*
    =============== Espacement maximal des fissures =============================
    Lorsque les armatures adhérentes sont disposées dans la zone tendue avec un entraxe suffisamment faible (espacement<=5(c+phi/2)), l'espacement final maximal des fissures peut être calculé au moyen de l'Expression (7.11)  (voir Figure 7.2)
    */
    float phi1 = 0.0;
    float phi2 = 0.0;
    vector<arma0> armaDispo;
    for (auto a: armaDispo) {
        phi1 += a.n*a.HA*a.HA;
        phi2 += a.n*a.HA;
    }
    float phiMoyen = phi1/phi2;
    float k1 = 0.8;
    /*
    k1 est un coefficient qui tient compte des propriétés d'adhérence des armatures adhérentes :
        = 0,8 pour les barres à haute adhérence
        = 1,6 pour les armatures ayant une surface effectivement lisse (armatures de précontrainte, par exemple)
    */
    float k2 = 0.5;    // flexion
    if (charge.N < 0.0) {
        float k2 = (sectionBAoutput.output.ebMax + sectionBAoutput.output.eaMin)/(2*sectionBAoutput.output.eaMin);
    }
    
    /*
    k2 est un coefficient qui tient compte de la distribution des déformations :
        = 0,5 en flexion
        = 1,0 en traction pure.
    Dans le cas d'une traction excentrée ou pour certaines zones localisées, il convient d'utiliser des valeurs intermédiaires de k2 que l'on peut déterminer de la manière suivante :
    k2 = (e1 + e2)/(2*e1)                       (7.13)
    où e1 est le plus grand et e2 le plus petit allongement relatif en fibre extrême, la section étant supposée fissurée
    k3 à utiliser est celle recommandée pour des enrobages inférieurs ou égaux à 25 mm. Pour des enrobages plus grands, la valeur de k3 à utiliser est  k3 = 3,4 (25 / c)2/3 (c en mm).
    */
    float k3 = 3.4;
    if (section.c > 25) {
        3.4*pow(25.0/section.c, 0.66667);
    }
    
    float k4 = 0.425;
    float a_e = section.Ea/section.Ecm;
    
    // float sr_max = k3*section.c + k1*k2*k4*phiMoyen/ro_pEff     // NF EN 1992-1-1 §7.3.4 (3)
    // ro_pEff = (As + xi1*Ap_p)/A_cEff
    /*
    Cas non traité:
    Lorsque l'espacement des armatures adhérentes excède 5(c + phi/2) (voir Figure 7.2) ou lorsqu'il n'y a pas d'armatures adhérentes à l'intérieur du béton tendu, on peut définir une limite supérieure à l'ouverture des fissures en admettant un espacement maximal des fissures :
    sr,max = 1,3 (h – x)
    s_s est la contrainte dans les armatures de béton armé tendues, en supposant la section fissurée. Dans le cas des éléments en béton précontraint par pré-tension, s_s peut être remplacée par delta s_p, variation de contrainte dans les armatures de précontrainte depuis l'état correspondant à l'absence de déformation du béton au même niveau
            
    a_e = Ea/Ecm
    ro_pEff = (As + xi1*Ap_p)/A_cEff
    
    Ac,eff est l'aire de la section effective de béton autour des armatures tendues, c'est-à-dire l'aire de la section de béton autour des armatures de traction, de hauteur hc,ef, où hc,ef est la plus petite des trois valeurs ci-après : 2,5(h – d), (h – x)/3 ou h/2 (voir Figure 7.1)
    xi1 est le rapport de la capacité d'adhérence des armatures de précontrainte à la capacité d'adhérence des armatures de béton armé, corrigé du rapport de leurs diamètres :
    La section As doit se lire A's et représente la section des armatures de béton armé situées dans la section Ac,eff :
    Dans le cas d’éléments précontraints :
        — la limite de (h – x)/3 pour le calcul de Ac,eff n’est pas applicable ;
        — delta s_p est la variation de contrainte dans les armatures de précontrainte depuis l’état correspondant à l’absence
        de déformation du béton au même niveau jusqu’à la limite élastique fp0,1k.
    */
    // return sr_max*max(s_s-kt*f_ctEff/ro_pEff(1+a_e*ro_pEff),0.6*s_s)/Es;
    return 0.0;
}