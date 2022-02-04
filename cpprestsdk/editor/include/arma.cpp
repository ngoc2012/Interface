#include "arma.h"

using namespace std;

float arma::getCoefficientEffectif(float start, float end, float x, float Lr) {
    if ((x >= start) | (x <= end)) {return 0.0;}
    return min((start - x)/Lr, min((x-end)/Lr, (float) 1.0));
}

