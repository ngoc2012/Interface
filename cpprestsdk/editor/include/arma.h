#ifndef ARMA_H // include guard
#define ARMA_H

#include <vector>
#include <map>

#include "math.h"
#include "charge.h"

using namespace std;
// using namespace core;

namespace arma
{
    enum palierName {horizontal, incline};

    float getCoefficientEffectif(float start, float end, float x, float Lr);

    class arma {
        public:
            float fyk = 500.0;  // MPa
            map<charge::etatName, float> fyd = {
                {charge::etatName::EB, 200.0},
                {charge::etatName::EC, 300.0},
                {charge::etatName::EH, 300.0},
                {charge::etatName::EE, 333.3},
                {charge::etatName::QP, 300.0},
                {charge::etatName::FREQ, 300.0},
                {charge::etatName::CARA, 300.0},
                {charge::etatName::FOND, 500.0/1.15},
                {charge::etatName::ACC, 500.0}
            };
            float Eyoung = 200000.0;   // MPa
            palierName palier = horizontal;
            float k = 1.08, euk = 50.0;
            float eud = 0.9*euk;
            // kValue = {'A': 1.05, 'B': 1.08, 'C': 1.15}
            // self.k = kValue[self.classe]
            // eukValue = {'A': 25.0, 'B': 50.0, 'C': 75.0}
            // self.euk = eukValue[self.classe]

            void checkArma() {
                cout << "fyk = " << fyk << "MPa, fyd[CARA] = " << fyd[charge::etatName::CARA] << "MPa, fyd[FOND] = " << fyd[charge::etatName::FOND] << "MPa, fyd[ACC] = " << fyd[charge::etatName::ACC] << endl;
            }
    };

    enum nappe {EB, EH, EE};

    class contrainteDTU {
        public:
            vector<float> listDia = {5.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 20.0, 25.0, 32.0, 40.0};
            
            map<float, float> contraintes;

            void contrainteDTU2000(float fyk, float fck, nappe n, bool aggressive = false) {
                // §7.3.1.1 DTU 14-1 (NF p 11-221-1)
                float alpha = 320.0; // EB
                if (n == nappe::EH) {
                    alpha = 320.0*1.414213562;
                }
                if (n == nappe::EE) {
                    alpha = 320.0*1.732050808;
                }

                // - Résistance à la traction à 28 jours (BAEL A.2.1,12) et Fasciculae 62-V §A.3.1,3
                // La résistance caractéristique à la traction ftj s´obtient en appliquant à la
                // résistance conventionnelle fc définie en A.3.1,2 ci-dessus la formule préconisée par les règles B.A.E.L. (*).
                // fc = inf(fcj, fc28, fclim)/k1/k2
                float ft28 = 0.6 + 0.06*fck;
                // η un coefficient numérique, dit coefficient de fissuration, qui vaut 1,0 pour les ronds
                // lisses y compris les treillis soudés formés de fils tréfilés lisses et 1,6 pour les armatures à
                // haute adhérence, sauf le cas des fils de diamètre inférieur à 6 mm pour lesquels on prend 1,3.
                // BAEL - A.4.5,33
                float neta;
                
                float sa0;
                float fe = fyk/1.50;

                for (float dia: listDia) {
                    neta = 1.6;
                    if (dia < 6) {neta = 1.3;}
                        
                    if (aggressive) {
                        sa0 = 0.0;
                    } else {
                        sa0 = 30.0*neta;
                    }
                    contraintes.insert(pair<float, float>(
                        dia, 
                        min(fe , alpha*sqrt(neta*ft28/dia) + sa0)
                    ));
                }

                // cout <<"contrainteDTU2000: fck = " << fck << "MPa, ft28 = " << ft28 << "MPa" << endl;
            };

            void checkContrainte() {
                for (float dia: listDia) {
                    cout << "d = " << dia << ", fyd = " << contraintes[dia] << endl;
                }
            }
    };
}

#endif




