#include "beton.h"

void beton::beton::updateFck(float newFck) {
    fckNominale = newFck;
    fck = newFck;
    fcm = getFcm();
    ec1 = getEc1();
    Ecm = getEcm();
    k = getK();
}

void beton::beton::getJsonParam(json param) {
    updateFck(param["fck"].get<float>());
    // if (model.betonMateriaux.fck = param["materiaux"]["beton"]["fck"].get<float>();
}

void beton::beton::getFckEffective(fondationProfonde fondation) {
    getK2(fondation.B, fondation.L);
    k1 = k1ClasseFondation[fondation.classeFondation];
    Cmax = CmaxClasseFondation[fondation.classeFondation];
    fck = min(fck, Cmax) / (k1 * k2);
    
    if (fondation.arme) {
        acc = 1.0;
    } else {
        acc = 0.8;
    }
        
    
    if (fondation.controleRenforce) {
        k3 = 1.2;
    } else {
        k3 = 1.0;
    }
    
    // output['contBetonCARAmaxLimite'] = min(0.6*k3*output['fck'], 0.6*param['fck'])
    // output['contBetonCARAmoyLimite'] = 0.3*k3*output['fck']
    float fcd0 = acc*min(k3*fck, min(fckNominale, Cmax));
    
    
    fcd[charge::etatName::QP] = min(fcd0/yc[charge::etatName::QP], fcd0*0.6f);
    fcd[charge::etatName::FREQ] = min(fcd0/yc[charge::etatName::FREQ], fcd0*0.6f);
    fcd[charge::etatName::CARA] = min(fcd0/yc[charge::etatName::CARA], fcd0*0.6f);
    fcd[charge::etatName::FOND] = fcd0/yc[charge::etatName::FOND];
    fcd[charge::etatName::ACC] = fcd0/yc[charge::etatName::ACC];
    fcd[charge::etatName::EB] = fcd[charge::etatName::CARA];
    fcd[charge::etatName::EC] = fcd[charge::etatName::CARA];
    fcd[charge::etatName::EH] = fcd[charge::etatName::CARA];
    fcd[charge::etatName::EE] = fcd[charge::etatName::ACC];
    std::cout << "fck =" << fck <<  "fcd0 = " << fcd0 
    <<  " yc[charge::etatName::CARA] = " << yc[charge::etatName::CARA] 
    <<  " fcd[charge::etatName::CARA] = " << fcd[charge::etatName::CARA]
    <<  " yc[charge::etatName::FOND] = " << yc[charge::etatName::FOND] 
    <<  " fcd[charge::etatName::FOND] = " << fcd[charge::etatName::FOND] 
    << std::endl;
}

void beton::beton::checkBeton() {
    cout << "fck = " << fck << "MPa, fcd[CARA] = " << fcd[charge::etatName::CARA] << "MPa, Eyoung[CARA] = " << Eyoung[charge::etatName::CARA] << "MPa" << endl;
    cout << "k1 = " << k1 << ",k2 = " << k2 << ",k3 = " << k3 << ",Cmax = " << Cmax << endl; 
}

void beton::beton::getK2(float B, float L) {
    k2 = 1.0;	//	voir 6.4.1.6 de NF P 94-262
    if ((20*B < L) & (B < 0.6)) {
        k2 = 1.35 - B/2.0;
    } else if (20*B < L) {
        k2 = 1.05;
    } else if (B < 0.6) {
        k2 = 1.3 - B/2.0;
    }
}



