#include "BAparoi.h"

using namespace std;
// using namespace core;

class BAparoi::armaDispo {
        public:

            vector<BAparoi::arma> armaDispo;

            void getArmaDispo(string fileName) {
                vector<json> armaDisposeJson = csv::csv2JsonList(fileName, 1, 2);
                for(json a : armaDisposeJson) {
                    // check valide ...
                    armaDispo.push_back(BAparoi::arma{
                        a["Coupe"].get<string>(),
                        getCote(a["Cote"].get<string>()),
                        a["n"].get<float>(),
                        a["HA"].get<float>(),
                        a["sup"].get<float>(),
                        a["inf"].get<float>()
                    });
                }
            };

            void showArmaLine(int i) {
                cout << armaDispo[i].coupeName << ", " << armaDispo[i].cote << ", " << armaDispo[i].n << ", " << armaDispo[i].HA << ", " << armaDispo[i].sup << ", " << armaDispo[i].inf << endl;
            };

            vector<BAparoi::arma> getArmaCoupe(string coupeName, coteParoi cote) {
                // auto isCoupe = [coupeName]( const arma &i ) { return i.coupeName == coupeName; };
                // auto n = count_if( armaDispo.begin(), armaDispo.end(), isCoupe );
                // vector<arma> armaList;
                // armaList.reserve( n );  // Requests that the vector capacity be at least enough to contain n elements.
                // // cout << armaList << endl;
                // copy_if( armaDispo.begin(), armaDispo.end(), back_inserter( armaList ), armaList );

                vector<BAparoi::arma> armaList;
                for (auto& a: armaDispo) {
                    if ((a.coupeName == coupeName) & (a.cote == cote)) {
                        armaList.push_back(a);
                    }
                }
                    
                return armaList;

            };
            
            void showArma(vector<BAparoi::arma> armaList) {
                for (auto& armaLine: armaList)
                    cout << armaLine.coupeName << ", " << armaLine.cote << ", " << armaLine.n << ", " << armaLine.HA << ", " << armaLine.sup << ", " << armaLine.inf << endl;
            };


        private:
            coteParoi getCote(string cote) {
                coteParoi cote1 = fouille;
                if (cote == "F") {
                    cote1 = fouille;
                } else if (cote == "T") {
                    cote1 = terre;
                } else {
                    cout << "Côté \"" << cote << "\" n'existe pas" << endl;
                }
                return cote1;
            };
    };

void BAparoi::calculArmaELU(map<string, float> inputData) {

        map<string, float> outputData;
        float d = inputData["h"] - inputData["enrobage"]/100.0;   // m
        float fcd = inputData["fck"]/1.5;       // MPa
        float fyd = inputData["fyk"]/1.15;      // MPa
        float muybu = inputData["MEd"]/inputData["b"]/d/d/fcd/1000.0;
        
        if (muybu > 0.25) {
            outputData["A"] = 10000.0;
        } else {
            float alpha = 1.25*(1.0 - sqrt(1.0 - 2.0*muybu));
            float z = d*(1.0 - 0.4*alpha);
            outputData["A"] = inputData["MEd"]/z/fyd*10.0;  // cm2
        };
        outputData["d"] = d;
        outputData["fcd"] = fcd;
        outputData["fyd"] = fyd;
        outputData["muybu"] = muybu;
        mapF::printMap(outputData);
    };
