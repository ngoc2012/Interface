#ifndef BASECTION_H // include guard
#define BASECTION_H

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <math.h>

#include "core.h"
#include "stringF.h"
#include "mathF.h"

#include "charge.h"
#include "beton.h"
#include "arma.h"

#define PI 3.14159265f

namespace BAsection  // namespace declaration
{
    struct element {
        public:
            float x, y, surface;
            element() {
                
            }
            element(float x0, float y0, float surface0) {
                x = x0;
                y = y0;
                surface = surface0;
            }
    };

    struct output {
        float eaMin=0.0, ebMax=0.0, ebMin=0.0, faMin=0.0, faMax=0.0, fbMax=0.0, fbMoy=0.0;

        template <typename T> void getOutput(T section) {
            ebMax = section.output.ebMax;
            eaMin = section.output.eaMin;
            faMin = section.output.faMin;
            fbMax = section.output.fbMax;
        };

        
        json getJson(){
            return json {
                {"ebMax", ebMax},
                {"eaMin", eaMin},
                {"faMin", faMin},
                {"fbMax", fbMax}
            };
        };
    };

    class BAsection {

        protected:
            float e0 = 0.0;    // °/oo
            float ax = 0.0;    // °/oo
            float ay = 0.0;    // °/oo

            float dif = 100.0;
            float det = 100.0;
            int nBoucles = 0;
            int bouclesLimite = 100;
            float tolerence = 0.0001;
            float ebMaxLimite = 10.0;

            vector<float> fa, fb, ea, eb, dfa, dfb;

            vector<float> Xa, Ya, Sa;
            vector<float> Xb, Yb, Sb;
        public:

            charge::charge charge;
            beton::beton beton;
            arma::arma arma;

            output output;
            
            float Eb;
            float fyd;
            float fcd;
            float Ea;

            void checkMateriaux() {
                cout << "fyd = " << fyd << ", fcd = " << fcd << ", Ea = " << Ea  << ", Eb = " << Eb << endl;
            };

            void getFa();

            void getFb();

            void getMaxMin();

            void reset();

    };

    class BAsection2D : public BAsection {
        public:
            
            vector<element> sectionBeton;
            vector<element> dispoArma;

            void getFbMoy() {
                float effortBeton = 0.0;
                float surfaceBeton = 0.0;
                // vector<float> Xb, Yb, Sb;
                int i = 0;
                for (auto& b: sectionBeton) {
                    surfaceBeton += b.surface;
                    effortBeton += b.surface*fb[i];
                    i++;
                }
                output.fbMoy = effortBeton/surfaceBeton;
            }
            
            
            void verif();

            void checkCharges() {

                float sumMy=0.0, sumMx=0.0, sumN=0.0;

                cout << "Mx = " << charge.Mx << "MN, My " << charge.My << "MN, N = " << charge.N << "MN" << endl;
                vector<float> Xa, Ya, Sa;
                for (auto& a: dispoArma) {
                    Xa.push_back(a.x);
                    Ya.push_back(a.y);
                    Sa.push_back(a.surface);
                }

                vector<float> Xb, Yb, Sb;
                for (auto& b: sectionBeton) {
                    Xb.push_back(b.x);
                    Yb.push_back(b.y);
                    Sb.push_back(b.surface);
                }

                sumMy = mathF::sumprod3(Xa, Sa, fa) + mathF::sumprod3(Xb, Sb, fb);
                sumMx = mathF::sumprod3(Ya, Sa, fa) + mathF::sumprod3(Yb, Sb, fb);
                sumN = mathF::sumprod2(Sa, fa) + mathF::sumprod2(Sb, fb);

                cout << "sumMx = " << sumMx << ", sumMy = " << sumMy << ", sumN = " << sumN << endl;
            };

            void showOutput() {
                float surfaceBeton = 0.0;
                for (auto& b: sectionBeton)
                    surfaceBeton += b.surface;

                cout << "faMin = " << output.faMin << ", fbMax = " << output.fbMax << ", ebMax = " << output.ebMax << ", fbMoy = " << output.fbMoy << ", N/Sb = " << charge.N/surfaceBeton << endl;
                cout << "nBoucles = " << nBoucles << ", dif = " << dif << ", det = " << det << endl;
                cout << "e0 = "<< e0 << ",ax = "<< ax << ",ay = "<< ay << endl;
            };

            
        
    };

    class rectangle : public BAsection {
        public:
            // charge::charge charge;
            // vector<element> sectionBeton;
            // vector<element> dispoArma;
            float b, h;
            float Asup, Ainf;
            float dsup, dinf;

            int maille = 100;

            void mailleSection();

            void checkSection();

            void verif();

            void checkCharges();

            void getArmatures();
        
    };

    vector<element> maillerSectionBetonCercle(float, float, int);
    
    vector<element> dispoArmaCercle(int, float, float);
    
    void sectionBetonProp(vector<element>);

    enum method {exact, simplified};

    struct sectionBAprop {
        float b, h, d, fcd, fyd, Ea, Eb, Ecm;
        float c = 100.0; // mm, c est l'enrobage des armatures longitudinales
        method method = method::simplified;
        void showSectionBAprop() {
            cout << "fcd = " << fcd << "MPa, fyd = " << fyd << "MPa, Ea = " << Ea << "MPa, Eb = " << Eb << "MPa, Ecm = " << Ecm << "MPa" << endl;
        };
        json getJson(){
            return json {
                {"b", b},
                {"h", h},
                {"d", d},
                {"fcd", fcd},
                {"fyd", fyd},
                {"Ea", Ea},
                {"Eb", Eb},
                {"Ecm", Ecm},
                {"c", c},
            };
        };

    };

    struct armaRectangle {
        float Ap = 0.0; // cm²
        float A = 0.0;  // cm²
    };

    struct arma0 {
        float n;
        float HA;
        float sup;
        float inf;
    };

    float fiss(sectionBAprop, BAsection2D, charge::charge);

    float armELU(float, sectionBAprop);

    float armELS(float, sectionBAprop);

    float armELUtraction(float, float, sectionBAprop);

    float armELStraction(float, float, sectionBAprop);

    std::tuple<float, float> armELUAp(float MEd, sectionBAprop section);
}

#endif