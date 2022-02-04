#ifndef TIRANT_H // include guard
#define TIRANT_H

// #include <iostream>
// #include <string>
#include <vector>
// #include <math.h>
#include <map>
// #include <set>
// #include <sstream>
// #include <iomanip> // setprecision

// // #include <nlohmann/json.hpp>
// // using json = nlohmann::json;

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

namespace tirant  // namespace declaration
{
    enum solTA95 {AL, SG, MC, R};
    enum forage {IRS, IGU};

    struct plqs {
        vector<float> pl;
        map<forage, vector<float>> qs;
    };

    map<solTA95, plqs> qsTA95;

    qsTA95[AL].pl = {0.075,   0.2,    0.3,    0.4,    0.5,    0.6,    0.7,    0.8,    0.9,    1,      1.1,    1.2,    1.3,    1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4};

//     qsTA95 = {
//     "AL": {
//         "pl": [0.075,   0.2,    0.3,    0.4,    0.5,    0.6,    0.7,    0.8,    0.9,    1,      1.1,    1.2,    1.3,    1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4],
//         "IRS": [0.03,   0.06,   0.09,   0.11,   0.13,   0.141,  0.155,  0.165,  0.175,  0.185,  0.192,  0.2,    0.21,   0.22,0.225,0.235,0.24,0.25,0.258,0.262,0.27,0.28,0.285,0.29],
//         "IGU": [0.018,  0.03,   0.04,   0.05,   0.06,   0.07,   0.08,   0.085,  0.091,  0.1,    0.105,  0.11,   0.115,  0.122,0.125,0.132,0.138,0.145,0.15,0.155,0.162,0.165,0.175,0.18]
//     },
//     "SG": {
//         "pl": [0.25,6.75],
//         "IRS": [0.075,0.73],
//         "IGU": [0.025,0.68]
//     },
//     "MC": {
//         "pl": [1.0,8.0],
//         "IRS": [0.2,0.7],
//         "IGU": [0.15,0.51]
//     },
//     "R": {
//         "pl": [1.0,10.0],
//         "IRS": [0.2,1.2],
//         "IGU": [0.16,0.98]
//     },
// }

// alphaTA95 = {
//     "IRS": {
//         "Graves": 1.8,
// 		"Graves-Sableuses": 1.6,
//         "Sables-Graveleux": 1.5,
//         "Sables": 1.4,
//         "Limons": 1.4,
//         "Argiles": 1.8,
//         "Marnes-Craie": 1.8,
//         "Rocher": 1.2
//     },
//     "IGU": {
//         "Graves": 1.3,
// 		"Graves-Sableuses": 1.2,
//         "Sables-Graveleux": 1.2,
//         "Sables": 1.1,
//         "Limons": 1.1,
//         "Argiles": 1.2,
//         "Marnes-Craie": 1.1,
//         "Rocher": 1.1
//     }
// }

// typeQs = {
//     "Graves": "SG",
// 	"Graves-Sableuses": "SG",
//     "Sables-Graveleux": "SG",
//     "Sables": "SG",
//     "Limons": "AL",
//     "Argiles": "AL",
//     "Marnes-Craie": "MC",
//     "Rocher": "R"
// }

// def tirant(rapport, paramTirant, typeTirant, diaForage):
    
//     import math
//     Llibre = paramTirant['Lu'] - paramTirant['Ls']/2
//     Ltotal = paramTirant['Lu'] + paramTirant['Ls']/2
//     niveauSup = paramTirant['niveau'] - Llibre*math.sin(math.radians(paramTirant['angle']))
//     niveauInf = paramTirant['niveau'] - Ltotal*math.sin(math.radians(paramTirant['angle']))
//     # print(paramTirant['niveau'], niveauSup, niveauInf)

//     qsTotal = {}

//     from Core.Table.table import rowInter
//     rapport0 = []
//     for key in rapport.keys():
//         # print(key)
//         qsTotal[key] = 0.0
//         couches = []
//         for couche in rapport[key]:
//             # print(couche.nom, couche.sup, couche.typeTA95, couche.pl, qsTA95[typeQs[couche.typeTA95]]["pl"])
//             h = max(min(niveauSup, couche.sup) - max(niveauInf, couche.inf), 0.0)
//             if h > 0.0:
//                 L = h/math.sin(math.radians(paramTirant['angle']))
//                 alpha = alphaTA95[typeTirant][couche.typeTA95]
//                 # print(couche.nom, couche.qsTA95)
//                 if couche.qsTA95 == None:
//                     qs1 = "Inconnu"
//                     qs = rowInter(couche.pl, qsTA95[typeQs[couche.typeTA95]]["pl"], qsTA95[typeQs[couche.typeTA95]][typeTirant])
//                     qsRetenu = qs
//                 else:
//                     qs1 = couche.qsTA95*1000.0
//                     qs = couche.qsTA95
//                     qsRetenu = min(qs, couche.qsTA95)
                
//                 qsTotal[key] += alpha*math.pi*diaForage*L*qsRetenu*100.0/2.0

//                 couches.append({
//                     'nom': couche.nom,
//                     'type': couche.typeTA95,
//                     'alpha': alpha,
//                     'pl': couche.pl,
//                     'qs': round(rowInter(couche.pl, qsTA95[typeQs[couche.typeTA95]]["pl"], qsTA95[typeQs[couche.typeTA95]][typeTirant])*1000.0*10.0)/10.0,
//                     'qs1': qs1,
//                     'qsRetenu': round(qsRetenu*1000.0*10.0)/10.0,
//                     'L': round(L*100.0)/100.0,
//                     'qs0': round(alpha*math.pi*diaForage*L*qs*100.0/2.0*10.0)/10.0
//                     })
//                 # print(couche.sup, couche.inf, h, L, couche.typeTA95, alpha, couche.pl, qs)

//         rapport0.append(couches)

//     return min([qsTotal[key] for key in qsTotal.keys()]), rapport0

    

}

#endif