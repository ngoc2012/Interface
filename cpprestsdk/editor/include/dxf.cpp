#include "dxf.h"

void dxf::point::rotate(float x0, float y0, float angle) {
    // Translate
    float x1 = x - x0;
    float y1 = y - y0;
    
    // Rotate
    x = x1*cos(angle*PI/180.0) - y1*sin(angle*PI/180.0);
	y = x1*sin(angle*PI/180.0) + y1*cos(angle*PI/180.0);

    // Translate
    x += x0;
    y += y0;
}

void dxf::readDXF::readDXFfile(string fileName) {

    int id = 0;

    vector<string> lines = fileF::textFile2LinesList(fileName);
    vector<string> entityTypes = {"TEXT", "LINE", "LWPOLYLINE", "ARC"};
    
    bool isLineCodeVal = false;
    // vector<dxf::entity> entList;
    string attr = "";
    bool inSection = false;
    bool inEnt = false; 
    bool isBlock = false;
    dxf::entity* currentEnt;
    dxf::point* currentPoint;
    int iLine = 0;

    for(string line : lines) {
        // if (line == "SECTION") {inSection = true;}
            
        if (line == "BLOCKS") {isBlock = true;}
  
        // if (line == "ENDSEC") {
        //     inSection = false;
        //     inEnt = false;
        //     isBlock = false; 
        // }
        
        if (line == "ENDSEC") {
            isBlock = false;
        }

        if (line == "LWPOLYLINE") {
            // cout << line << endl;
        }

        if (stringF::stringInStringVector(line, entityTypes)) {
            entity ent;
            inEnt = true;
            id++;
            ent.id = id;
            // ent.type = stringF::substr(line, 0, int(line.size()) - 1);
            ent.type = line;
            // cout << ent.id << " " << line << endl;
            ent.isBlock = isBlock;
            // currentEnt = &ent;
            entityList.push_back(ent);
        }
        /*
        lineCode = {
            " 10\n":    "x",
            " 20\n":    "y",
            " 30\n":    "z",
            " 11\n":    "x",
            " 21\n":    "y",
            " 31\n":    "z",
            "  8\n":    "layer",
            "  1\n":    "txt",
            " 40\n":    "radius",
            " 50\n":    "startAngle",
            " 51\n":    "endAngle",
        }
        */
        
        if (inEnt & ((line == " 10") | (line == " 11"))) {
            dxf::point newPoint;
            // currentPoint = &newPoint;
            currentEnt = &entityList[int(entityList.size()) - 1];
            newPoint.x = stof(lines[iLine+1]);
            currentEnt->points.push_back(newPoint);
        }

        if (inEnt & ((line == " 20") | (line == " 21"))) {
            currentEnt = &entityList[int(entityList.size()) - 1];
            currentPoint = &currentEnt->points[int(currentEnt->points.size()) - 1];
            currentPoint->y = stof(lines[iLine+1]);
        }

        if (inEnt & (line == "  8")) {
            currentEnt = &entityList[int(entityList.size()) - 1];
            currentEnt->layer = lines[iLine+1];
        }

        if (inEnt & (line == "  1")) {
            currentEnt = &entityList[int(entityList.size()) - 1];
            currentEnt->txt = lines[iLine+1];
        }

        if (inEnt & (line == " 40")) {
            currentEnt = &entityList[int(entityList.size()) - 1];
            currentEnt->radius = stof(lines[iLine+1]);
        }

        if (line == "  0") {inEnt = false;}   
        
        iLine++;
    }
    // if (ent != nullptr) {delete ent;}
    // return entList;
};

void dxf::readDXF::showDXFentities() {
    // struct entity {
    //     int id = 0;
    //     vector<point> points;   // List points of LINE and POLYLINE, p[0] center of a ARC entity
    //     string layer = "";
    //     string type = "";
    //     bool isBlock = false;
    //     string txt = "";
    //     float radius, startAngle, endAngle;
    // };
    cout << "entityList.size() :" << entityList.size() << endl;
    for (auto ent: entityList) {
        cout << "id: "<<  ent.id << ", layer: "<<  ent.layer << ", type: "<<  ent.type << ", block?: "<<  ent.isBlock << ", p.size(): "<<  ent.points.size() << ", p[0].x: "<<  ent.points[0].x << ", p[0].y: "<<  ent.points[0].y << endl;
    }
}
