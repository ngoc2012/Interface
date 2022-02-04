#ifndef DXF_H // include guard
#define DXF_H

#include <iostream>
#include <vector>
#include <string>
// #include <regex>
#include <math.h>

#include "const.h"
#include "fileF.h"
#include "stringF.h"
#include "dxflib/dl_dxf.h"

using namespace std;

namespace dxf  // namespace declaration
{
    enum typeEntity {TEXT, LINE, LWPOLYLINE, ARC};

    struct point {
        float x, y;
        point(){};
        point(float x0, float y0) {
            x = x0;
            y = y0;
        };
        void rotate(float x0, float y0, float angle);
    };

    struct entity {
        int id = 0;
        vector<point> points;   // List points of LINE and POLYLINE, p[0] center of a ARC entity
        string layer = "";
        string type = "";
        bool isBlock = false;
        string txt = "";
        float radius, startAngle, endAngle;
    };
    
    struct readDXF {
        vector<entity> entityList;
        void showDXFentities();
        void readDXFfile(string filename);
    };

    struct layer {
        string name; int color; string linetype = "CONTINUOUS"; 
    };
}

#endif