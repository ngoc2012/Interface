#ifndef BETON_H // include guard
#define BETON_H

#include <map>

#include "charge.h"
#include "jsonF.h"

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

using namespace std;

namespace beton
{
    enum diagrammeName {paraboleRectangle, analyseStructurale};

    struct fondationProfonde {
        int classeFondation;
        float B, L;
        bool arme = true;
        bool controleRenforce = false;
    };

    class beton {
        public:

            map<charge::etatName, float> yc = { {charge::etatName::QP, 1.5}, {charge::etatName::FREQ, 1.5}, {charge::etatName::CARA, 1.5}, {charge::etatName::FOND, 1.5}, {charge::etatName::ACC, 1.2} };

            map<int, float> CmaxClasseFondation ={{1, 35.0}, {2, 35.0}, {3, 35.0}, {4, 35.0}, {9, 40.0}};
            map<int, float> k1ClasseFondation ={{1, 1.3}, {2, 1.35}, {3, 1.3}, {4, 1.3}, {9, 1.15}};
            float fckNominale = 25.0;   // MPa
            float fck = 25.0;   // MPa
            float ecu1 = 3.5;   // %o
            float ecu2 = 3.5;   // %o
            float ec2 = 2.0;   // %o
            float n = 2.0;
            float acc = 1.0;

            float k1 = 1.0;
            float k2 = 1.0;
            float k3 = 1.0;
            float Cmax = 30.0;
            
            // EE, EH, EB == CARA
            map<charge::etatName, float> fcd = {
                {charge::etatName::EE, 20.0},
                {charge::etatName::EH, 20.0},
                {charge::etatName::EB, 20.0},
                {charge::etatName::QP, 10.0},
                {charge::etatName::FREQ, 10.0},
                {charge::etatName::CARA, 20.0},
                {charge::etatName::FOND, 20.0},
                {charge::etatName::ACC, 30.0}
            }; // MPa
            map<charge::etatName, float> Eyoung = {
                {charge::etatName::EE, 30000.0},
                {charge::etatName::EH, 20000.0},
                {charge::etatName::EB, 20000.0},
                {charge::etatName::QP, 10000.0},
                {charge::etatName::FREQ, 10000.0},
                {charge::etatName::CARA, 20000.0},
                {charge::etatName::FOND, 20000.0},
                {charge::etatName::ACC, 30000.0}
            }; // MPa
            
            diagrammeName diagramme=paraboleRectangle;

            float fcm = getFcm();
            float ec1 = getEc1();
            float Ecm = getEcm();
            float k = getK();

            float getFcm() {return fck + 0.8;};
            float getEc1() {return min(0.7*pow(fck + 0.8, 0.31), 2.8);};
            float getEcm() {return 22.0*pow(fcm/10.0, 0.3);};
            float getK() {return 1.05*Ecm*abs(ec1)/fcm;};

            void updateFck(float newFck);

            void getJsonParam(json param);

            void checkBeton();

            void getK2(float B, float L);
                
            void getFckEffective(fondationProfonde fondation);
    };
}

#endif




