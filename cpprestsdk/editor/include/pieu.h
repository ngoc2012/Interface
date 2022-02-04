#ifndef PIEU_H // include guard
#define PIEU_H

#include "core.h"
#include "mapF.h"

#include "const.h"
#include "geotech.h"
#include "charge.h"
#include "beton.h"

namespace pieu  // namespace declaration
{
    struct portance {
        float qst, qstTotal, kp, ple, Def, kh;
        map<charge::etatName, float> value;
        portance() {
            qst = 0.0;
            qstTotal = 0.0;
            kp = 0.0;
            ple = 0.0;
            Def = 0.0;
            kh = 0.0;
        }
    };

    struct element {
        map<std::string, portance> portance;
        float sup, inf, B;

        element(float sup0, float inf0, float B0) {
            sup = sup0;
            inf = inf0;
            B = B0;
        }

        void getPle(geotech::sondage sondage);
        void getKp(geotech::sondage sondage);
        void getQst(geotech::sondage sondage, geotech::typeFondation typeFondation);
        
    };

    struct KfReduit {
        float startNiveau, startCoef, endNiveau, endCoef;
        bool reduit = false;
    };

    // struct portance {
    //     map<charge::etatName, float> EI;
    //     float qst, ple, kp;
    // };

    class pieuType {
        public:
            string label;
            float sup, supEff, inf, inertie;
            float B, Ce=1.0;
            float surface, perimetre;
            float mailleNiveau;
            beton::beton beton;
            map<charge::etatName, float> EI;
            // map<string, vector<element>> model;
            // map<float, portance> portance;
            // map<float, map<charge::etatName, float>> portance;
            // vector<geotech::sondage> sondages;
            map<std::string, geotech::sondage> sondagesProjet;
            vector<std::string> sondagesNom;
            geotech::typeFondation typeFondation;
            string sondagesTxt;

            // pieuType(map<std::string, geotech::sondage>& sondagesProjet0) {
            //     map<std::string, geotech::sondage>& sondagesProjet = sondagesProjet0;
            // };

            void getParams(json pieu);

            // void getSondages(map<std::string, geotech::sondage>& sondagesProjet, json sondagesJson);

            void getPortance(vector<charge::charge> chargeList);

            void getInf();
            
            void sectionPropReelle() {
                // cout << "surface réelle: " << PI*0.9*0.9/4.0 << endl;
                // cout << "inertie réelle: " << PI*0.9*0.9*0.9*0.9/64.0 << endl;
            };
            
            // void getJsonPieu(json jsonPieu) {
            //     sup = jsonPieu["sup"];
            //     diametre = jsonPieu["diametre"];
            //     getInertie();
            // };

            
            
            void mailleCouche(float elementLength, geotech::couche couche, vector<element>& elementsList) {

            };
            
//             void newModel(geotech::sondage sondage, float elementLength, KfReduit reduction) {
//                 vector<element> elementsList;

//                 if (sup > sondage.couchesList[0].sup) {
//                     cout << "Attention niveau supérieur du pieu " << sup << " est plus au haut que les couches définies " << sondage.couchesList[0].sup << endl;
//                     geotech::couche newCouche = {
//                         "console", // nom
//                         sup,    // sup
//                         0.001,  // Em (MPa)
//                         1.0,    // alpha
//                         0.0,    // pl (MPa)
//                         0.0     // pf (MPa)
//                     };
                        
//                     mailleCouche(elementLength, newCouche, elementsList);
                    
//                     map<charge::etatName, geotech::KfDiagramme> Kf1 = newCouche.getKf(B, typeFondation);
//                 }
                    
//                 int maille, i;
//                 float he, sup, inf, khCoef, z;
//                 for (auto couche: sondage.couchesList) {
//                     if (couche.inf < sup) {
//                         float supCouche = min(sup, couche.sup);
//                         float h = supCouche - max(inf, couche.inf);
//                         if (h > 0.0) {
//                             maille = ceil(h/elementLength);
//                             he = h/maille;
//                             for (i=0; i<maille; i++) {
//                                 sup = supCouche - i*he;
//                                 inf = supCouche - (i+1)*he;
// /*
//                                 // Coefficient de réduction près de la surface du sol
//                                 khCoef = 1.0;
//                                 if (reduction.reduit) {
//                                     if ((reduction.startNiveau <= (sup/2.0 + inf/2.0)) & (reduction.endNiveau >= (sup/2.0 + inf/2.0))) {
//                                         z = reduction.startNiveau - (sup/2.0 + inf/2.0);
//                                         khCoef = 0.5*(1 + z/(zc));
//                                     }
//                                 }
                                    
                                
//                                 # print he, sup, inf
//                                 newEle = element({
//                                     'L': he,
//                                     'couche': couche,
//                                     'pieu': pieu,
//                                     'khCoef': khCoef,
//                                 })
//                                 newEle.etatSup.niveau = sup
//                                 newEle.etatInf.niveau = inf
//                                 # newEle.etatSup.z = sup
//                                 # newEle.etatInf.z = inf
//                                 eleList.append(newEle)*/

//                             }
//                         }
//                     }
//                 }
//                 /*
//                     # print('Pieu/Sollicitation/maillage/couche', couche.sup, couche.inf)
//                     if :
                        
                        
//                 */
//                 model.insert(pair<string, vector<element>>(sondage.nom, elementsList));
//             };
    };

    

    class pieuCirc : public pieuType {
        public:
            float diametre;

            void getInertie() {
                inertie = PI*pow(diametre, 4.0)/64.0;
                EI[charge::etatName::QP] = inertie*beton.Eyoung[charge::etatName::QP];
                EI[charge::etatName::FREQ] = inertie*beton.Eyoung[charge::etatName::FREQ];
                EI[charge::etatName::CARA] = inertie*beton.Eyoung[charge::etatName::CARA];
                EI[charge::etatName::FOND] = inertie*beton.Eyoung[charge::etatName::FOND];
                EI[charge::etatName::ACC] = inertie*beton.Eyoung[charge::etatName::ACC];
            };

            void updateDiametre(float newDiametre) {
                diametre = newDiametre;
                getInertie();
            };
    };

    class pieu {
        pieuType& pieuType;
    };

    extern float groupeEffet(float d, float B, float m, float n);
}

#endif