#ifndef GEOTECH_H // include guard
#define GEOTECH_H

#include "csv.h"
#include "charge.h"

using namespace std;

namespace geotech  // namespace declaration
{
    enum typeFondation {FS, FB, barrette, FTP, FTR, FTC, TER3, M1, M2, MIGU, MIRS, FSR, FBR, PU};
    enum typeSol {argileLimons, sableGrave, craie, marnesCalcaireMarneux, rocheAltereeFragmentee};
    enum approche {COMP, TRAC};

    struct KfDiagramme {
        vector<float> Kf;
        vector<float> palierContrainte;   // paliers de contrainte
        vector<float> palierDeplacement;    // paliers de déplacement
    };

    extern map<typeFondation, int> classeFondation;
    extern map<typeFondation, int> categorieFondation;
    extern vector<map<typeSol, float>> kpMaxTable;
    extern vector<map<typeSol, float>> qsMaxTable;
    extern vector<map<typeSol, float>> alphaPieuSol;
    extern map<int, float> CmaxList;
    extern map<int, float> k1List;
    extern map<geotech::typeSol, float> qsA;
    extern map<geotech::typeSol, float> qsB;
    extern map<geotech::typeSol, float> qsC;
    extern map<geotech::approche, vector<map<geotech::typeSol, float>>> R1;
    extern typeFondation str2typeFondation(string);

    struct couche {
        public:
            string nom;
            float sup, Em, alpha, pl, pf;
            typeSol typeSol;
            float inf;
            map<typeFondation, float> qs, qsMax, kpMax;
            
            void getKpMax(typeFondation);

            void getQsMax(typeFondation);

            map<charge::etatName, KfDiagramme> getKf(float B, typeFondation tF){
                // Raideur frontale de base
                float kf;
                if (B >= 0.6) {
                    kf = 12.0*Em/( 4.0/3.0 * 0.6/B * pow(2.65*B/0.6, alpha) + alpha);   // MPa
                }
                else {
                    kf = 12.0*Em/( 4.0/3.0 * pow(2.65, alpha) + alpha);        // MPa
                }
                
                float Ls = 0.0; // Longueur de frottment latéral (pieu polygonal)
                float ks = 0.0;
                // // if pieu.horz['Ls'] > 0.1:
                // //     ks = kf     // § I.1.3 (5)
                
                // Les palier de pression
                // 1er palier
                float r1 = pf*B;                // MN/m
                float rs = qs[tF]/1000.0*Ls;    // MN/m
                float r2 = pl*B;		        // MN/m

                map<charge::etatName, KfDiagramme> newDiagramme;
                // I.1.4 Lois d'interaction vis-à-vis des sollicitations de longue durée d'application 
                // 2 paliers - QP

                newDiagramme[charge::etatName::QP].Kf.push_back(0.5*(kf + ks));
                newDiagramme[charge::etatName::QP].palierContrainte.push_back(2.0*min(r1, rs));

                newDiagramme[charge::etatName::QP].Kf.push_back(0.5*max(kf, ks));
                newDiagramme[charge::etatName::QP].palierContrainte.push_back(r1 + rs);
                
                newDiagramme[charge::etatName::FREQ] = newDiagramme[charge::etatName::QP];

                // I.1.3 Lois d'interaction vis-à-vis des sollicitations de courte durée d'application 
                // 2 palierContrainte - CARA, FOND

                newDiagramme[charge::etatName::CARA].Kf.push_back(kf + ks);
                newDiagramme[charge::etatName::CARA].palierContrainte.push_back(2.0*min(r1, rs));

                newDiagramme[charge::etatName::CARA].Kf.push_back(max(kf, ks));
                newDiagramme[charge::etatName::CARA].palierContrainte.push_back(r1 + rs);

                newDiagramme[charge::etatName::FOND] = newDiagramme[charge::etatName::CARA];
            
                // I.1.3 Lois d'interaction vis-à-vis des sollicitations de courte durée d'application 
                // 3 palierContraintes - ACC   
                
                // 2 palierContraintes
                if (2.0*min(r2, rs) == r1) {
                    newDiagramme[charge::etatName::ACC].Kf.push_back(kf + ks);
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(r1);

                    newDiagramme[charge::etatName::ACC].Kf.push_back(0.5*max(kf, ks));
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(r2 + rs);
                }
                
                // 3 palierContraintes
                else if (2.0*min(r2, rs) < r1) {
                    newDiagramme[charge::etatName::ACC].Kf.push_back(kf + ks);
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(2.0*min(r1, rs));

                    newDiagramme[charge::etatName::ACC].Kf.push_back(max(kf, ks));
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(r1);

                    newDiagramme[charge::etatName::ACC].Kf.push_back(0.5*max(kf, ks));
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(r2 + rs);
                }
                else {
                    newDiagramme[charge::etatName::ACC].Kf.push_back(kf + ks);
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(r1);

                    newDiagramme[charge::etatName::ACC].Kf.push_back(0.5*(kf + ks));
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(2.0*min(r1, rs));

                    newDiagramme[charge::etatName::ACC].Kf.push_back(0.5*max(kf, ks));
                    newDiagramme[charge::etatName::ACC].palierContrainte.push_back(r2 + rs);
                }
                
                vector<charge::etatName> etatNameList = {charge::etatName::QP, charge::etatName::CARA, charge::etatName::FOND, charge::etatName::ACC};
                
                for (auto etat: etatNameList) {
                    for (int i; i < newDiagramme[etat].Kf.size(); i++){
                        if (newDiagramme[etat].Kf[i] == 0.0) {
                            newDiagramme[etat].palierDeplacement.push_back(0.0);
                        }
                        else {
                            newDiagramme[etat].palierDeplacement.push_back(newDiagramme[etat].palierContrainte[i]/newDiagramme[etat].Kf[i]);
                        }
                    }
                }
                        
                return newDiagramme;
            }
    };
    
    typeSol getTypeSol(int i);
    
    class sondage {
        public:
            string nom;
            vector<couche> couchesList;
            vector<json> couchesListJson;

            

            void getCouchesList(string filename) {
                couchesListJson = csv::csv2JsonList(filename, 1, 3);
                int i = 0;
                for (auto c: couchesListJson) {
                    // cout << c["nom"] << " " << c["sup"] << endl;
                    couche newCouche;
                    newCouche.nom = c["nom"].get<string>();
                    newCouche.sup = c["sup"].get<float>();
                    newCouche.Em = c["Em"].get<float>();
                    newCouche.alpha = c["alpha"].get<float>();
                    newCouche.pl = c["pl"].get<float>();
                    newCouche.pf = c["pf"].get<float>();
                    newCouche.typeSol = getTypeSol(c["typeSol"].get<int>());

                    if (i>0) {
                        couchesList[couchesList.size()-1].inf = c["sup"].get<float>();
                    }
                    i++;
                    couchesList.push_back(newCouche);
                }
                couchesList[couchesList.size()-1].inf = couchesList[couchesList.size()-1].sup - 100.0;
            };

            void showCouchesList() {
                cout << "nom, sup, inf, Em, alpha, pl, pf, kh" << endl;
                for (auto c: couchesList)
                    cout << c.nom << "   " << c.sup << "   " << c.inf << "   " << c.Em << "   " << c.alpha << "   " << c.pl << "   " << c.pf << endl;
            }
    };
}

#endif