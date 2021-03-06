#ifndef RIDO_H // include guard
#define RIDO_H

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <set>
#include <sstream>
#include <iomanip> // setprecision

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

#include "dxflib/dl_dxf.h"

#include "fileF.h"
#include "mapF.h"
#include "stringF.h"
#include "setF.h"
#include "mathF.h"
#include "jsonF.h"
#include "csv.h"
#include "dxf.h"
#include "beton.h"
#include "arma.h"

#include "BAsection.h"

// #include "exprtk/exprtk.hpp"

#include "tinyexpr.h"
#include "stringF.h"

namespace rido  // namespace declaration
{
    struct ridoTableLine {
        struct sol {
            public:
                int etat;
                float pression;
                float surcharge;
                float kh;
        };

        struct sollicitation {
            public:
                float M, V, N;
        };

        public:
            float niveau;
            float deplacement;
            float rotation;
            float chargeRepartie;
            sol sol1;
            sol sol2;
            sollicitation solli;
    };

    enum coteParoi {fouille, terre};

    struct etatNiveau {
        public:
            float niveau, Mmax, Mmin, Vmax, Vmin;
            map<coteParoi, float> A;
            map<coteParoi, float> Adispo;
            map<coteParoi, BAsection::output> output;
            json getJson(){
                return json {
                    {"niveau", niveau},
                    {"Mmax", Mmax},
                    {"Mmin", Mmin},
                    {"Vmax", Vmax},
                    {"Vmin", Vmin},
                    {"AF", A[coteParoi::fouille]},
                    {"AT", A[coteParoi::terre]},
                    {"AdispoF", A[coteParoi::fouille]},
                    {"AdispoT", A[coteParoi::terre]},
                    {"outputF", output[coteParoi::fouille].getJson()},
                    {"outputT", output[coteParoi::terre].getJson()},
                };
            }
    };

    class rido; // forward declaration

    class ridoEtat {
        public:
            charge::etatName nom;
            vector<int> phasesList;
            float ponderation;
            map<float, etatNiveau> table;
            map<float, etatNiveau> tableDecalee;
            float buteeSol1;
            float buteeSol2;

            BAsection::sectionBAprop sectionBA;
            void getSectionBA(rido coupe);
            float getArmaNiveau(rido coupe, float M, float N, float niveau, coteParoi cote);

            json getJson(){
                return json {
                    {"nom", charge::getEtatNameString(nom)},
                    {"ponderation", ponderation},
                    {"sectionBA", sectionBA.getJson()},
                    
                };
            }
    };

    enum appuiType {buton, bracon, tirant, dalle};

    std::string getAppuiTypeString(appuiType a);

    struct appui {
        appuiType type = appuiType::buton;
        int id, lit;
        int pos;
        int cote;
        float niveau, angle, influence, F, K, espacement, precontrainte;
        map<int, float> Flist;
        int nToronts;
        float Lu, Ll, Ls; // tirant

        json getJson(){
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
                {"nToronts", nToronts},
                {"Lu", Lu},
                {"Ll", Ll},
                {"Ls", Ls},
                {"type", appuiType::buton}
            };
        }
    };

    class ridoPhase {
        public:
            int id;
            string description;
            vector<string> textTables;
            vector<ridoTableLine> table;
            float buteeSol1;
            float buteeSol2;
            map<int, pair<float, float>> effortsAppuis;
            vector<appui> appuiList;
            float niveauExcavationFouille = -100000.0;
            float niveauExcavationTerre = -100000.0;
            float dMax = 0.0;
            float dTeteMax = 0.0;
            float Mmax=0.0, Mmin=0.0;

            void showTable() {
                for (ridoTableLine line: table)
                    cout << "|  " << line.niveau << "    |  " << line.deplacement << "    |  " << line.rotation << "    |  " << line.solli.M << "    |  " << line.solli.V << "    |  " << line.solli.N << "    |  " << line.chargeRepartie  << "    |  " << line.sol1.etat  << "    |  " << line.sol1.pression  << "    |  " << line.sol1.surcharge  << "    |  " << line.sol1.kh  << "    |  " << line.sol2.etat  << "    |  " << line.sol2.pression << "    |  " << line.sol2.surcharge << "    |  " << line.sol2.kh << endl;
            };

            void showEffortsAppuis() {
                for (auto effort: effortsAppuis)
                    cout << "Id appui: " << effort.first << ", Niveau: " << effort.second.first << ", Effort:" << effort.second.second << endl;
            }
    };
    
    extern vector<charge::etatName> ELS;
    extern vector<charge::etatName> ELU;
    extern vector<charge::etatName> EAU;

    struct armaParoi {
        string coupeName;
        coteParoi cote;
        float n;
        float HA;
        float sup;
        float inf;
        float nReel;
        // float Lr;
        // getSection 

        json getJson(){
            return json {
                {"coupeName", coupeName},
                {"cote", cote},
                {"n", n},
                {"nReel", nReel},
                {"HA", HA},
                {"sup", sup},
                {"inf", inf}
            };
        }
    };
    
    class armaDispo {
        public:
            vector<armaParoi> armaDispo;
            void getArmaDispo(string, string, map<string, float> params);
            void showArmaLine(int);
            vector<armaParoi> getArmaCoupe(string, coteParoi);
            void showArma();
            float getAeffectif(float niv, coteParoi cote, float ancrage);
            float getAeffectif1(float niv, coteParoi cote, map<float,float> ancrageList);
            
        private:
            coteParoi getCote(string);
    };

    struct barLine {
        string text;
        float sup, inf, x;
    };

    struct couche {
        string nom="";
        float sup, inf, PVh, PVd, Ka, K0, Kp, C, phi, Da, Dp, Re, ple=0.0, Em=0.0;

        json getJson(){
            return json {
                {"nom", nom},
                {"sup", sup},
                {"inf", inf},
                {"PVh", PVh},
                {"PVd", PVd},
                {"Ka", Ka},
                {"K0", K0},
                {"Kp", Kp},
                {"C", C},
                {"phi", phi},
                {"Da", Da},
                {"Dp", Dp},
                {"Re", Re},
                {"ple", ple},
                {"Em", Em}
            };
        }
    };

    struct excavation {
        coteParoi cote;
        float niveau;

        json getJson(){
            return json {
                {"cote", cote},
                {"niveau", niveau}
            };
        }
    };
    
    class rido {
        public:
            rido(string, string, json, json);
            // vector<charge::etatName> etatList;
            int id;
            float epaisseur = 0.0;
            float enrobage = 0.0;
            float ratio = 100.0;
            bool impermeable = false;
            string coupeName;
            string folder;
            map<string, float> params;
            vector<ridoPhase> phases;
            vector<appui> appuiList;
            vector<couche> coucheList;
            vector<excavation> excavationList;
            int getPhasesNumber() {return phases.size();};
            void getRioData(string fileName);
            string comment;
            float supCouche;
            void getAppui(vector<string> command, vector<string> commandParams);
            couche getCoucheData(vector<string> commandParams);
            void getExcavation(vector<string> command, vector<string> commandParams);
            map<charge::etatName, ridoEtat> etats;
            void getNewEtat(vector<int> phasesList, float ponderation, charge::etatName nom);

            // Sollicitations
            void getMoment(json coupeParam);
            void decalerMomentEtat(float distance, charge::etatName nom);
            float Amin;

            // Armatures
            arma::contrainteDTU contrainteDTU;
            void getMateriaux(json param);
            void getArmatures(json coupeParam, std::string fileName);
            void getArmaturesEtat(charge::etatName nom, float effort);
            void getArmaturesMinimal(bool traction);
            void verifArmatures(json coupeParam, charge::etatName etatName);

            vector<BAsection::element> getBetonT(string, string);
            void verifArmaturesT(json coupeParam, charge::etatName etatName, string points, string elements, string coorBars, string dispoBars);

            void getJsonOuput(string fileName);
            beton::beton betonMateriaux;
            arma::arma armaMateriaux;
            map<charge::etatName, map<coteParoi, map<float, float>>> contrainteArmaturesELS;
            armaDispo armaDispo;
            map<coteParoi, map<float, float>> Adispo;
            void getAdispo(charge::etatName);

            
            vector<BAsection::element> getAdispoT(float niveau, string coorBars, string dispoBars);

            // float ancrage = 35.0;
            float ancrage = 50.0;
            map<float, float> ancrageList;
            void getContrainteDTU(map<float, float>, charge::etatName);
            // void getContrainteDTU2020(map<float, float>, string);
            void checkContrainteArmaturesELS(charge::etatName, coteParoi);

            set<float> getNiveauxArmaDispo(coteParoi cote);
            map<float, float> getArmaDispoLines(coteParoi);
            vector<barLine> getBarLines();

            // Panneautage
            json paramsPanneautage;
            void getRatio();
            
            void showAppuis() {
                for (auto appui: appuiList) {
                    std::cout << "id = " << appui.id << ", type = " << appui.type << ", lit = " << appui.lit << ", angle = " << appui.angle << ", espacement = " << appui.espacement << ", precontrainte = " << appui.precontrainte << ", F = " << appui.F << ", K = " << appui.K << ", Ll = " << appui.Ll << ", Ls = " << appui.Ls << endl;
                }
            };

            void checkCoucheList() {
                for (auto couche: coucheList) {
                    std::cout << couche.nom << " " << couche.sup << " " << couche.inf << " " << couche.PVh << endl;
                }
            }

            void writeDxfPhase(DL_Dxf& dxf, DL_WriterA& dw, ridoPhase& phase, std::vector<excavation>& excavations, float startX);
            void dxfCoupeType();
    };

    void dxfSection(std::string filename, std::string coorBars, std::string dispoBars);

    struct lierne {
        int panID, lit;
        string label;
        string etatName;
        string type;
        float a1, a2, L1, L2, L3;
        float niveau;
        float nT, HAT, nF, HAF;
        float ATdispo, AFdispo;
        float fyd=100.0;
        float fydF=100.0;
        float fcd=10.0;
        // string coupeName;

        // Output:
        float p;
        float b;
        float Mt, Ma1, Ma1e, Ma2, Ma2e;
        float Va1g, Va1ge, Va2g, Va2ge;
        float Va1d, Va1de, Va2d, Va2de;
        float Vmax;
        float VRdc;
        // float Va1ge, Va2ge;
        // float Va1de, Va2de;
        float Ra1, Ra1e, Ra2, Ra2e;
        float Aa1 = 0.0, Aa2 = 0.0, At = 0.0, Aap1 = 0.0, Aap2 = 0.0, Atp = 0.0;
        BAsection::output outputA1, outputA2, outputT;

        void getMoment();
        void getArmatures(rido coupe, charge::etatName etatName);
        void verifArmatures(rido coupe, charge::etatName etatName);
        json getJson(){
            return json {
                {"outputA1", outputA1.getJson()},
                {"outputA2", outputA2.getJson()},
                {"outputT", outputT.getJson()},
                {"panID", panID},
                {"lit", lit},
                {"label", label},
                {"etatName", etatName},
                {"a1", a1},
                {"a2", a2},
                {"L1", L1},
                {"L2", L2},
                {"L3", L3},
                {"niveau", niveau},
                {"nT", nT},
                {"HAT", HAT},
                {"nF", nF},
                {"HAF", HAF},
                {"ATdispo", ATdispo},
                {"AFdispo", AFdispo},
                {"fyd", fyd},
                {"fydF", fydF},
                {"fcd", fcd},
                {"p", p},
                {"b", b},
                {"VRdc", VRdc},
                {"Mt", Mt},
                {"Ma1", Ma1},
                {"Ma1e", Ma1e},
                {"Ma2", Ma2},
                {"Ma2e", Ma2e},
                {"Va1g", Va1g},
                {"Va1ge", Va1ge},
                {"Va2g", Va2g},
                {"Va2ge", Va2ge},
                {"Va1d", Va1d},
                {"Va1de", Va1de},
                {"Va2d", Va2d},
                {"Va2de", Va2de},
                {"Vmax", Vmax},
                {"Ra1", Ra1},
                {"Ra1e", Ra1e},
                {"Ra2", Ra2},
                {"Ra2e", Ra2e},
                {"Aa1", Aa1},
                {"Aa2", Aa2},
                {"At", At},
                {"Aap1", Aap1},
                {"Aap2", Aap2},
                {"Atp", Atp},
            };
        }
    };

    struct coor: dxf::point {};

    struct edge {
        int id;
        string label;
        coor start, end, vectorUnite, perpUnite;
        float length;

        edge() {
            id = 0;
            start.x = 0.0;
            start.y = 0.0;
            end.x = 0.0;
            end.y = 0.0;
        }

        edge(int id0, float startX, float startY,  float endX, float endY) {
            id = id0;
            start.x = startX;
            start.y = startY;
            end.x = endX;
            end.y = endY;
            getLength();
            getVectorUnite();
        }

        template <class T>
        edge(int id0, T start0, T end0) {
            id = id0;
            start.x = start0.x;
            start.y = start0.y;
            end.x = end0.x;
            end.y = end0.y;
            getLength();
            getVectorUnite();
            // cout << "Edge: id = " << id0 << " start.x = " << start.x << ", start.y = " << start.y << ", end.x = " << end.x << ", end.y = " << end.y << endl;
            // cout << "length = " << length << ", vectorX = " << vectorUnite.x << ", vectorY = " << vectorUnite.y << endl;
        }
            
        
        void getLength() {
            length = sqrt(pow(start.x - end.x, 2.0) + pow(start.y - end.y, 2.0));
            // return length;
        };

        void getVectorUnite() {
            if (length > 0.0) {
                vectorUnite.x = (end.x - start.x)/length;
                vectorUnite.y = (end.y - start.y)/length;
                perpUnite.x = -vectorUnite.y;
                perpUnite.y = vectorUnite.x;
            }
        }

        void offset(float startOffset, float endOffset) {
            // cout << "perpUnite.x = " << perpUnite.x << ", perpUnite.y = " << perpUnite.x << ", startOffset=" << startOffset << ", endOffset=" << endOffset << endl;
            start.x += perpUnite.x*startOffset;
            start.y += perpUnite.y*startOffset;
            end.x += perpUnite.x*endOffset;
            end.y += perpUnite.y*endOffset;
            getVectorUnite();
        }
    };

    // vector<edge> getEdgeList(vector<dxf::entity> entities, vector<string> layers, vector<string> entitiesTypes);

    void showEdgeList(vector<edge> edgeList);

    enum panneauType {primaire, fermeture, consecutif};

    struct box: edge {
        box(int id_, float startX, float startY,  float endX, float endY, int coupeId0, float epaisseur0) : edge(id_, startX, startY, endX, endY) {
            // cout << "id0 = " << id0 << ", startX = " << startX << ", startY = " << startY << ", endX = " << endX << ", endY = " << endY << endl;
            coupeId = coupeId0;
            epaisseur = epaisseur0;
        }
        // int id0;
        int coupeId;
        float epaisseur;
        vector<coor> polyline;

        void getPolyline();
    };

    struct cage : box {
        cage(int id_, float startX, float startY,  float endX, float endY, int coupeId0, float epaisseur0) : box(id_, startX, startY,  endX, endY, coupeId0, epaisseur0) {};

        int panId;
        string label0;
        vector<armaParoi> arma;

        json getJson();
    };

    struct panneau : box {
        panneau(int id_, float startX, float startY,  float endX, float endY, int coupeId0, float epaisseur0) : box(id_, startX, startY, endX, endY, coupeId0, epaisseur0) {};

        float elevationStartX;
        vector<int> cageList;
        int nCages;

        json getJson(vector<cage>& cageList);
    };
    
    void checkPanneauList(vector<panneau> panneauList, vector<rido> coupeList);

    struct sectionA {
        float sa=1000.0;
        float angle, Ls, section, d0;

        json getJson(){
            return json {
                {"sa", sa},
                {"angle", angle},
                {"Ls", Ls},
                {"section", section},
                {"d0", d0}
            };
        }
    };

    struct tige {
        int n=6;
        float As, sbadm, sb, Lt=400.0, espacement;
        float FvbRd=0.0, FbRd=0.0;
        std::string txt;

        json getJson(){
            return json {
                {"n", n},
                {"As", As},
                {"sbadm", sbadm},
                {"sb", sb},
                {"Lt", Lt},
                {"espacement", espacement},
                {"FvbRd", FvbRd},
                {"FbRd", FbRd},
                {"txt", txt}
            };
        }
    };

    struct beche {
        int n=3;
        float da, La, MRd1, MRd2, espacement, sb;
        json getJson(){
            return json {
                {"n", n},
                {"da", da},
                {"La", La},
                {"MRd1", MRd1},
                {"MRd2", MRd2},
                {"espacement", espacement},
                {"sb", sb}
            };
        }
    };

    struct mortier {
        float k, d, vmin, Ac, scp, VRdc, scplim, fcd;
        json getJson(){
            return json {
                {"k", k},
                {"d", d},
                {"vmin", vmin},
                {"Ac", Ac},
                {"scp", scp},
                {"VRdc", VRdc},
                {"scplim", scplim},
                {"fcd", fcd}
            };
        }
    };

    struct assemblage {
        float VFa, VFb, VA, V, VB, VBA;
        tige tige;
        mortier mortier;
        beche beche;
        float beta_w = 0.8;
        float gamma_M2 = 1.25;

        json getJson(){
            return json {
                {"VFa", VFa},
                {"VFb", VFb},
                {"VA", VA},
                {"V", V},
                {"VB", VB},
                {"VBA", VBA},
                {"tige", tige.getJson()},
                {"mortier", mortier.getJson()},
                {"beche", beche.getJson()},
                {"beta_w", beta_w},
                {"gamma_M2", gamma_M2}
            };
        }
    };

    struct soudure {
        float pt;
        int ab=3, ac=3;
        

        json getJson(){
            return json {
                {"pt", pt},
                {"ab", ab},
                {"ac", ac}
            };
        }
    };

    struct platine {
        float a1=0.150;
        float s1=1000.0;
        float tp=10.0;
        float dt=30.0;
        float Hp, Lp, pu;
        float aire;
        float dt0, HpEffectif, LpEffectif, puEffectif;
        sectionA sectionA;
        soudure soudure;
        void contraintePlatine(float a, float b);
        
        float fu = 360.0;

        json getJson(){
            return json {
                {"a1", a1},
                {"s1", s1},
                {"tp", tp},
                {"dt", dt},
                {"Hp", Hp},
                {"Lp", Lp},
                {"pu", pu},
                {"aire", aire},
                {"dt0", dt0},
                {"HpEffectif", HpEffectif},
                {"LpEffectif", LpEffectif},
                {"puEffectif", puEffectif},
                {"sectionA", sectionA.getJson()},
                {"soudure", soudure.getJson()},
                {"fu", fu}
            };
        }
    };

    struct bout : appui {
        float fck;
        int panneauId;
        float P, Fa, Kr, Ft, T, N;
        float precontrainte0;
        float Ksol=10000.0; // Raideur du buton vers terre pour dimensionner le buton
        // float distanceAuMilieu;
        platine platine;
        assemblage assemblage;

        json getJson();
    };

    struct metalMateriaux {
        float fy, Eyoung, Gyoung;
        float fu = 360.0;
        // json getJson(){
        //     return json {
        //         {"fy", fy},
        //         {"Eyoung", Eyoung},
        //         {"Gyoung", Gyoung}
        //     };
        // }
    };
    
    struct profileMetal: metalMateriaux {
        float section;
        float G;
        float Iy, Iz, IT; // m4
        float Wy;
        float iy; // m
        float h, tf;
        float dte2;
        int classe;
        void getClasse() {
            // 		but["dte2"] = but["profile"]["h (mm)"]*but["profile"]["tf (mm)"]/pow(235.0/but["fy"], 0.5)
            // 		if but["dte2"] <= 50.0:
            // 			but["classe"] = 1
            // 		elif but["dte2"] <= 70.0:
            // 			but["classe"] = 2
            // 		elif but["dte2"] <= 90.0:
            // 			but["classe"] = 3
            dte2 = h * tf / sqrt(235.0/fy);
			if (dte2 <= 50.0) {
				classe = 1;
			} else if (dte2 <= 70.0) {
				classe = 2;
			} else if (dte2 <= 90.0) {
				classe = 3;
			}
        }

        json getJson(){
            return json {
                {"fy", fy},
                {"Eyoung", Eyoung},
                {"Gyoung", Gyoung},
                {"section", section},
                {"G", G},
                {"Iy", Iy},
                {"Iz", Iz},
                {"IT", IT},
                {"Wy", Wy},
                {"iy", iy},
                {"h", h},
                {"tf", tf},
                {"dte2", dte2},
                {"classe", classe}
            };
        }
    };

    struct flambement {
        // input
        profileMetal* profile;
        float section, Lcr;
        // output
        float lambdaFL, alphaFL, phiFL, chiFL, NbRd, Ncr;
        void getNbRd() {
            // 	but["Ncr"] = math.pi*but["profile"]["Iy (cm4)"]/100000000.0*Eyoung/but["Lcr"]/but["Lcr"] # t
            // 		but["lambdaFL"] = pow( but["profile"]["A (cm2)"]/10000.0 * but["fy"] * 100.0 / but["Ncr"], 0.5)
            // 		but["alphaFL"] = 0.49
            // 		but["phiFL"] = 0.5*(1.0 + but["alphaFL"]*(but["lambdaFL"] - 0.2) + but["lambdaFL"]*but["lambdaFL"])
            // 		but["chiFL"] = min(1.0/( but["phiFL"] + pow(but["phiFL"]*but["phiFL"] - but["lambdaFL"]*but["lambdaFL"], 0.5) ), 1.0)
            // 		but["NbRd"] = but["chiFL"]*but["profile"]["A (cm2)"]/10000.0 * but["fy"] * 100.0
            Ncr = PI*profile->Iy*profile->Eyoung/Lcr/Lcr;
            lambdaFL = sqrt(profile->section * profile->fy * 100.0 / Ncr);
            alphaFL = 0.49;
            phiFL = 0.5*(1.0 + alphaFL*(lambdaFL - 0.2) + lambdaFL*lambdaFL);
            chiFL = min(1.0, 1.0/( phiFL + sqrt( phiFL*phiFL - lambdaFL*lambdaFL ) ));
            NbRd = chiFL*profile->section*profile->fy*100.0;
        }

        json getJson(){
            return json {
                {"section", section},
                {"Lcr", Lcr},
                {"lambdaFL", lambdaFL},
                {"alphaFL", alphaFL},
                {"phiFL", phiFL},
                {"chiFL", chiFL},
                {"NbRd", NbRd},
                {"Ncr", Ncr},
                {"profile", profile->getJson()}
            };
        }
    };

    struct derversement {
        profileMetal* profile;
        float length, Lcr;
        float Mcr, lambdaLT, alphaLT, phiLT, chiLT, MbRd;
        
        void getMbRd() {
            // 		but["Mcr"] = 1.13*math.pi*pow(but["profile"]["Iz (cm4)"]/100000000.0*but["profile"]["IT (cm4)"]/100000000.0*Eyoung*Gyoung, 0.5)/but["length"] # tm
            // 		but["lambdaLT"] = pow( but["profile"]["Wy (cm3)"]/1000000.0 * but["fy"] * 100.0 / but["Mcr"], 0.5)
            // 		but["alphaLT"] = 0.76
            // 		but["phiLT"] = 0.5*(1.0 + but["alphaLT"]*(but["lambdaLT"] - 0.2) + but["lambdaLT"]*but["lambdaLT"])
            // 		but["chiLT"] = min( 1.0/( but["phiLT"] + pow(but["phiLT"]*but["phiLT"] - but["lambdaLT"]*but["lambdaLT"], 0.5) ), 1.0)
            // 		if but["lambdaLT"] <= 0.2:
            // 			but["MbRd"] = but["profile"]["Wy (cm3)"]/1000000.0 * but["fy"] * 100.0 # tm
            // 		else:
            // 			but["MbRd"] = but["chiLT"]*but["profile"]["Wy (cm3)"]/1000000.0 * but["fy"] * 100.0 # tm
            Mcr = 1.13*PI*sqrt(profile->Iz*profile->IT*profile->Eyoung*profile->Gyoung)/length; // tm
			lambdaLT = sqrt( profile->Wy * profile->fy * 100.0 / Mcr );
			alphaLT = 0.76;
			phiLT = 0.5*(1.0 + alphaLT*(lambdaLT -0.2) + lambdaLT*lambdaLT);
			chiLT = min(1.0, 1.0/( phiLT + sqrt(phiLT*phiLT - lambdaLT*lambdaLT) ));
			if (lambdaLT <= 0.2) {
				MbRd = profile->Wy * profile->fy * 100.0; // tm
			} else {
				MbRd = chiLT*profile->Wy * profile->fy * 100.0; // tm
				}
        }

        json getJson(){
            return json {
                {"length", length},
                {"Lcr", Lcr},
                {"Mcr", Mcr},
                {"lambdaLT", lambdaLT},
                {"alphaLT", alphaLT},
                {"phiLT", phiLT},
                {"chiLT", chiLT},
                {"MbRd", MbRd},
                {"profile", profile->getJson()}
            };
        }
    };

    struct flexionComposee {
        profileMetal* profile;
        flambement flambement;
        derversement derversement;
        float length;
        float NcrT, NcrTF, lambdacr, Cmy0, Cmy, CmLT, aLT, varepsilony, muy, kyy, DeltaFC;
        
        // Voir note buton Aheme;

        bool checkFlexionComposee(float NEd, float MEdy) {
            flambement.profile = profile;
            flambement.Lcr = 0.7*length;
            flambement.getNbRd();
            derversement.profile = profile;
            derversement.Lcr = 0.7*length;
            derversement.length = length;
            derversement.getMbRd();
            
            // 		# Flexion - compression
            // 		but["NcrT"] = but["profile"]["IT (cm4)"]/100000000.0 * Gyoung / 2.0 / but["profile"]["iy (cm)"]/ but["profile"]["iy (cm)"] * 10000.0 # t
            // 		but["NcrTF"] = min(but["Ncr"], but["NcrT"])
            // 		but["lambdacr"] = 0.2*pow(1.13, 0.5)*pow((1 - but["NEd"]/but["Ncr"]) * (1 - but["NEd"]/but["NcrTF"]), 0.25)
            // 		but["Cmy0"] = 1 + 0.03*but["NEd"]/but["Ncr"]
            // 		if but["lambdaLT"] <= but["lambdacr"]:
            // 			but["Cmy"] = but["Cmy0"]
            // 			but["CmLT"] = 1.0
            // 		else:
            // 			but["aLT"] = max(1 - but["profile"]["IT (cm4)"]/but["profile"]["Iy (cm4)"], 0.0)
            // 			but["varepsilony"] = but["MEdy"]/but["NEd"]*but["profile"]["A (cm2)"]/but["profile"]["Wy (cm3)"]*100.0
            // 			but["Cmy"] = but["Cmy0"] + (1 - but["Cmy0"])*pow(but["varepsilony"], 0.5)*but["aLT"]/(1 + pow(but["varepsilony"], 0.5)*but["aLT"])
            // 			but["CmLT"] = max(but["Cmy0"]*but["Cmy0"] * but["aLT"] / pow((1 - but["NEd"]/but["Ncr"]) * (1 - but["NEd"]/but["NcrT"]), 0.5), 1.0)
                    
            // 		but["muy"] = (1 - but["NEd"]/but["Ncr"]) / (1 - but["chiFL"] * but["NEd"]/but["Ncr"])
            // 		but["kyy"] = but["Cmy"] * but["CmLT"] * but["muy"] / (1 - but["NEd"]/but["Ncr"])
            // 		but["DeltaFC"] = but["NEd"]/but["NbRd"] + but["kyy"]*but["MEdy"]/but["MbRd"]
            // 		condition = condition & (but["DeltaFC"] <= 1.0)
            
            NcrT = profile->IT * profile->Gyoung / 2.0 / profile->iy*profile->iy; // t
			NcrTF = min(flambement.Ncr, NcrT);
			lambdacr = 0.2*sqrt(1.13)*pow((1 - NEd/flambement.Ncr) * (1 - NEd/NcrTF), 0.25);
			Cmy0 = 1 + 0.03*NEd/flambement.Ncr;
			if (derversement.lambdaLT <= lambdacr) {
				Cmy = Cmy0;
				CmLT = 1.0;
			} else {
				aLT = max(1.0 - profile->IT/profile->Iy, 0.0);
				varepsilony = MEdy/NEd*profile->section/profile->Wy*100.0;
				Cmy = Cmy0 + (1.0 - Cmy0)*sqrt(varepsilony)*aLT/(1.0 + sqrt(varepsilony)*aLT);
				CmLT = max(Cmy0*Cmy0 * aLT / sqrt((1.0 - NEd/flambement.Ncr) * (1.0 - NEd/NcrT)), 1.0);
			}
			
			muy = (1.0 - NEd/flambement.Ncr) / (1.0 - flambement.chiFL * NEd/flambement.Ncr);
			kyy = Cmy * CmLT * muy / (1.0 - NEd/flambement.Ncr);
			DeltaFC = NEd/flambement.NbRd + kyy*MEdy/derversement.MbRd;
            return (DeltaFC <= 1.0);
        }

        json getJson(){
            return json {
                {"length", length},
                {"NcrT", NcrT},
                {"NcrTF", NcrTF},
                {"lambdacr", lambdacr},
                {"Cmy0", Cmy0},
                {"Cmy", Cmy},
                {"CmLT", CmLT},
                {"aLT", aLT},
                {"varepsilony", varepsilony},
                {"muy", muy},
                {"kyy", kyy},
                {"DeltaFC", DeltaFC},
                {"profile", profile->getJson()},
                {"flambement", flambement.getJson()},
                {"derversement", derversement.getJson()}
            };
        }
        // flexionComposee ()
    };

    struct semelle {
        float qnet, Rvd, kp, d_sol, i_beta, i_delta, taux;
        float Ks=0.0;
        float ps, As, MEd, MEd_reduit, d;

        json getJson() {

            return json {
                {"qnet", qnet},
                {"Rvd", Rvd},
                {"kp", kp},
                {"d_sol", d_sol},
                {"i_beta", i_beta},
                {"i_delta", i_delta},
                {"taux", taux},
                {"Ks", Ks},
                {"ps", ps},
                {"As", As},
                {"MEd", MEd},
                {"MEd_reduit", MEd_reduit},
                {"d", d}
            };
        }
    };

    struct butonMetal : edge {
        
        // input
        int id;
        std::string label;
        int lit;
        std::string profileNom="";
        // float d = 400.0;
        // float t = 100.0;
        std::string startPan, endPan;
        std::string startCoupe, endCoupe;
        float precontrainte;
        float LA, GL, PL, TG, TD;
        float FaMax;
        // float Lcr;
        float poids, FEdx, FEdz, MEdy, NEd;
        
        float fck, Dmax, scplim, fcd;
        float frottementBetonBeton;
        float frottementAcierBeton;
        float mortierFck;
        
        // output
        float Kt;
        float dilatationT;
        bout startBout;
        bout endBout;
        profileMetal profile;
        flexionComposee flexionComposee;
        
        json butonJson;
        json* Cprofiles;

        // bracons
        semelle semelle;
//        float qnet, Rvd, kp, d_sol, i_beta, i_delta, taux;
//        float Ks=0.0;

        void getCharges();
        bool checkKr();
        void dimButons();
        void getProfile(json&);
        void getMateriaux(json);
        void dimPlatine(bout&);
        void dimAssemblage(bout&);
        json getJson();
    };
 
    vector<butonMetal> butonnage(vector<rido> coupeList, vector<panneau> panneauList, string fileButonDispo, json materiaux);

    void dimButons(vector<butonMetal>& butonList, vector<panneau> panneauList, json materiaux);

    struct tirantPosition {
        vector<int> pan, lit;
        int pos;
        float dis, angle;
    };

    // Singleton
    struct paroi {
        vector<edge> edgesList;
        vector<rido> coupeList;
        vector<butonMetal> butonList;
        vector<panneau> panneauList;
        vector<cage> cageList;
        // vector<int> tirantOff;
        json params;
        // json pelle;
        json materiaux;
        float poidsButons, poidsPlatines;

        map<std::string, vector<lierne>> lierneList;
        void getLiernes(std::string fileName);
        void getNewLierne(int coupeId, json param);

        void getEdgesList(json paroiParam);

        void panneautage();
        void getCages(float enrobage);
        // void getCagesDxf(float enrobage);

        void panneautageMetre(std::string fileName);
        void panneautageMetrePanneaux(std::string fileName);
        void panneautagePrix(std::string fileName);
        // void getEdgeList(vector<dxf::entity> entities, vector<string> layers, vector<string> entitiesTypes);
        void checkPanneauList();
        void getPanneauxInEdge(edge currentEdge, rido coupe, json line, vector<json>& panDep);
        void recapTirant(std::string fileName);

        void butonnage(std::string fileButonDispo, json& Cprofiles);
        void braconnage(std::string fileButonDispo, json& Cprofiles);
        void checkButonList();
        void dxfPanneautage(std::string);
        void writeDxfEdges(DL_Dxf& dxf, DL_WriterA& dw);
        void writeDxfTirants(panneau& pan, DL_Dxf& dxf, DL_WriterA& dw, int& tirantId);
        void writeDxfPanneaux(DL_Dxf& dxf, DL_WriterA& dw);
        void writeDxfCages(DL_Dxf& dxf, DL_WriterA& dw);
        void writeDxfElevation(DL_Dxf& dxf, DL_WriterA& dw, vector<int> panIdList);
        void writeDxfButons(DL_Dxf& dxf, DL_WriterA& dw);

        int getnLitButonsMax();
        int getCoupeByName(std::string coupeName) {
            // bool isCoupeExist = false;
            for (rido& coupe: coupeList) {
                if (coupe.coupeName == coupeName) {
                    return coupe.id;
                }
            }
            // if (!isCoupeExist) {
            //     cout << "Coupe " << coupeName << " does not exist" << endl;
            //     exit(0);
            // }
            return -1;
        };
        
        void getJson(std::string fileName);
    };

}

#endif
