#include "geotech.h"

map<geotech::typeFondation, int> geotech::classeFondation    = {{FS, 1}, {FB, 1}, {barrette, 1}, {FTP, 1}, {FTR, 1}, {FTC, 2}, {TER3, 9},  {M1, 11}, {M2, 11}, {MIGU, 8},  {MIRS, 8}};
map<geotech::typeFondation, int> geotech::categorieFondation = {{FS, 1}, {FB, 2}, {barrette, 2}, {FTP, 3}, {FTR, 4}, {FTC, 6}, {TER3, 21}, {M1, 17}, {M2, 18}, {MIGU, 19}, {MIRS, 20}};

geotech::typeFondation geotech::str2typeFondation(string name) {
    if (name == "FS") {return geotech::typeFondation::FS;}
    if (name == "FB") {return geotech::typeFondation::FB;}
    if (name == "barrette") {return geotech::typeFondation::barrette;}
    if (name == "FTP") {return geotech::typeFondation::FTP;}
    if (name == "FTR") {return geotech::typeFondation::FTR;}
    if (name == "FTC") {return geotech::typeFondation::FTC;}
    if (name == "TER3") {return geotech::typeFondation::TER3;}
    if (name == "M1") {return geotech::typeFondation::M1;}
    if (name == "M2") {return geotech::typeFondation::M2;}
    if (name == "MIGU") {return geotech::typeFondation::MIGU;}
    if (name == "MIRS") {return geotech::typeFondation::MIRS;}
    cout << "Type fondation : " << name << " does not exist" << endl;
    return geotech::typeFondation::FB;
}

// class 1-9, 1bis (10)
vector<map<geotech::typeSol, float>> geotech::kpMaxTable = {
    {{geotech::argileLimons, 1.15}, {geotech::sableGrave, 1.10}, {geotech::craie, 1.45}, {geotech::marnesCalcaireMarneux, 1.45}, {geotech::rocheAltereeFragmentee, 1.45}}, // Classe 1
    {{geotech::argileLimons, 1.30}, {geotech::sableGrave, 1.65}, {geotech::craie, 1.60}, {geotech::marnesCalcaireMarneux, 1.60}, {geotech::rocheAltereeFragmentee, 2.00}}, // Classe 2
    {{geotech::argileLimons, 1.55}, {geotech::sableGrave, 3.20}, {geotech::craie, 2.35}, {geotech::marnesCalcaireMarneux, 2.10}, {geotech::rocheAltereeFragmentee, 2.10}}, // Classe 3
    {{geotech::argileLimons, 1.35}, {geotech::sableGrave, 3.10}, {geotech::craie, 2.30}, {geotech::marnesCalcaireMarneux, 2.30}, {geotech::rocheAltereeFragmentee, 2.30}}, // Classe 4
    {{geotech::argileLimons, 1.00}, {geotech::sableGrave, 1.90}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.20}}, // Classe 5
    {{geotech::argileLimons, 1.20}, {geotech::sableGrave, 3.10}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 2.20}, {geotech::rocheAltereeFragmentee, 1.50}}, // Classe 6
    {{geotech::argileLimons, 1.00}, {geotech::sableGrave, 1.00}, {geotech::craie, 1.00}, {geotech::marnesCalcaireMarneux, 1.00}, {geotech::rocheAltereeFragmentee, 1.20}}, // Classe 7
    {{geotech::argileLimons, 1.15}, {geotech::sableGrave, 1.10}, {geotech::craie, 1.45}, {geotech::marnesCalcaireMarneux, 1.45}, {geotech::rocheAltereeFragmentee, 1.45}}, // Classe 8
    {{geotech::argileLimons, 1.40}, {geotech::sableGrave, 1.95}, {geotech::craie, 1.60}, {geotech::marnesCalcaireMarneux, 1.60}, {geotech::rocheAltereeFragmentee, 2.00}}, // Classe 9
    {{geotech::argileLimons, 1.15}, {geotech::sableGrave, 1.10}, {geotech::craie, 1.45}, {geotech::marnesCalcaireMarneux, 1.45}, {geotech::rocheAltereeFragmentee, 1.45}}, // Classe 10 (1bis)
};

// categorie 1-21
vector<map<geotech::typeSol, float>> geotech::alphaPieuSol = {
    {{geotech::argileLimons, 1.10},   {geotech::sableGrave, 1.00}, {geotech::craie, 1.80}, {geotech::marnesCalcaireMarneux, 1.50}, {geotech::rocheAltereeFragmentee, 1.60}},
    {{geotech::argileLimons, 1.25},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.80}, {geotech::marnesCalcaireMarneux, 1.50}, {geotech::rocheAltereeFragmentee, 1.60}},
    {{geotech::argileLimons, 0.70},   {geotech::sableGrave, 0.60}, {geotech::craie, 0.50}, {geotech::marnesCalcaireMarneux, 0.90}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 1.25},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 1.30},   {geotech::sableGrave, 0.00}, {geotech::craie, 0.00}, {geotech::marnesCalcaireMarneux, 0.00}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 1.50},   {geotech::sableGrave, 1.80}, {geotech::craie, 2.10}, {geotech::marnesCalcaireMarneux, 1.60}, {geotech::rocheAltereeFragmentee, 1.60}},
    {{geotech::argileLimons, 1.90},   {geotech::sableGrave, 2.10}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.70}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 0.60},   {geotech::sableGrave, 0.60}, {geotech::craie, 1.00}, {geotech::marnesCalcaireMarneux, 0.70}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 1.10},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.00}, {geotech::marnesCalcaireMarneux, 0.90}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 2.10}, {geotech::craie, 1.90}, {geotech::marnesCalcaireMarneux, 1.60}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 1.20},   {geotech::sableGrave, 1.40}, {geotech::craie, 2.10}, {geotech::marnesCalcaireMarneux, 1.00}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 0.80},   {geotech::sableGrave, 1.20}, {geotech::craie, 0.40}, {geotech::marnesCalcaireMarneux, 0.90}, {geotech::rocheAltereeFragmentee, 0.00}},
    {{geotech::argileLimons, 1.20},   {geotech::sableGrave, 0.70}, {geotech::craie, 0.50}, {geotech::marnesCalcaireMarneux, 1.00}, {geotech::rocheAltereeFragmentee, 1.00}},
    {{geotech::argileLimons, 1.10},   {geotech::sableGrave, 1.00}, {geotech::craie, 0.40}, {geotech::marnesCalcaireMarneux, 1.00}, {geotech::rocheAltereeFragmentee, 0.90}},
    {{geotech::argileLimons, 2.70},   {geotech::sableGrave, 2.90}, {geotech::craie, 2.40}, {geotech::marnesCalcaireMarneux, 2.40}, {geotech::rocheAltereeFragmentee, 2.40}},
    {{geotech::argileLimons, 0.90},   {geotech::sableGrave, 0.80}, {geotech::craie, 0.40}, {geotech::marnesCalcaireMarneux, 1.20}, {geotech::rocheAltereeFragmentee, 1.20}},
    {{geotech::argileLimons, 1.10},   {geotech::sableGrave, 1.00}, {geotech::craie, 1.80}, {geotech::marnesCalcaireMarneux, 1.50}, {geotech::rocheAltereeFragmentee, 1.60}},
    {{geotech::argileLimons, 1.10},   {geotech::sableGrave, 1.00}, {geotech::craie, 1.80}, {geotech::marnesCalcaireMarneux, 1.50}, {geotech::rocheAltereeFragmentee, 1.60}},
    {{geotech::argileLimons, 2.70},   {geotech::sableGrave, 2.90}, {geotech::craie, 2.40}, {geotech::marnesCalcaireMarneux, 2.40}, {geotech::rocheAltereeFragmentee, 2.40}},
    {{geotech::argileLimons, 3.40},   {geotech::sableGrave, 3.80}, {geotech::craie, 3.10}, {geotech::marnesCalcaireMarneux, 3.10}, {geotech::rocheAltereeFragmentee, 3.10}},
    {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 2.10}, {geotech::craie, 2.10}, {geotech::marnesCalcaireMarneux, 1.60}, {geotech::rocheAltereeFragmentee, 1.60}}

};

vector<map<geotech::typeSol, float>> geotech::qsMaxTable = {
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 90 }, {geotech::craie, 200}, {geotech::marnesCalcaireMarneux, 170}, {geotech::rocheAltereeFragmentee, 200}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 90 }, {geotech::craie, 200}, {geotech::marnesCalcaireMarneux, 170}, {geotech::rocheAltereeFragmentee, 200}},
    {{geotech::argileLimons, 50 },   {geotech::sableGrave, 50 }, {geotech::craie, 50 }, {geotech::marnesCalcaireMarneux, 90 }, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 90 }, {geotech::craie, 170}, {geotech::marnesCalcaireMarneux, 170}, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 0.0}, {geotech::craie, 0.0}, {geotech::marnesCalcaireMarneux, 0.0}, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 170}, {geotech::craie, 200}, {geotech::marnesCalcaireMarneux, 200}, {geotech::rocheAltereeFragmentee, 200}},
    {{geotech::argileLimons, 130},   {geotech::sableGrave, 200}, {geotech::craie, 170}, {geotech::marnesCalcaireMarneux, 170}, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 50 },   {geotech::sableGrave, 90 }, {geotech::craie, 90 }, {geotech::marnesCalcaireMarneux, 90 }, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 130},   {geotech::sableGrave, 130}, {geotech::craie, 90 }, {geotech::marnesCalcaireMarneux, 90 }, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 170},   {geotech::sableGrave, 260}, {geotech::craie, 200}, {geotech::marnesCalcaireMarneux, 200}, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 130}, {geotech::craie, 260}, {geotech::marnesCalcaireMarneux, 200}, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 90 }, {geotech::craie, 50 }, {geotech::marnesCalcaireMarneux, 90 }, {geotech::rocheAltereeFragmentee, 0.0}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 50 }, {geotech::craie, 50 }, {geotech::marnesCalcaireMarneux, 90 }, {geotech::rocheAltereeFragmentee, 90 }},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 130}, {geotech::craie, 50 }, {geotech::marnesCalcaireMarneux, 90 }, {geotech::rocheAltereeFragmentee, 90 }},
    {{geotech::argileLimons, 200},   {geotech::sableGrave, 380}, {geotech::craie, 320}, {geotech::marnesCalcaireMarneux, 320}, {geotech::rocheAltereeFragmentee, 320}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 50 }, {geotech::craie, 50 }, {geotech::marnesCalcaireMarneux, 90 }, {geotech::rocheAltereeFragmentee, 90 }},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 90 }, {geotech::craie, 200}, {geotech::marnesCalcaireMarneux, 170}, {geotech::rocheAltereeFragmentee, 200}},
    {{geotech::argileLimons, 90 },   {geotech::sableGrave, 90 }, {geotech::craie, 200}, {geotech::marnesCalcaireMarneux, 170}, {geotech::rocheAltereeFragmentee, 200}},
    {{geotech::argileLimons, 200},   {geotech::sableGrave, 380}, {geotech::craie, 320}, {geotech::marnesCalcaireMarneux, 320}, {geotech::rocheAltereeFragmentee, 320}},
    {{geotech::argileLimons, 200},   {geotech::sableGrave, 440}, {geotech::craie, 440}, {geotech::marnesCalcaireMarneux, 440}, {geotech::rocheAltereeFragmentee, 500}},
    {{geotech::argileLimons, 100},   {geotech::sableGrave, 200}, {geotech::craie, 200}, {geotech::marnesCalcaireMarneux, 200}, {geotech::rocheAltereeFragmentee, 200}}

};

map<geotech::approche, vector<map<geotech::typeSol, float>>> geotech::R1 = {
    {geotech::approche::COMP, {
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.40}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.40}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.40}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.40}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 2.00}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.40}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.40}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.15},   {geotech::sableGrave, 1.15}, {geotech::craie, 1.45}, {geotech::marnesCalcaireMarneux, 1.15}, {geotech::rocheAltereeFragmentee, 1.15}}
    }},
    {geotech::approche::TRAC, {
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.70}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.70}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.70}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.70}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.70}},
        {{geotech::argileLimons, 1.40},   {geotech::sableGrave, 1.40}, {geotech::craie, 1.40}, {geotech::marnesCalcaireMarneux, 1.40}, {geotech::rocheAltereeFragmentee, 1.40}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.70}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.70}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.70}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.70}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.70}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.70}},
        {{geotech::argileLimons, 2.00},   {geotech::sableGrave, 1.70}, {geotech::craie, 2.00}, {geotech::marnesCalcaireMarneux, 2.00}, {geotech::rocheAltereeFragmentee, 1.70}},
        {{geotech::argileLimons, 1.25},   {geotech::sableGrave, 1.25}, {geotech::craie, 1.75}, {geotech::marnesCalcaireMarneux, 1.45}, {geotech::rocheAltereeFragmentee, 1.45}}
    }}
};

map<int, float> CmaxList = {{1, 35.0}, {2, 30.0}, {3, 35.0}, {4, 35.0}, {9, 4.0}};
map<int, float> k1List = {{1, 1.3}, {2, 1.35}, {3, 1.3}, {4, 1.3}, {9, 1.15}};

map<geotech::typeSol, float> qsA = {{geotech::argileLimons, 0.003},   {geotech::sableGrave, 0.010}, {geotech::craie, 0.007}, {geotech::marnesCalcaireMarneux, 0.008}, {geotech::rocheAltereeFragmentee, 0.010}};
map<geotech::typeSol, float> qsB = {{geotech::argileLimons, 0.040},   {geotech::sableGrave, 0.060}, {geotech::craie, 0.070}, {geotech::marnesCalcaireMarneux, 0.080}, {geotech::rocheAltereeFragmentee, 0.080}};
map<geotech::typeSol, float> qsC = {{geotech::argileLimons, 3.500},   {geotech::sableGrave, 1.200}, {geotech::craie, 1.300}, {geotech::marnesCalcaireMarneux, 3.000}, {geotech::rocheAltereeFragmentee, 3.000}};

void geotech::couche::getKpMax(geotech::typeFondation t) {
    kpMax[t] = kpMaxTable[classeFondation[t]][typeSol];
}

void geotech::couche::getQsMax(geotech::typeFondation t) {
    qsMax[t] = qsMaxTable[categorieFondation[t]][typeSol];
}

// self.alphaPieuSol = alphaPieuSol[pieu.categorie-1][self.typeSol]
//             self.qsMax = qsMax[pieu.categorie-1][self.typeSol]
//             self.qs = round(self.qsCoef*min(self.qsMax, self.alphaPieuSol*self.fsol)*10.0)/10.0  # (kPa)
//             # print("couche.py", self.qsCoef, self.qsMax, self.alphaPieuSol, self.fsol, self.qs)

//             # * ==================================================================================
//             # TODO: Facteur de portance pressiométrique en tenant en compte des clauses de l'Article A.10 (§ F.4.2 (1))
//             kpMax = importJson("C:/Users/mnnguyen/Documents/Gcpm/codes/Geotech/" + 'kpMax.json')
//             # classe = typePieu[pieu.typeFondation]['classe']
//             if pieu.classe == "1bis":
//                 self.kpMax = kpMax[8][self.typeSol]
//             else:
//                 self.kpMax = kpMax[pieu.classe-1][self.typeSol]

geotech::typeSol geotech::getTypeSol(int i) {
    switch (i) {
        case 1:
            return typeSol::argileLimons;
        case 2:
            return typeSol::sableGrave;
        case 3:
            return typeSol::craie;
        case 4:
            return typeSol::marnesCalcaireMarneux;
        case 5:
            return typeSol::rocheAltereeFragmentee;
        default:
            exit(0);
    };
}