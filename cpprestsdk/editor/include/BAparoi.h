#ifndef BAPAROI_H // include guard
#define BAPAROI_H

#include <iostream>
#include <vector>
#include <string>

// #include "core.h"
#include "mapF.h"
#include "csv.h"

using namespace std;
// using namespace core;

namespace BAparoi  // namespace declaration
{
    enum coteParoi {fouille, terre};

    class armaDispo;

    void calculArmaELU(map<string, float>);
}

#endif