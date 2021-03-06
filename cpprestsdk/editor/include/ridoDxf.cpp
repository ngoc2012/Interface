#include "const.h"
#include "rido.h"

void writeClosedPolyline(DL_Dxf& dxf, DL_WriterA& dw, vector<rido::coor>& polyline, string layer) {
    int i = 0;
    for (auto point: polyline) {
            // DL_LineData lineData(
            //     pan.start.x, pan.start.y, 0,
            //     pan.end.x, pan.end.y, 0);
            // dxf.writeLine(*dw, lineData, attributes1);
            
            if (i == (polyline.size() - 1)) {
                ;
                dxf.writeLine(
                    dw,
                    DL_LineData (
                        polyline[i].x, polyline[i].y, 0,
                        polyline[0].x, polyline[0].y, 0),
                    DL_Attributes (layer, 256, -1, -1, "BYLAYER")
                );
            } else {
                dxf.writeLine(
                    dw,
                    DL_LineData (
                        point.x, point.y, 0,
                        polyline[i+1].x, polyline[i+1].y, 0),
                    DL_Attributes (layer, 256, -1, -1, "BYLAYER")
                );
            }
            
            i++;
        }
}

void writeClosedPolylineVector(DL_Dxf& dxf, DL_WriterA& dw, vector<float> polyline, string layer) {
    int length = polyline.size()/2;
    for (int i=0; i<length; i++) {
        if (i == 0) {
            ;
            dxf.writeLine(
                dw,
                DL_LineData (
                    polyline[length*2-2], polyline[length*2-1], 0,
                    polyline[0], polyline[1], 0),
                DL_Attributes (layer, 256, -1, -1, "BYLAYER")
            );
        } else {
            dxf.writeLine(
                dw,
                DL_LineData (
                    polyline[2*i-2], polyline[2*i-1], 0,
                    polyline[2*i], polyline[2*i+1], 0),
                DL_Attributes (layer, 256, -1, -1, "BYLAYER")
            );
        }
    }
}

void writeDxfHeader(DL_Dxf& dxf, DL_WriterA& dw) {
    // section header:
    dxf.writeHeader(dw);
    dw.sectionEnd();
    // section tables:
    dw.sectionTables();
    // VPORT:
    dxf.writeVPort(dw);

    // // VIEW:
    // dxf.writeView(dw);

    // // UCS:
    // dxf.writeUcs(dw);

    // // APPID:
    // dw.tableAppid(1);
    // dxf.writeAppid(dw, "ACAD");
    // dw.tableEnd();

    // // DIMSTYLE:
    // dxf.writeDimStyle(dw, 2.5, 0.625, 0.625, 0.625, 2.5);

    // // BLOCK_RECORD:
    // dxf.writeBlockRecord(dw);
    // dw.tableEnd();

    // dw.sectionEnd();
 
    // // BLOCK:
    // dw.sectionBlocks();
    // dxf.writeBlock(dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    // dxf.writeEndBlock(dw, "*Model_Space");
    // dxf.writeBlock(dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    // dxf.writeEndBlock(dw, "*Paper_Space");
    // dxf.writeBlock(dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    // dxf.writeEndBlock(dw, "*Paper_Space0");
    // dw.sectionEnd();
}

void writeDxfHeaderEnd(DL_Dxf& dxf, DL_WriterA& dw) {
    // // section header:
    // dxf.writeHeader(dw);
    // dw.sectionEnd();
    // // section tables:
    // dw.sectionTables();
    // // VPORT:
    // dxf.writeVPort(dw);

    // VIEW:
    dxf.writeView(dw);

    // UCS:
    dxf.writeUcs(dw);

    // APPID:
    dw.tableAppid(1);
    dxf.writeAppid(dw, "ACAD");
    dw.tableEnd();

    // DIMSTYLE:
    dxf.writeDimStyle(dw, 2.5, 0.625, 0.625, 0.625, 2.5);

    // BLOCK_RECORD:
    dxf.writeBlockRecord(dw);
    dw.tableEnd();

    dw.sectionEnd();
 
    // BLOCK:
    dw.sectionBlocks();
    dxf.writeBlock(dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf.writeEndBlock(dw, "*Model_Space");
    dxf.writeBlock(dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf.writeEndBlock(dw, "*Paper_Space");
    dxf.writeBlock(dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf.writeEndBlock(dw, "*Paper_Space0");
    dw.sectionEnd();
}

void writeDxfLineType(DL_Dxf& dxf, DL_WriterA& dw) {
    
    // LTYPE:
    /*
    DL_LinetypeData(
        const std::string& name,
        const std::string& description,
        int flags,
        int numberOfDashes,
        double patternLength,
        double* pattern = NULL
        )
    */
    dw.tableLinetypes(4);
    dxf.writeLinetype(dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
    dxf.writeLinetype(dw, DL_LinetypeData("BYLAYER", "", 0, 0, 0.0));
    dxf.writeLinetype(dw, DL_LinetypeData("BYBLOCK", "", 0, 0, 0.0));
    double dashed[2] = {0.05, 0.05};
    dxf.writeLinetype(dw, DL_LinetypeData("DASHED", "Dashed", 0, 2, 0.1, dashed));
    // dxf.writeLinetype(*dw, DL_LinetypeData("CENTER", "Center", 0, 2, 0.7));
    dw.tableEnd();
}

void writeDxfLayers(DL_Dxf& dxf, DL_WriterA& dw, string dxfLayOff, int nLitButonMax) {
    std::cout << "nLitButonMax = " << nLitButonMax << endl;

    json layers = json::array({
        {"AXES", 1, "CENTER"},
        {"COFFRAGE", 5, "DASHED"},
        {"PLATEFORME", 7},
        {"ASpm", 7},
        {"AIpm", 7},
        {"FF", 7},
        {"EDGE_ID", 1},
        {"EDGE_ID0", 5},
        {"EDGE_COUPE", 2},
        {"EDGE_LENGTH", 1},
        {"EDGE_DIM", 7},
        {"PANNEAU", 5, "CONTINUOUS"},
        {"PANNEAU_HATCH_CONSECUTIF", 111},
        {"PANNEAU_HATCH_PRIMAIRE", 30},
        {"PANNEAU_HATCH_FERMETURE", 6},
        {"PANNEAU_LABEL", 7},
        {"PANNEAU_COUPE", 4},
        {"PANNEAU_ID", 3},
        {"PANNEAU_ID0", 3},
        {"PANNEAU_SUP", 7},
        {"PANNEAU_PLATEFORME", 9, "DASHED"},
        {"PANNEAU_DIM", 7},
        {"PANNEAU_LENGTH", 7},
        {"PANNEAU_FICTIF", 9, "DASHED"},
        {"AXE_TIRANT", 1},
        {"TEXT", 7},
        {"BUTON", 7},
        {"BUTON_LABEL", 7},
        {"DALLE", 7},
        {"TIRANT", 7},
        {"TIRANT_ID", 7},
        {"PIEU", 5},
        {"PIEU_HATCH_CONSECUTIF", 111},
        {"PIEU_HATCH_PRIMAIRE", 30},
        {"PIEU_LF", 6},
        {"PIEU_LABEL", 7},
        {"PIEU_DIAMETRE", 4},
        {"PIEU_NOM", 7},
        {"PIEU_ID", 3},
        {"PIEU_DIM", 7},
        {"PIEU_COUPE", 4},
        {"CAGE", 30},
        {"CAGE_LABEL", 7},
        {"CAGE_ID", 7},
        {"COUCHE_PROP", 7},
        {"NAPPE", 5},
        {"EXCAVATION", 7},
        {"CHARGES", 7},
        {"APPUI_PROVISOIRE_AXE", 9, "CENTER"},
        {"APPUI_PROVISOIRE", 7},
        {"APPUI_DEFINITIF", 1, "DASHED"},
        {"COTE", 7}
    });
    
    for (int i=1; i < nLitButonMax+1; ++i){
        layers.push_back(json::array({
            "BUTON_LIT_" + to_string(i), 7, "CONTINUOUS"
        }));
    }

    // cout << layers.dump(4) << endl;

    // LAYER:
    dw.tableLayers(layers.size() + 1);
    bool off = false;
    vector<string> layerOffList = stringF::splitStr(dxfLayOff, ".");
    dxf.writeLayer(
        dw,
        DL_LayerData("0", 0),
        DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS")
    );
    for (auto layer: layers) {
        // cout << layer[0].get<string>() << ", " << layer[1].get<int>() << endl;
        dxf.writeLayer(
            dw,
            DL_LayerData(layer[0].get<string>(), 0, stringF::stringInStringVector(layer[0].get<string>(), layerOffList)),
            DL_Attributes(
                "",
                layer[1].get<int>(),
                100,
                "CONTINUOUS",
                1.0)
        );
    }
    dw.tableEnd();
}

void writeDxfTextStyle(DL_Dxf& dxf, DL_WriterA& dw) {
    json textStyle = json::array({
        {"EDGE_ID_STYLE", "Arial", 1.0},
        {"PANNEAU_LABEL_STYLE", "Arial", 0.8},
        {"PANNEAU_LENGTH_STYLE", "Arial", 0.8},
        {"PANNEAU_COUPE_STYLE", "Arial", 0.8},
        {"PANNEAU_ID_STYLE", "Arial", 0.8},
        {"CAGE_LABEL_STYLE", "Arial", 0.8},
        {"CAGE_ID_STYLE", "Arial", 0.8},
        {"BUTON_LABEL_STYLE", "Arial", 0.8},
        {"PIEU_LABEL_STYLE", "Arial", 0.8},
        {"PIEU_STYLE", "Arial", 0.8},
        {"PIEU_ID_STYLE", "Arial", 0.8},
        {"COTE_STYLE", "Arial", 0.8},
        {"COUCHE_STYLE", "Arial", 0.8},
        {"COUCHE_PROP_STYLE", "Arial", 0.8},
        {"TEXT_STYLE", "Arial", 0.8},
        {"PANNEAU_LABEL_STYLE", "Arial", 0.8}
    });

    // STYLE:
    dw.tableStyle(textStyle.size());
    
    for (auto tStyle: textStyle) {
        DL_StyleData style(
            tStyle[0].get<string>(), // const std::string& name,
            0,      // flags
            tStyle[2].get<double>(),    // double fixedTextHeight,
            1.0,    // double widthFactor,
            0.0,    // double obliqueAngle,
            0,      // int textGenerationFlags,
            tStyle[2].get<double>(),    // double lastHeightUsed,
            tStyle[1].get<string>(),    // const std::string& primaryFontFile,
            tStyle[1].get<string>()     // const std::string& bigFontFile
        );
        style.bold = false;
        style.italic = false;
        dxf.writeStyle(dw, style);
    }

    dw.tableEnd();
}

void rido::paroi::writeDxfEdges(DL_Dxf& dxf, DL_WriterA& dw) {
    // map<string, DL_Attributes> 
    // LINE:
    for (auto edge: edgesList) {
        
        dxf.writeLine(dw, 
            DL_LineData (
                edge.start.x, edge.start.y, 0,
                edge.end.x, edge.end.y, 0),
            DL_Attributes (
                "AXES", // Layer name for this entity or NULL for no layer
                256, // Color number (0..256). 0 = BYBLOCK, 256 = BYLAYER.
                -1,  // color24 24 bit color (0x00RRGGBB, see DXF reference).
                -1, // Line thickness. Defaults to zero. -1 = BYLAYER, -2 = BYBLOCK, -3 = default width
                "BYLAYER"   // const std::string& linetype, Line type name or "BYLAYER" or "BYBLOCK". Defaults to "BYLAYER"
            )
        );
        
        
        dxf.writeText(dw, 
            DL_TextData (
                edge.start.x*0.5 + edge.end.x*0.5, edge.start.y*0.5 + edge.end.y*0.5, 0.0,
                edge.start.x*0.5 + edge.end.x*0.5, edge.start.y*0.5 + edge.end.y*0.5, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                2,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                to_string(edge.id), // Text string
                "EDGE_ID_STYLE",    // Style (font).
                0.0)                // Rotation angle of dimension text away from default orientation.
            , DL_Attributes ("EDGE_ID", 256, -1, -1, "BYLAYER")
        );
        
        
        // msp.add_text(edge['length'], dxfattribs={'layer': 'EDGE_LENGTH', 'style': 'EDGE_ID_STYLE', 'height': 0.2}).set_pos((edge['start']['x']/2.0 + edge['end']['x']/2.0, edge['start']['y']/2.0 + edge['end']['y']/2.0), align='TOP_CENTER')
        
        dxf.writeText(dw, 
            DL_TextData (
                edge.start.x*0.5 + edge.end.x*0.5, edge.start.y*0.5 + edge.end.y*0.5, 0.0,
                edge.start.x*0.5 + edge.end.x*0.5, edge.start.y*0.5 + edge.end.y*0.5, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                3,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                to_string(edge.length),
                "EDGE_ID_STYLE",
                0.0),
            DL_Attributes ("EDGE_LENGTH", 256, -1, -1, "BYLAYER")
        );
    }
}

void rido::paroi::writeDxfTirants(panneau& pan, DL_Dxf& dxf, DL_WriterA& dw, int& tirantId) {
    // polyTirants = []
	// for i, tir in enumerate(tirants):
	// 	poly = []
	// 	nTirants = math.ceil(pan.length/tir['espacement'])
	// 	cos = math.cos(math.radians(tir['angle']))
	// 	for n in range(nTirants):
	// 		poly.append(pan.getTirantLine((2*n+1)/nTirants/2.0, (tir['Lu']-tir['Ls']/2)*cos, tir['Ls']*cos, param['panneautage'][paroiName]['coteTirant']))
	// 	polyTirants.append(poly)

    vector<int> tirantOff;
    if (params.contains("tirantOff")) {
        // std::cout << params["tirantOff"].dump(4) << std::endl;
        tirantOff = stringF::str2IdList(params["tirantOff"].get<string>(), ",");
    }

    vector<tirantPosition> tirantList;
    if (params.contains("tirant")) {
        vector<json> tirantJsonList = csv::csv2JsonList(params["tirant"].get<string>(), 1, 1);
        for (json tirantJson: tirantJsonList) {
            tirantList.push_back(tirantPosition {
                stringF::str2IdList(tirantJson["pan"].get<string>(), ","),
                stringF::str2IdList(tirantJson["lit"].get<string>(), ","),
                tirantJson["pos"].get<int>(),
                tirantJson["dis"].get<float>(),
                tirantJson["angle"].get<float>()
            });

            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["pan"].get<string>(), ","));
            // stringF::printVectorInLine(stringF::str2IdList(tirantJson["lit"].get<string>(), ","));
        }
    }

    // map<int, float> tirantMetreL;
    // map<int, int> tirantMetreN;
    // for (int i=3;i<10;i++) {
    //     tirantMetreL.insert(pair<int, float>(i,0.0));
    //     tirantMetreN.insert(pair<int, float>(i,0));
    // }

    float lPanTotal = pan.length;
    for (auto pan0: panneauList) {
        if ((pan0.label == pan.label) & (pan0.id != pan.id)) {
            lPanTotal += pan0.length;
        }
    }

    for (auto appui: coupeList[pan.coupeId].appuiList) {

        if ((appui.type == appuiType::tirant) & (!vectorF::isInVector(tirantOff, pan.id))) {
        // if (appui.type == appuiType::tirant) {
            int nTirants = ceil((lPanTotal - 0.01f)/appui.espacement);
            for (auto pan0: panneauList) {
                if ((pan0.label == pan.label) & (pan0.id != pan.id)) {
                    nTirants -= ceil((pan0.length-0.01f)/appui.espacement);
                }
            }
            nTirants = max(nTirants, 1);
            std::cout << "ridoDxf >> tirants >> " << pan.label << " " << pan.length << " " << lPanTotal << " " << nTirants << std::endl;
            //int nTirants = ceil((pan.length-0.01f)/appui.espacement);
            // tirantMetreL[appui.nToronts] += (appui.Ll + appui.Ls);
            // tirantMetreN[appui.nToronts] += nTirants;

            for (int i=0; i<nTirants; i++) {

                tirantId++;

                float position = (2.0f*i + 1.0f)/nTirants/2.0;
                float angle = 0.0;
                float dis = 0.0;

                for (auto t: tirantList) {
                    if (vectorF::isInVector(t.pan, pan.id) & vectorF::isInVector(t.lit, appui.lit) & ((i+1) == t.pos)) {
                        angle = t.angle;
                        dis = t.dis;
                    }
                }

                // std::cout << "pan.id= " << pan.id << ", appui.lit=" << appui.lit << ", (i+1)=" << (i+1) << ", angle= " << angle << ", dis=" << dis << ", cos(angle*PI/180.0)=" << cos(angle*PI/180.0) << ", sin(angle*PI/180.0)=" << sin(angle*PI/180.0) << std::endl;

                appui.pos = i + 1;

                float startX = pan.start.x + position*(pan.end.x - pan.start.x) + coupeList[pan.coupeId].epaisseur/2.0*pan.perpUnite.x + dis*pan.vectorUnite.x;
                float startY = pan.start.y + position*(pan.end.y - pan.start.y) + coupeList[pan.coupeId].epaisseur/2.0*pan.perpUnite.y + dis*pan.vectorUnite.y;

                dxf::point startS(
                    startX - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.x,
                    startY - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.y
                );

                dxf::point endS(
                    startS.x - appui.Ls*cos(appui.angle*PI/180.0)*pan.perpUnite.x,
                    startS.y - appui.Ls*cos(appui.angle*PI/180.0)*pan.perpUnite.y
                );
                
                // std::cout << startS.x << " " << startS.y << std::endl;

                startS.rotate(startX, startY, angle);
                endS.rotate(startX, startY, angle);

                // std::cout << startS.x << " " << startS.y << std::endl;

                // float startXS = startX - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.x;
                // float startYS = startY - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.y;
                // float endXS = startXS - appui.Ls*cos(appui.angle*PI/180.0)*pan.perpUnite.x;
                // float endYS = startYS - appui.Ls*cos(appui.angle*PI/180.0)*pan.perpUnite.y;

                // float startXSr = 
                // float startXS = startX - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.x*cos(angle*PI/180.0);
                // float startYS = startY - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.y*sin(angle*PI/180.0);
                // float endXS = startX + (startXS - appui.Ls*cos(appui.angle*PI/180.0)*pan.perpUnite.x - startX)*cos(angle*PI/180.0);
                // float endYS = startY + (startYS - appui.Ls*cos(appui.angle*PI/180.0)*pan.perpUnite.y - startY)*sin(angle*PI/180.0);

                // std::cout << (startX - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.x) << " " << appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.x*cos(angle*PI/180.0) << " " << startXS << std::endl;
                // std::cout << (startY - appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.y) << " " << appui.Ll*cos(appui.angle*PI/180.0)*pan.perpUnite.y*sin(angle*PI/180.0) << " " << startYS << std::endl;

                dxf.writeLine(
                    dw,
                    DL_LineData (
                        startX, startY, 0,
                        startS.x, startS.y, 0),
                    DL_Attributes ("TIRANT", 256, -1, -1, "BYLAYER")
                );

                // writeClosedPolylineVector(dxf, dw, {
                //     startS.x - 0.18f*0.5f*pan.vectorUnite.x*cos(angle*PI/180.0f), startS.y - 0.18f*0.5f*pan.vectorUnite.y*sin(angle*PI/180.0f),
                //     startS.x + 0.18f*0.5f*pan.vectorUnite.x*cos(angle*PI/180.0f), startS.y + 0.18f*0.5f*pan.vectorUnite.y*sin(angle*PI/180.0f),
                //     endS.x + 0.18f*0.5f*pan.vectorUnite.x*cos(angle*PI/180.0f), endS.y + 0.18f*0.5f*pan.vectorUnite.y*sin(angle*PI/180.0f),
                //     endS.x - 0.18f*0.5f*pan.vectorUnite.x*cos(angle*PI/180.0f), endS.y - 0.18f*0.5f*pan.vectorUnite.y*sin(angle*PI/180.0f)
                // }, "TIRANT");

                // float x0 = 

                float x0 = pan.vectorUnite.x*cos(angle*PI/180.0) - pan.vectorUnite.y*sin(angle*PI/180.0);
	            float y0 = pan.vectorUnite.x*sin(angle*PI/180.0) + pan.vectorUnite.y*cos(angle*PI/180.0);

                // writeClosedPolylineVector(dxf, dw, {
                //     startS.x - 0.18f*0.5f*pan.vectorUnite.x, startS.y - 0.18f*0.5f*pan.vectorUnite.y,
                //     startS.x + 0.18f*0.5f*pan.vectorUnite.x, startS.y + 0.18f*0.5f*pan.vectorUnite.y,
                //     endS.x + 0.18f*0.5f*pan.vectorUnite.x, endS.y + 0.18f*0.5f*pan.vectorUnite.y,
                //     endS.x - 0.18f*0.5f*pan.vectorUnite.x, endS.y - 0.18f*0.5f*pan.vectorUnite.y
                // }, "TIRANT");

                writeClosedPolylineVector(dxf, dw, {
                    startS.x - 0.18f*0.5f*x0, startS.y - 0.18f*0.5f*y0,
                    startS.x + 0.18f*0.5f*x0, startS.y + 0.18f*0.5f*y0,
                    endS.x + 0.18f*0.5f*x0, endS.y + 0.18f*0.5f*y0,
                    endS.x - 0.18f*0.5f*x0, endS.y - 0.18f*0.5f*y0
                }, "TIRANT");

                dxf.writeText(dw, 
                    DL_TextData (
                        startX*0.5 + endS.x*0.5, startY*0.5 + endS.y*0.5, 0.0,
                        startX*0.5 + endS.x*0.5, startY*0.5 + endS.y*0.5, 0.0,
                        0.5, 1.0,   // double height, double xScaleFactor,
                        0,          // int textGenerationFlags,
                        1,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                        3,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                        to_string(appui.id) + "|" + to_string(appui.pos),
                        "EDGE_ID_STYLE",
                        0.0),
                    DL_Attributes ("TIRANT", 256, -1, -1, "BYLAYER")
                );

                dxf.writeText(dw, 
                    DL_TextData (
                        startX, startY, 0.0,
                        startX, startY, 0.0,
                        0.3, 1.0,   // double height, double xScaleFactor,
                        0,          // int textGenerationFlags,
                        1,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                        3,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                        to_string(tirantId),
                        "EDGE_ID_STYLE",
                        0.0),
                    DL_Attributes ("TIRANT_ID", 256, -1, -1, "BYLAYER")
                );
            }
        }
    }

    // std::cout << "M??tr?? des tirants :" << std::endl;
    // for (auto t: tirantMetreL) {
    //     if (t.second > 0.0) {
    //         std::cout << "Tirants " << t.first << " toronts: Ltotal = " << t.second << "m" << std::endl;
    //     }
    // }
    // for (auto t: tirantMetreN) {
    //     if (t.second > 0.0) {
    //         std::cout << "Tirants " << t.first << " toronts: ntotal = " << t.second << " tirants" << std::endl;
    //     }
    // }
    
    // def getTirantLine(self, pos, Ll, Ls, cote = 1.0):
    // import math
	// 	# print(pos, Ll, Ls)
	// 	startX = self.start.x + pos*(self.end.x -self.start.x) - cote*self.epaisseur/2.0*math.sin(math.radians(self.rotZ))
	// 	startY = self.start.y + pos*(self.end.y -self.start.y) + cote*self.epaisseur/2.0*math.cos(math.radians(self.rotZ))
	// 	sin = math.sin(math.radians(self.rotZ))
	// 	cos = math.cos(math.radians(self.rotZ))
	// 	startXS = startX + cote*Ll*sin
	// 	startYS = startY - cote*Ll*cos
	// 	Llibre = [(startX, startY), (startXS, startYS)]
	// 	unitePan = {'x': (self.end.x - self.start.x)/self.length, 'y': (self.end.y - self.start.y)/self.length}
	// 	RS = 0.1
	// 	Lscellement = [
	// 		(startXS + RS*unitePan['x'], startYS + RS*unitePan['y']), 
	// 		(startXS + RS*unitePan['x'] + cote*Ls*sin, startYS + RS*unitePan['y'] - cote*Ls*cos),
	// 		(startXS - RS*unitePan['x'] + cote*Ls*sin, startYS - RS*unitePan['y'] - cote*Ls*cos),
	// 		(startXS - RS*unitePan['x'], startYS - RS*unitePan['y']),
	// 		(startXS + RS*unitePan['x'], startYS + RS*unitePan['y'])
	// 		]
	// 	return {
	// 		'Llibre': Llibre,
	// 		'Lscellement': Lscellement
	// 	}
    // if pan['id0'] not in annuleTirants:
    //     for lit in pan['polyTirants']:
    //         for tir in lit:
    //             msp.add_lwpolyline(tir['Llibre'], dxfattribs={'layer': 'AXE_TIRANT'})
    //             msp.add_lwpolyline(tir['Lscellement'], dxfattribs={'layer': 'AXE_TIRANT'})

}

void rido::paroi::writeDxfPanneaux(DL_Dxf& dxf, DL_WriterA& dw) {
    
    // int i;
    int tirantId = 0;
    for (auto pan: panneauList) {        
        
        writeDxfTirants(pan, dxf, dw, tirantId);
        writeClosedPolyline(dxf, dw, pan.polyline, "PANNEAU");
        
        dxf.writeText(
            dw,
            DL_TextData (
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification,
                2,          // int vJustification,
                to_string(pan.id),
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("PANNEAU_ID", 256, -1, -1, "BYLAYER")
        );

        dxf.writeText(
            dw,
            DL_TextData (
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification,
                2,          // int vJustification,
                pan.label,
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("PANNEAU_LABEL", 256, -1, -1, "BYLAYER")
        );

        std::ostringstream out;
        out.precision(3);
        out << std::fixed << round(pan.length*1000.0)/1000.0;

        dxf.writeText(
            dw,
            DL_TextData (
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                0.3, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification,
                2,          // int vJustification,
                out.str(),
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("PANNEAU_LENGTH", 256, -1, -1, "BYLAYER")
        );

        dxf.writeText(
            dw,
            DL_TextData (
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                0.3, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification,
                2,          // int vJustification,
                coupeList[pan.coupeId].coupeName,
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("PANNEAU_COUPE", 256, -1, -1, "BYLAYER")
        );
        
    }
}

void rido::paroi::writeDxfCages(DL_Dxf& dxf, DL_WriterA& dw) {
    
    for (auto pan: cageList) {        
        
        writeClosedPolyline(dxf, dw, pan.polyline, "CAGE");
        
        dxf.writeText(
            dw,
            DL_TextData (
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                0.2, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification,
                2,          // int vJustification,
                pan.label,
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("CAGE_LABEL", 256, -1, -1, "BYLAYER")
        );

        std::ostringstream out;
        out.precision(2);
        out << std::fixed << round(pan.length*1000.0)/1000.0;

        dxf.writeText(
            dw,
            DL_TextData (
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                pan.start.x*0.5 + pan.end.x*0.5, pan.start.y*0.5 + pan.end.y*0.5, 0.0,
                0.3, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification,
                2,          // int vJustification,
                // to_string(pan.length),
                out.str(),
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("CAGE_LENGTH", 256, -1, -1, "BYLAYER")
        );
    }
}

void elevationLigneNiveau(DL_Dxf& dxf, DL_WriterA& dw, json params, string p, float startX, string layer) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << round(params[p].get<float>()*100.0)/100.0;

    dxf.writeText(
        dw,
        DL_TextData (
            startX + 0.1, params[p].get<float>() + 0.1f, 0.0,
            startX + 0.1, params[p].get<float>() + 0.1f, 0.0,
            0.2, 1.0,   // double height, double xScaleFactor,
            0,          // int textGenerationFlags,
            0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.),
            1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top,
            p + "=" + out.str(),
            "PANNEAU_ID_STYLE",
            0.0
        ),
        DL_Attributes (layer, 256, -1, -1, "BYLAYER")
    );
}

void rido::paroi::writeDxfElevation(DL_Dxf& dxf, DL_WriterA& dw, vector<int> panIdList) {
    
    // stringF::printVectorInLine(panIdList);
    float startX = 0.0;
    for (auto pan: panneauList) 
        startX = max(startX, max(pan.start.x, pan.end.x));
    startX += 50.0;
    float startY = 0.0;

    // vector<float> startXList;
    // int i;
    for (int i: panIdList) {
        if (i > panneauList.size()) {
            std::cout << "Error: Panneau " << i << " n'existe pas" << std::endl;
            // exit(0);
        } else {
            panneau& pan = panneauList[i-1];
            rido& coupe = coupeList[pan.coupeId];
            pan.elevationStartX = startX + 0.0;
            float endX = startX + pan.length;

            // Panneau
            writeClosedPolylineVector(dxf, dw, {
                startX, coupe.params["AIpmH"],
                endX, coupe.params["AIpmH"],
                endX, coupe.params["ASpm"],
                startX, coupe.params["ASpm"]
            }, "PANNEAU");
            
            elevationLigneNiveau(dxf, dw, coupe.params, "ASpm", startX, "ASpm");
            elevationLigneNiveau(dxf, dw, coupe.params, "AIpmH", startX, "AIpm");

            elevationLigneNiveau(dxf, dw, coupe.params, "plateforme", startX, "PLATEFORME");
            dxf.writeLine(dw, DL_LineData (startX, coupe.params["plateforme"], 0, startX + pan.length, coupe.params["plateforme"], 0), DL_Attributes ("PLATEFORME", 256, -1, -1, "BYLAYER"));

            elevationLigneNiveau(dxf, dw, coupe.params, "FF", startX, "FF");
            dxf.writeLine(dw, DL_LineData (startX, coupe.params["FF"], 0, startX + pan.length, coupe.params["FF"], 0), DL_Attributes ("FF", 256, -1, -1, "BYLAYER"));
            
            // Coupe name
            dxf.writeText(
                dw,
                DL_TextData (
                    startX*0.5 + endX*0.5, coupe.params["ASpm"]*0.5 + coupe.params["AIpm"]*0.5, 0.0,
                    startX*0.5 + endX*0.5, coupe.params["ASpm"]*0.5 + coupe.params["AIpm"]*0.5, 0.0,
                    0.5, 1.0,   // double height, double xScaleFactor,
                    0,          // int textGenerationFlags,
                    1,          // int hJustification,
                    2,          // int vJustification,
                    coupe.coupeName,
                    "PANNEAU_ID_STYLE",
                    0.0
                ),
                DL_Attributes ("PANNEAU_COUPE", 256, -1, -1, "BYLAYER")
            );

            // Couches de sols
            DL_Attributes COUCHE ("COUCHE", 256, -1, -1, "BYLAYER");
            int i = 0;
            for (auto couche: coupe.coucheList) {
                float sup = min(couche.sup, coupe.params["TN"]);
                float inf = couche.inf;
                if (i == (coupe.coucheList.size() - 1)) {
                    inf = min(couche.sup - 5.0, coupe.params["AIpmH"] - 5.0);
                }

                dxf.writeLine(dw, DL_LineData (startX, sup, 0, startX + pan.length, sup, 0), COUCHE);

                dxf.writeText(dw, 
                    DL_TextData (
                        startX + 0.1, sup - 0.1, 0.0,
                        startX + 0.1, sup - 0.1, 0.0,
                        0.2, 1.0,   // double height, double xScaleFactor,
                        0,          // int textGenerationFlags,
                        0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                        3,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                        couche.nom, // Text string
                        "COUCHE_STYLE",    // Style (font).
                        0.0)                // Rotation angle of dimension text away from default orientation.
                    , DL_Attributes ("COUCHE", 256, -1, -1, "BYLAYER")
                );
                
                // ======================== Sous la paroi =========================
                if (sup < coupe.params["AIpmH"]) {
                    dxf.writeLine(dw, DL_LineData (startX, sup, 0, startX + pan.length, sup, 0), COUCHE);
                }
                
                i++;
            }

            // Tirants
            for (auto appui: coupe.appuiList) {
                if (appui.type == appuiType::tirant) {
                    // Platine
                    
                    int nTirants = round((pan.length - 0.01)/appui.espacement);
                    float esp = pan.length/nTirants;

                    // std::cout << pan.coupeId << " " << appui.niveau << " " << appui.espacement << " " << nTirants << " " << esp << std::endl;
                    for (int i=0; i < nTirants; i++) {
                        float tirantX = startX + esp*0.5f + i*esp;
                        // std::cout << tirantX << " " << appui.niveau << std::endl;
                        writeClosedPolylineVector(dxf, dw, {
                            tirantX - 0.25f, appui.niveau - 0.25f,
                            tirantX + 0.25f, appui.niveau - 0.25f,
                            tirantX + 0.25f, appui.niveau + 0.25f,
                            tirantX - 0.25f, appui.niveau + 0.25f
                        }, "TIRANT");
                        dxf.writeCircle(dw, DL_CircleData (
                            tirantX, appui.niveau, 0.0,
                            0.2
                        ), DL_Attributes ("TIRANT", 256, -1, -1, "BYLAYER"));
                        dxf.writeText(
                            dw,
                            DL_TextData (
                                tirantX + 0.1, appui.niveau + 0.1f, 0.0,
                                tirantX + 0.1, appui.niveau + 0.1f, 0.0,
                                0.2, 1.0,   // double height, double xScaleFactor,
                                0,          // int textGenerationFlags,
                                0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.),
                                1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top,
                                // to_string(round(appui.niveau*100.0)/100.0),
                                stringF::ftos(appui.niveau, 2),
                                "PANNEAU_ID_STYLE",
                                0.0
                            ),
                            DL_Attributes ("BUTON", 256, -1, -1, "BYLAYER")
                        );
                    }
                } else if (appui.type == appuiType::dalle) {
                    // Dalle
                    dxf.writeText(
                        dw,
                        DL_TextData (
                            startX + 0.1, appui.niveau + 0.1f, 0.0,
                            startX + 0.1, appui.niveau + 0.1f, 0.0,
                            0.2, 1.0,   // double height, double xScaleFactor,
                            0,          // int textGenerationFlags,
                            0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.),
                            1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top,
                            // "dalle=" + to_string(round(appui.niveau*100.0)/100.0),
                            "dalle=" + stringF::ftos(appui.niveau, 2),
                            "PANNEAU_ID_STYLE",
                            0.0
                        ),
                        DL_Attributes ("DALLE", 256, -1, -1, "BYLAYER")
                    );
                    
                    dxf.writeLine(dw, DL_LineData (startX, appui.niveau, 0, startX + pan.length, appui.niveau, 0), DL_Attributes ("DALLE", 256, -1, -1, "BYLAYER"));
                    dxf.writeLine(dw, DL_LineData (startX, appui.niveau - 0.2, 0, startX + pan.length, appui.niveau - 0.2, 0), DL_Attributes ("DALLE", 256, -1, -1, "BYLAYER"));

                }
            }

            startX += pan.length;
        }
    }

    for (auto but: butonList) {
        // Panneau
        // float butStart, butEnd;
        // float panStart = panneauList[but.startBout.panneauId - 1].elevationStartX;
        // float butEnd = panneauList[but.endBout.panneauId - 1].elevationStartX;

        panneau& startPan = panneauList[but.startBout.panneauId - 1];
        panneau& endPan = panneauList[but.endBout.panneauId - 1];
        // json butJson = *but.butonJson;

        float butStart = startPan.elevationStartX + startPan.length*0.5 + but.butonJson["dis1"].get<float>();
        float butEnd = endPan.elevationStartX + endPan.length*0.5 + but.butonJson["dis2"].get<float>();

        // Platine
        writeClosedPolylineVector(dxf, dw, {
            butStart - 0.5f, but.startBout.niveau - 0.5f,
            butStart + 0.5f, but.startBout.niveau - 0.5f,
            butStart + 0.5f, but.startBout.niveau + 0.5f,
            butStart - 0.5f, but.startBout.niveau + 0.5f
        }, "BUTON");

        writeClosedPolylineVector(dxf, dw, {
            butEnd - 0.5f, but.endBout.niveau - 0.5f,
            butEnd + 0.5f, but.endBout.niveau - 0.5f,
            butEnd + 0.5f, but.endBout.niveau + 0.5f,
            butEnd - 0.5f, but.endBout.niveau + 0.5f
        }, "BUTON");

        // Buton
        dxf.writeCircle(dw, DL_CircleData (
            butStart, but.startBout.niveau, 0.0,
            0.45
        ), DL_Attributes ("BUTON", 256, -1, -1, "BYLAYER"));
        dxf.writeCircle(dw, DL_CircleData (
            butEnd, but.endBout.niveau, 0.0,
            0.45
        ), DL_Attributes ("BUTON", 256, -1, -1, "BYLAYER"));

        dxf.writeText(
            dw,
            DL_TextData (
                butStart + 0.1, but.startBout.niveau + 0.1f, 0.0,
                butStart + 0.1, but.startBout.niveau + 0.1f, 0.0,
                0.2, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.),
                1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top,
                // to_string(round(but.startBout.niveau*100.0)/100.0),
                stringF::ftos(but.startBout.niveau, 2),
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("BUTON", 256, -1, -1, "BYLAYER")
        );
        dxf.writeText(
            dw,
            DL_TextData (
                butEnd + 0.1, but.endBout.niveau + 0.1f, 0.0,
                butEnd + 0.1, but.endBout.niveau + 0.1f, 0.0,
                0.2, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.),
                1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top,
                // to_string(round(but.endBout.niveau*100.0)/100.0),
                stringF::ftos(but.endBout.niveau, 2),
                "PANNEAU_ID_STYLE",
                0.0
            ),
            DL_Attributes ("BUTON", 256, -1, -1, "BYLAYER")
        );

    }
    
}

template<class T>
float crossProduct(T p1, T p2, T p3) {
    // # Line over p1 and p2
        return (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
};

void rido::paroi::writeDxfButons(DL_Dxf& dxf, DL_WriterA& dw) {
    for (auto but: butonList) {
        
        dxf.writeLine(dw,
            DL_LineData (
                but.start.x, but.start.y, 0,
                but.end.x, but.end.y, 0
            ),
            DL_Attributes("BUTON_LIT_1", 256, -1, -1, "BYLAYER"));
        
        // dxf.writeLine(dw,
        //     DL_LineData (
        //         but.start.x, but.start.y, 0,
        //         but.end.x, but.end.y, 0
        //     ),
        //     DL_Attributes("BUTON_LIT_1", 256, -1, -1, "BYLAYER"));

        // vBut1 = {'x': (but['end']['x'] - but['start']['x'])/distance(but), 'y': (but['end']['y'] - but['start']['y'])/distance(but)}

        float t1 = (but.startBout.platine.tp + 70.0)/1000.0;
        float t2 = (but.endBout.platine.tp + 70.0)/1000.0;

        vector<float> start = {
            but.start.x + but.vectorUnite.x*t1/sin(but.startBout.angle*PI/180.0f),
            but.start.y + but.vectorUnite.y*t1/sin(but.startBout.angle*PI/180.0f),
        };
        
        vector<float> end = {
            // but['end']['x'] - vBut1['x']*t2/math.sin(math.radians(but['angle2'])),
            // but['end']['y'] - vBut1['y']*t2/math.sin(math.radians(but['angle2'])),
            but.end.x - but.vectorUnite.x*t2/sin(but.endBout.angle*PI/180.0f),
            but.end.y - but.vectorUnite.y*t2/sin(but.endBout.angle*PI/180.0f),
        };
        
        // # print(start, end)
        // msp.add_lwpolyline([(start['x'], start['y']), (end['x'], end['y'])], dxfattribs={'layer': "0"})

        // vPan1P = {'x': but['vPan1']['y'], 'y': -but['vPan1']['x']}
        // if (vPan1P['x']*vBut1['x'] + vPan1P['y']*vBut1['y']) < 0:
        //     vPan1P = {'x': -but['vPan1']['y'], 'y': but['vPan1']['x']}
        
        // vPan2P = {'x': but['vPan2']['y'], 'y': -but['vPan2']['x']}
        // if (-vPan2P['x']*vBut1['x'] - vPan2P['y']*vBut1['y']) < 0:
        //     vPan2P = {'x': -but['vPan2']['y'], 'y': but['vPan2']['x']}

        // platine1 = [
        //     (start['x'] - but['vPan1']['x']*but['Lpg']/2000.0 - vPan1P['x']*t1, start['y'] - but['vPan1']['y']*but['Lpg']/2000.0 - vPan1P['y']*t1),
        //     (start['x'] - but['vPan1']['x']*but['Lpg']/2000.0, start['y'] - but['vPan1']['y']*but['Lpg']/2000.0),
        //     (start['x'] + but['vPan1']['x']*but['Lpg']/2000.0, start['y'] + but['vPan1']['y']*but['Lpg']/2000.0),
        //     (start['x'] + but['vPan1']['x']*but['Lpg']/2000.0 - vPan1P['x']*t1, start['y'] + but['vPan1']['y']*but['Lpg']/2000.0 - vPan1P['y']*t1),
        // ]

        // vector<float> platine1 = {
        //     (but.start.x - but.vectorUnite.x*but['Lpg']/2000.0 - vPan1P['x']*t1, start['y'] - but['vPan1']['y']*but['Lpg']/2000.0 - vPan1P['y']*t1),
        //     (but.start.x - but.vectorUnite.x*but['Lpg']/2000.0, start['y'] - but['vPan1']['y']*but['Lpg']/2000.0),
        //     (but.start.x + but.vectorUnite.x*but['Lpg']/2000.0, start['y'] + but['vPan1']['y']*but['Lpg']/2000.0),
        //     (but.start.x + but.vectorUnite.x*but['Lpg']/2000.0 - vPan1P['x']*t1, start['y'] + but['vPan1']['y']*but['Lpg']/2000.0 - vPan1P['y']*t1)
        // };

        // mortier1 = [
        //     (start['x'] - but['vPan1']['x']*but['Lpg']/2000.0 - vPan1P['x']*but['tpg']/1000.0, start['y'] - but['vPan1']['y']*but['Lpg']/2000.0 - vPan1P['y']*but['tpg']/1000.0),
        //     (start['x'] + but['vPan1']['x']*but['Lpg']/2000.0 - vPan1P['x']*but['tpg']/1000.0, start['y'] + but['vPan1']['y']*but['Lpg']/2000.0 - vPan1P['y']*but['tpg']/1000.0)
        // ]
        
        // platine2 = [
        //     (end['x'] - but['vPan2']['x']*but['Lpd']/2000.0 - vPan2P['x']*t2, end['y'] - but['vPan2']['y']*but['Lpd']/2000.0 - vPan2P['y']*t2),
        //     (end['x'] - but['vPan2']['x']*but['Lpd']/2000.0, end['y'] - but['vPan2']['y']*but['Lpd']/2000.0),
        //     (end['x'] + but['vPan2']['x']*but['Lpd']/2000.0, end['y'] + but['vPan2']['y']*but['Lpd']/2000.0),
        //     (end['x'] + but['vPan2']['x']*but['Lpd']/2000.0 - vPan2P['x']*t2, end['y'] + but['vPan2']['y']*but['Lpd']/2000.0 - vPan2P['y']*t2),
        // ]

        // mortier2 = [
        //     (end['x'] - but['vPan2']['x']*but['Lpd']/2000.0 - vPan2P['x']*but['tpd']/1000.0, end['y'] - but['vPan2']['y']*but['Lpd']/2000.0 - vPan2P['y']*but['tpd']/1000.0),
        //     (end['x'] + but['vPan2']['x']*but['Lpd']/2000.0 - vPan2P['x']*but['tpd']/1000.0, end['y'] + but['vPan2']['y']*but['Lpd']/2000.0 - vPan2P['y']*but['tpd']/1000.0)
        // ]
        // but.getVectorUnite();
        coor startP1, startP2, endP1, endP2;
        panneau& startPan = panneauList[but.startBout.panneauId-1];
        panneau& endPan = panneauList[but.endBout.panneauId-1];
        startPan.getVectorUnite();
        endPan.getVectorUnite();
        startP1.x = but.start.x - startPan.vectorUnite.x*but.profile.h/2.0f/sin(but.startBout.angle*PI/180.0f);
        startP1.y = but.start.y - startPan.vectorUnite.y*but.profile.h/2.0f/sin(but.startBout.angle*PI/180.0f);
        startP2.x = but.start.x + startPan.vectorUnite.x*but.profile.h/2.0f/sin(but.startBout.angle*PI/180.0f);
        startP2.y = but.start.y + startPan.vectorUnite.y*but.profile.h/2.0f/sin(but.startBout.angle*PI/180.0f);
        endP1.x = but.end.x - endPan.vectorUnite.x*but.profile.h/2.0f/sin(but.endBout.angle*PI/180.0f);
        endP1.y = but.end.y - endPan.vectorUnite.y*but.profile.h/2.0f/sin(but.endBout.angle*PI/180.0f);
        endP2.x = but.end.x + endPan.vectorUnite.x*but.profile.h/2.0f/sin(but.endBout.angle*PI/180.0f);
        endP2.y = but.end.y + endPan.vectorUnite.y*but.profile.h/2.0f/sin(but.endBout.angle*PI/180.0f);
        
        // std::cout << startPan.id << " " << startPan.start.x << " " << startPan.start.y << " " << startPan.end.x << " " << startPan.end.y << " " << startPan.vectorUnite.x << " " << startPan.vectorUnite.y << " " << std::endl;
        // std::cout << endPan.id << " " << endPan.start.x << " " << endPan.start.y << " " << endPan.end.x << " " << endPan.end.y << " " << endPan.vectorUnite.x << " " << endPan.vectorUnite.y << " " << std::endl;

        // startP2 = (
        //     start['x'] + but['vPan1']['x']*but['d']/2000.0/math.sin(math.radians(but['angle1'])),
        //     start['y'] + but['vPan1']['y']*but['d']/2000.0/math.sin(math.radians(but['angle1']))
        // )
        
        // endP1 = (
        //     end['x'] - but['vPan2']['x']*but['d']/2000.0/math.sin(math.radians(but['angle2'])),
        //     end['y'] - but['vPan2']['y']*but['d']/2000.0/math.sin(math.radians(but['angle2']))
        // )
        
        // endP2 = (
        //     end['x'] + but['vPan2']['x']*but['d']/2000.0/math.sin(math.radians(but['angle2'])),
        //     end['y'] + but['vPan2']['y']*but['d']/2000.0/math.sin(math.radians(but['angle2']))
        // )

        // msp.add_lwpolyline(platine1, dxfattribs={'layer': layer2[but['lit']]})
        // msp.add_lwpolyline(platine2, dxfattribs={'layer': layer2[but['lit']]})

        // msp.add_lwpolyline(mortier1, dxfattribs={'layer': layer2[but['lit']]})
        // msp.add_lwpolyline(mortier2, dxfattribs={'layer': layer2[but['lit']]})

        // # Check if startP1 and endP1 are at the same side of the line cross (but['start'], but['end'])
        if ((crossProduct(but.start, but.end, startP1)*crossProduct(but.start, but.end, endP1)) > 0) {

            // std::cout << "1" << std::endl;
            dxf.writeLine(dw,
            DL_LineData (
                startP1.x, startP1.y, 0,
                endP1.x, endP1.y, 0
            ),
            DL_Attributes("BUTON_LIT_1", 256, -1, -1, "BYLAYER"));

            dxf.writeLine(dw,
            DL_LineData (
                startP2.x, startP2.y, 0,
                endP2.x, endP2.y, 0
            ),
            DL_Attributes("BUTON_LIT_1", 256, -1, -1, "BYLAYER"));
        } else {

            // std::cout << "2" << std::endl;
            // std::cout << but.start.x << " " << but.start.y << " " << startP1.x << " " << startP1.y << std::endl;
            // std::cout << but.end.x << " " << but.end.y << " " << endP2.x << " " << endP2.y << std::endl;

            dxf.writeLine(dw,
            DL_LineData (
                startP1.x, startP1.y, 0,
                endP2.x, endP2.y, 0
            ),
            DL_Attributes("BUTON_LIT_1", 256, -1, -1, "BYLAYER"));

            dxf.writeLine(dw,
            DL_LineData (
                startP2.x, startP2.y, 0,
                endP1.x, endP1.y, 0
            ),
            DL_Attributes("BUTON_LIT_1", 256, -1, -1, "BYLAYER"));
        }
            // msp.add_lwpolyline([startP1, endP2], dxfattribs={'layer': layer2[but['lit']]})
            // msp.add_lwpolyline([startP2, endP1], dxfattribs={'layer': layer2[but['lit']]})

        
        dxf.writeText(dw, 
            DL_TextData (
                but.start.x*0.5 + but.end.x*0.5, but.start.y*0.5 + but.end.y*0.5, 0.0,
                but.start.x*0.5 + but.end.x*0.5, but.start.y*0.5 + but.end.y*0.5, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                1,          // int hJustification,
                2,          // int vJustification,
                but.label + "|" + but.profileNom,
                "BUTON_LABEL_STYLE",
                0.0
            ),
            DL_Attributes ("BUTON_LABEL", 256, -1, -1, "BYLAYER"));
        
    }
}

int rido::paroi::getnLitButonsMax(){
    if (butonList.size() == 0) {return 1;}
    int n = butonList[0].lit;
    for (auto buton: butonList) {
        // cout << buton.lit << endl;
        n = max(n, buton.lit);
    }
    return n;
}

void rido::paroi::dxfPanneautage(string fileName) {
    // cout << "dxfPanneautage: " << fileName << endl;
    // CHECK IF FILE IN USE.....
    DL_Dxf dxf;
    DL_WriterA* dw = dxf.out(fileName.c_str(), DL_Codes::AC1015);
    
    // cout << getnLitButonsMax() << endl;

    writeDxfHeader(dxf, *dw);
    writeDxfLineType(dxf, *dw);
    writeDxfLayers(dxf, *dw, params["dxfLayOff"], getnLitButonsMax());
    writeDxfTextStyle(dxf, *dw);
    writeDxfHeaderEnd(dxf, *dw);

    // cout << "writeDxfLayers" << endl;

    // ENTITIES:
    dw->sectionEntities();

    std::cout << "DXF - Edges" << std::endl;
    writeDxfEdges(dxf, *dw);
    std::cout << "DXF - Panneaux" << std::endl;
    writeDxfPanneaux(dxf, *dw);
    std::cout << "DXF - Cages" << std::endl;
    writeDxfCages(dxf, *dw);
    std::cout << "DXF - Butons" << std::endl;
    writeDxfButons(dxf, *dw);

    if (params.contains("elevation") & (params["elevationDXF"].get<bool>())) {
        std::cout << "Elevation" << std::endl;
        writeDxfElevation(dxf, *dw, stringF::str2IdList(params["elevation"], ","));
    }
    
    // cout << "entities" << endl;

    // end section ENTITIES:
    dw->sectionEnd();
    dxf.writeObjects(*dw, "MY_OBJECTS");
    dxf.writeObjectsEnd(*dw);
 
    dw->dxfEOF();
    dw->close();
    delete dw;

    cout << "File " << fileName << " created." << endl;
}

float getLtirantMax(std::vector<rido::appui>& appuis) {
    float LtirantMax = 0.0;
    for (auto appui :appuis) {
        if (appui.type == rido::appuiType::tirant) {
            LtirantMax = max(LtirantMax, (appui.Ls + appui.Ll)*cos(appui.angle*PI/180.0f));
        }
    }
    return LtirantMax;
}

void rido::rido::writeDxfPhase(DL_Dxf& dxf, DL_WriterA& dw, ridoPhase& phase, std::vector<excavation>& excavations, float startX) {

    DL_Attributes PANNEAU_FICTIF (
        "PANNEAU_FICTIF", // Layer name for this entity or NULL for no layer
        256, // Color number (0..256). 0 = BYBLOCK, 256 = BYLAYER.
        -1,  // color24 24 bit color (0x00RRGGBB, see DXF reference).
        -1, // Line thickness. Defaults to zero. -1 = BYLAYER, -2 = BYBLOCK, -3 = default width
        "BYLAYER"   // const std::string& linetype, Line type name or "BYLAYER" or "BYBLOCK". Defaults to "BYLAYER"
    );
    
    dxf.writeLine(dw, DL_LineData (startX, params["AIpmH"], 0, startX+epaisseur, params["AIpmH"], 0), PANNEAU_FICTIF);
    dxf.writeLine(dw, DL_LineData (startX+epaisseur, params["AIpmH"], 0, startX+epaisseur, params["ASpm"], 0), PANNEAU_FICTIF);
    dxf.writeLine(dw, DL_LineData (startX+epaisseur, params["ASpm"], 0, startX, params["ASpm"], 0), PANNEAU_FICTIF);
    dxf.writeLine(dw, DL_LineData (startX, params["ASpm"], 0, startX, params["AIpmH"], 0), PANNEAU_FICTIF);
    dxf.writeLine(dw, DL_LineData (startX, params["ASpm"], 0, startX, phase.niveauExcavationTerre, 0), PANNEAU_FICTIF);

    float LtirantMax = getLtirantMax(phase.appuiList);

    float largeurCoteTerre = max(10.0f, LtirantMax + 5.0f);
    float largeurCoteFouille = 10.0f;

    DL_Attributes COUCHE (
        "COUCHE", // Layer name for this entity or NULL for no layer
        256, // Color number (0..256). 0 = BYBLOCK, 256 = BYLAYER.
        -1,  // color24 24 bit color (0x00RRGGBB, see DXF reference).
        -1, // Line thickness. Defaults to zero. -1 = BYLAYER, -2 = BYBLOCK, -3 = default width
        "BYLAYER"   // const std::string& linetype, Line type name or "BYLAYER" or "BYBLOCK". Defaults to "BYLAYER"
    );

    float sup, inf;
    int i = 0;
    for (auto couche: coucheList) {
        
        inf = couche.inf;
        sup = min(min(couche.sup, params["TN"]), phase.niveauExcavationTerre);
        // ======================== C??t?? Fouille =========================
        if (inf < phase.niveauExcavationTerre) {
            

            if (i == (coucheList.size() - 1)) {
                inf = min(couche.sup - 5.0, params["AIpmH"] - 5.0);
            }

            dxf.writeLine(dw, DL_LineData (startX, sup, 0, startX-largeurCoteTerre, sup, 0), COUCHE);
            dxf.writeText(dw, 
                DL_TextData (
                    startX-largeurCoteTerre+0.5, sup - 0.5, 0.0,
                    startX-largeurCoteTerre+0.5, sup - 0.5, 0.0,
                    0.5, 1.0,   // double height, double xScaleFactor,
                    0,          // int textGenerationFlags,
                    0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                    3,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                    couche.nom, // Text string
                    "COUCHE_STYLE",    // Style (font).
                    0.0)                // Rotation angle of dimension text away from default orientation.
                , DL_Attributes ("COUCHE", 256, -1, -1, "BYLAYER")
            );

        }
        
        // ======================== C??t?? Fouille =========================
        // if (inf < phase.niveauExcavationFouille) {
        //     sup = min(sup, phase.niveauExcavationFouille);
        //     dxf.writeLine(dw, DL_LineData (startX+epaisseur, sup, 0, startX+largeurCoteFouille, sup, 0), COUCHE);
        // }
            
        // ======================== Sous la paroi =========================
        if (sup < params["AIpmH"]) {
            dxf.writeLine(dw, DL_LineData (startX, sup, 0, startX+epaisseur, sup, 0), COUCHE);
        }
        
        i++;
    }

    for (auto exca: excavations) {
        if (exca.cote == coteParoi::fouille) {
            dxf.writeLine(dw, DL_LineData (startX+epaisseur, exca.niveau, 0, startX+largeurCoteFouille, exca.niveau, 0), DL_Attributes ("EXCAVATION", 256, -1, -1, "BYLAYER"));
            dxf.writeText(dw, 
                DL_TextData (
                    startX+epaisseur, exca.niveau, 0.0,
                    startX+epaisseur, exca.niveau, 0.0,
                    0.5, 1.0,   // double height, double xScaleFactor,
                    0,          // int textGenerationFlags,
                    0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                    1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                    // to_string(exca.niveau), // Text string
                    stringF::ftos(exca.niveau,2), // Text string
                    "COTE_STYLE",    // Style (font).
                    0.0)                // Rotation angle of dimension text away from default orientation.
                , DL_Attributes ("EXCAVATION", 256, -1, -1, "BYLAYER")
            );
        }

        if (exca.cote == coteParoi::terre) {
            // dxf.writeLine(dw, DL_LineData (startX, sup, 0, startX-largeurCoteTerre, sup, 0), COUCHE);
            dxf.writeLine(dw, DL_LineData (startX, exca.niveau, 0, startX-largeurCoteTerre, exca.niveau, 0), DL_Attributes ("EXCAVATION", 256, -1, -1, "BYLAYER"));
            dxf.writeText(dw, 
                DL_TextData (
                    startX, exca.niveau, 0.0,
                    startX, exca.niveau, 0.0,
                    0.5, 1.0,   // double height, double xScaleFactor,
                    0,          // int textGenerationFlags,
                    2,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                    1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                    stringF::ftos(exca.niveau,2), // Text string
                    "COTE_STYLE",    // Style (font).
                    0.0)                // Rotation angle of dimension text away from default orientation.
                , DL_Attributes ("EXCAVATION", 256, -1, -1, "BYLAYER")
            );
        }
    }
    
    // ============== Tirants =======================
    float Dlibre = 0.18;
    float Dscellement = 0.5;
    float x1, x2, y1, y2;
    float x1a, y1a, x1b, y1b;
    float x2a, y2a, x2b, y2b;
    for (auto tirant: phase.appuiList) {
        dxf.writeText(dw, 
            DL_TextData (
                startX + epaisseur + largeurCoteFouille*0.5 + 0.1, tirant.niveau - 0.7, 0.0,
                startX + epaisseur + largeurCoteFouille*0.5 + 0.1, tirant.niveau - 0.7, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                // to_string(tirant.K), // Text string
                "K=" + stringF::ftos(tirant.K,0),
                "COTE_STYLE",    // Style (font).
                0.0)                // Rotation angle of dimension text away from default orientation.
            , DL_Attributes ("COTE", 256, -1, -1, "BYLAYER")
        );

        // Niveau
        dxf.writeText(dw, 
            DL_TextData (
                startX + 0.1, tirant.niveau + 0.7, 0.0,
                startX + 0.1, tirant.niveau + 0.7, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                3,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                // to_string(tirant.K), // Text string
                stringF::ftos(tirant.niveau,2),
                "COTE_STYLE",    // Style (font).
                0.0)                // Rotation angle of dimension text away from default orientation.
            , DL_Attributes ("COTE", 256, -1, -1, "BYLAYER")
        );

        // Lit
        dxf.writeText(dw, 
            DL_TextData (
                startX + epaisseur + 0.1, tirant.niveau + 0.7, 0.0,
                startX + epaisseur + 0.1, tirant.niveau + 0.7, 0.0,
                0.3, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                2,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                3,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                // to_string(tirant.K), // Text string
                to_string(tirant.lit),
                "COTE_STYLE",    // Style (font).
                0.0)                // Rotation angle of dimension text away from default orientation.
            , DL_Attributes ("COTE", 256, -1, -1, "BYLAYER")
        );

        if (tirant.type == appuiType::tirant) {
            // =================== Llibre ==================
            // Axe
            x1 = epaisseur;
            y1 = tirant.niveau;
            x2 = x1 - tirant.Ll*cos(tirant.angle*PI/180.0);
            y2 = y1 - tirant.Ll*sin(tirant.angle*PI/180.0);
            dxf.writeLine(dw, DL_LineData (startX+x1, y1, 0, startX+x2, y2, 0), DL_Attributes ("APPUI_PROVISOIRE_AXE", 256, -1, -1, "BYLAYER"));
            dxf.writeText(dw, 
                DL_TextData (
                    startX+0.5*x1 + 0.5*x2, 0.5*y1 + 0.5*y2, 0.0,
                    startX+0.5*x1 + 0.5*x2, 0.5*y1 + 0.5*y2, 0.0,
                    0.5, 1.0,   // double height, double xScaleFactor,
                    0,          // int textGenerationFlags,
                    1,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                    2,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                    "Ll =" + to_string(tirant.Lu - tirant.Ls*0.5) + "m", // Text string
                    "COTE_STYLE",    // Style (font).
                    0.0)                // Rotation angle of dimension text away from default orientation.
                , DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER")
            );

            x1a = x1 - Dlibre*0.5*sin(tirant.angle*PI/180.0);
            y1a = y1 + Dlibre*0.5*cos(tirant.angle*PI/180.0);
            x1b = x1 + Dlibre*0.5*sin(tirant.angle*PI/180.0);
            y1b = y1 - Dlibre*0.5*cos(tirant.angle*PI/180.0);

            // Lscellement
            x1 = x2 + 0.0;
            y1 = y2 + 0.0;
            x2 = x1 - tirant.Ls*cos(tirant.angle*PI/180.0);
            y2 = y1 - tirant.Ls*sin(tirant.angle*PI/180.0);
            dxf.writeLine(dw, DL_LineData (startX+x1, y1, 0, startX+x2, y2, 0), DL_Attributes ("APPUI_PROVISOIRE_AXE", 256, -1, -1, "BYLAYER"));
            dxf.writeText(dw, 
                DL_TextData (
                    startX+0.5*x1 + 0.5*x2, 0.5*y1 + 0.5*y2, 0.0,
                    startX+0.5*x1 + 0.5*x2, 0.5*y1 + 0.5*y2, 0.0,
                    0.5, 1.0,   // double height, double xScaleFactor,
                    0,          // int textGenerationFlags,
                    1,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                    2,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                    "Ls =" + to_string(tirant.Ls) + "m", // Text string
                    "COTE_STYLE",    // Style (font).
                    0.0)                // Rotation angle of dimension text away from default orientation.
                , DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER")
            );


            x2a = x2 - Dlibre*0.5*sin(tirant.angle*PI/180.0);
            y2a = y2 + Dlibre*0.5*cos(tirant.angle*PI/180.0);
            x2b = x2 + Dlibre*0.5*sin(tirant.angle*PI/180.0);
            y2b = y2 - Dlibre*0.5*cos(tirant.angle*PI/180.0);
            // msp.add_lwpolyline([(x1a, y1a), (x2a, y2a), (x2b, y2b), (x1b, y1b)], dxfattribs={'layer': 'APPUI_PROVISOIRE'})

            dxf.writeLine(dw, DL_LineData (startX+x1a, y1a, 0, startX+x2a, y2a, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            dxf.writeLine(dw, DL_LineData (startX+x2a, y2a, 0, startX+x2b, y2b, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            dxf.writeLine(dw, DL_LineData (startX+x2b, y2b, 0, startX+x1b, y1b, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            dxf.writeLine(dw, DL_LineData (startX+x1b, y1b, 0, startX+x1a, y1a, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));

            x1a = x1 - Dscellement*0.5*sin(tirant.angle*PI/180.0);
            y1a = y1 + Dscellement*0.5*cos(tirant.angle*PI/180.0);
            x2a = x2 - Dscellement*0.5*sin(tirant.angle*PI/180.0);
            y2a = y2 + Dscellement*0.5*cos(tirant.angle*PI/180.0);
            x1b = x1 + Dscellement*0.5*sin(tirant.angle*PI/180.0);
            y1b = y1 - Dscellement*0.5*cos(tirant.angle*PI/180.0);
            x2b = x2 + Dscellement*0.5*sin(tirant.angle*PI/180.0);
            y2b = y2 - Dscellement*0.5*cos(tirant.angle*PI/180.0);
            dxf.writeLine(dw, DL_LineData (startX+x1a, y1a, 0, startX+x2a, y2a, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            dxf.writeLine(dw, DL_LineData (startX+x2a, y2a, 0, startX+x2b, y2b, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            dxf.writeLine(dw, DL_LineData (startX+x2b, y2b, 0, startX+x1b, y1b, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            dxf.writeLine(dw, DL_LineData (startX+x1b, y1b, 0, startX+x1a, y1a, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            // msp.add_lwpolyline([(x1a, y1a), (x2a, y2a)], dxfattribs={'layer': 'APPUI_PROVISOIRE'})
            // msp.add_lwpolyline([(x1b, y1b), (x2b, y2b)], dxfattribs={'layer': 'APPUI_PROVISOIRE'})

            // {\fArial|b0|i0|c0|p34;\H1.2083x;??}
            // sign = '-' if tirant['niveau'] < 0.0 else '+'
            // msp.add_text(sign + str(tirant['niveau']), dxfattribs={'layer': 'COTE', 'style': 'TEXT_STYLE', 'height': 0.35}).set_pos((ep+0.1, tirant['niveau'] + 0.1), align='BOTTOM_LEFT')

            dxf.writeText(dw, 
                DL_TextData (
                    startX+epaisseur + 0.1, tirant.niveau + 0.1, 0.0,
                    startX+epaisseur + 0.1, tirant.niveau + 0.1, 0.0,
                    0.5, 1.0,   // double height, double xScaleFactor,
                    0,          // int textGenerationFlags,
                    0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                    1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                    // to_string(tirant.niveau), // Text string
                    stringF::ftos(tirant.niveau,2), // Text string
                    "COTE_STYLE",    // Style (font).
                    0.0)                // Rotation angle of dimension text away from default orientation.
                , DL_Attributes ("COTE", 256, -1, -1, "BYLAYER")
            );
        } else if (tirant.type == appuiType::dalle) {
            // # ============== AXE =======================
            // msp.add_lwpolyline([(but['start']['x'], but['start']['y']), (but['end']['x'], but['end']['y'])], dxfattribs={'layer': layer1[but['lit']]})
            dxf.writeLine(dw, DL_LineData (startX+epaisseur, tirant.niveau, 0, startX+epaisseur+5.0, tirant.niveau, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));
            dxf.writeLine(dw, DL_LineData (startX+epaisseur, tirant.niveau-0.2, 0, startX+epaisseur+5.0, tirant.niveau-0.2, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));

        } else if (tirant.type == appuiType::buton) {
            // # ============== AXE =======================
            // msp.add_lwpolyline([(but['start']['x'], but['start']['y']), (but['end']['x'], but['end']['y'])], dxfattribs={'layer': layer1[but['lit']]})
            dxf.writeLine(dw, DL_LineData (startX+epaisseur-0.2, tirant.niveau, 0, startX+epaisseur+5.0, tirant.niveau, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));

            float Dbuton = 0.6;
            float ePlatine = 0.1;
            float hPlatine = 1.5;
            // msp.add_lwpolyline([(ep - 0.5, buton['niveau']), (coteFouille/2.0, buton['niveau'])], dxfattribs={'layer': 'APPUI_PROVISOIRE_AXE'})
            // # Platine

            dxf.writeLine(dw, DL_LineData (startX+epaisseur-0.2, tirant.niveau, 0, startX+epaisseur+largeurCoteFouille/2.0, tirant.niveau, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));

            // msp.add_lwpolyline([(ep, buton['niveau'] - hPlatine/2.0), (ep + ePlatine, buton['niveau'] - hPlatine/2.0), (ep + ePlatine, buton['niveau'] + hPlatine/2.0), (ep, buton['niveau'] + hPlatine/2.0), (ep, buton['niveau'] - hPlatine/2.0)], dxfattribs={'layer':'APPUI_PROVISOIRE'})

            // std::vector<coor> polyline = {
            //     {startX+epaisseur, tirant.niveau - hPlatine/2.0f},
            //     {startX+epaisseur+ePlatine, tirant.niveau - hPlatine/2.0f},
            //     {startX+epaisseur+ePlatine, tirant.niveau + hPlatine/2.0f},
            //     {startX+epaisseur, tirant.niveau + hPlatine/2.0f}
            // };

            std::vector<coor> polyline;
            coor coor1, coor2, coor3, coor4;
            coor1.x = startX+epaisseur;
            coor1.y = tirant.niveau - hPlatine/2.0f;
            coor2.x = startX+epaisseur+ePlatine;
            coor2.y = tirant.niveau - hPlatine/2.0f;
            coor3.x = startX+epaisseur+ePlatine;
            coor3.y = tirant.niveau + hPlatine/2.0f;
            coor4.x = startX+epaisseur;
            coor4.y = tirant.niveau + hPlatine/2.0f;
            // coor coor1 = {
            //     startX+epaisseur, tirant.niveau - hPlatine/2.0f
            // };
            // coor coor2 (startX+epaisseur+ePlatine, tirant.niveau - hPlatine/2.0f);
            // coor coor3 = {startX+epaisseur+ePlatine, tirant.niveau + hPlatine/2.0f};
            // coor coor4 = {startX+epaisseur, tirant.niveau + hPlatine/2.0f};

            polyline.push_back(coor1);
            polyline.push_back(coor2);
            polyline.push_back(coor3);
            polyline.push_back(coor4);
            // polyline.push_back(coor );
            // polyline.push_back(coor );
            // polyline.push_back(coor );

            writeClosedPolyline(dxf, dw, polyline, "APPUI_PROVISOIRE");

            // 
            // // # Buton
            // msp.add_lwpolyline([(ep + ePlatine, buton['niveau'] + Dbuton/2.0), (coteFouille/2.0 - 0.5, buton['niveau'] + Dbuton/2.0)],dxfattribs={'layer': 'APPUI_PROVISOIRE'})

            dxf.writeLine(dw, DL_LineData (startX+epaisseur+ePlatine, tirant.niveau+Dbuton/2.0, 0, startX+epaisseur+largeurCoteFouille/2.0-0.5, tirant.niveau+Dbuton/2.0, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));

            // msp.add_lwpolyline([(ep + ePlatine, buton['niveau'] - Dbuton/2.0), (coteFouille/2.0 - 0.5, buton['niveau'] - Dbuton/2.0)],dxfattribs={'layer': 'APPUI_PROVISOIRE'})

            dxf.writeLine(dw, DL_LineData (startX+epaisseur+ePlatine, tirant.niveau-Dbuton/2.0, 0, startX+epaisseur+largeurCoteFouille/2.0-0.5, tirant.niveau-Dbuton/2.0, 0), DL_Attributes ("APPUI_PROVISOIRE", 256, -1, -1, "BYLAYER"));

            // sign = '-' if buton['niveau'] < 0.0 else '+'
            // msp.add_text(sign + str(buton['niveau']), dxfattribs={'layer': 'COTE', 'style': 'TEXT_STYLE', 'height': 0.35}).set_pos((ep+0.1,buton['niveau'] + 0.1), align='BOTTOM_LEFT')

            // # ============== TEXT BUTON =======================
            // msp.add_text(but['label'] + '|' + but['section'] + '|' + str(round(but['length'],2)), dxfattribs={'layer': layer2[but['lit']], 'style': 'BUTON_LABEL_STYLE', 'height': 0.45}).set_pos((but['start']['x']/2.0 + but['end']['x']/2.0, but['start']['y']/2.0 + but['end']['y']/2.0), align='MIDDLE_CENTER')

            // dxf.writeText(dw, 
            //     DL_TextData (
            //         startX+epaisseur + 0.1, tirant.niveau + 0.1, 0.0,
            //         startX+epaisseur + 0.1, tirant.niveau + 0.1, 0.0,
            //         0.5, 1.0,   // double height, double xScaleFactor,
            //         0,          // int textGenerationFlags,
            //         0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
            //         1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
            //         // to_string(tirant.niveau), // Text string
            //         stringF::ftos(tirant.niveau, 2),
            //         "COTE_STYLE",    // Style (font).
            //         0.0)                // Rotation angle of dimension text away from default orientation.
            //     , DL_Attributes ("COTE", 256, -1, -1, "BYLAYER")
            // );

            
        }
    }
}

void rido::rido::dxfCoupeType() {

    // cout << "qsdfqsdf" << endl;
    string fileName = folder + "/" + coupeName + ".dxf";

    DL_Dxf dxf;
    DL_WriterA* dw = dxf.out(fileName.c_str(), DL_Codes::AC1015);
 
    writeDxfHeader(dxf, *dw);
    writeDxfLineType(dxf, *dw);
    writeDxfLayers(dxf, *dw, "", 1);
    writeDxfTextStyle(dxf, *dw);
    writeDxfHeaderEnd(dxf, *dw);

    // ENTITIES:
    dw->sectionEntities();

    // writeDxfPhase(dxf, *dw, phases[0], excavationList, 0.0f);

    float startX = 0.0f;
    for (auto phase: phases) {
        float LtirantMax = getLtirantMax(phase.appuiList);
        float largeurCoteTerre = max(10.0f, LtirantMax + 5.0f);
        startX += largeurCoteTerre + 10.0 + 10.0;

        vector<excavation> excavationList0;
        // excavation newExcavation = {coteParoi::fouille, phase.niveauExcavationFouille};
        excavationList0.push_back(excavation {coteParoi::fouille, phase.niveauExcavationFouille});
        excavationList0.push_back(excavation {coteParoi::terre, phase.niveauExcavationTerre});

        writeDxfPhase(dxf, *dw, phase, excavationList0, startX);

        dxf.writeText(*dw, 
            DL_TextData (
                startX+epaisseur + 0.1, params["ASpm"] + 10.0, 0.0,
                startX+epaisseur + 0.1, params["ASpm"] + 10.0, 0.0,
                0.5, 1.0,   // double height, double xScaleFactor,
                0,          // int textGenerationFlags,
                0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                "PHASE " + to_string(phase.id), // Text string
                "COTE_STYLE",    // Style (font).
                0.0)                // Rotation angle of dimension text away from default orientation.
            , DL_Attributes ("COTE", 256, -1, -1, "BYLAYER")
        );
    }


    // getLtirantMax(std::vector<rido::appui>& appuis)
    /*
    // DIMENSION:
    DL_DimensionData dimData(10.0,                  // def point (dimension line pos)
                             50.0,
                             0.0,
                             0,                     // text pos (irrelevant if flag 0x80 (128) set for type
                             0,
                             0.0,
                             0x1,                   // type: aligned with auto text pos (0x80 NOT set)
                             8,                     // attachment point: bottom center
                             2,                     // line spacing: exact
                             1.0,                   // line spacing factor
                             "",                    // text
                             "Standard",            // style
                             0.0,                   // text angle
                             1.0,                   // linear factor
                             1.0);                  // dim scale

    DL_DimAlignedData dimAlignedData(10.0,          // extension point 1
                                     5.0,
                                     0.0,
                                     30.0,          // extension point 2
                                     5.0,
                                     0.0);

    dxf.writeDimAligned(*dw, dimData, dimAlignedData, attributes);

    
    // start hatch with one loop:
    DL_HatchData data(
        1,      // int numLoops; Number of boundary paths (loops).
        false,  // bool solid; Solid fill flag (true=solid, false=pattern).
        100.0,  // double scale; Pattern scale or spacing
        0.0,        // double angle; Pattern angle in degrees
        "ESCHER",   // std::string pattern; Pattern name.
        0.0,        // double originX; Pattern origin
        0.0);       // double originY; Pattern origin
    
    
    dxf.writeHatch1(*dw, data, attributes);
 
    // start loop:
    DL_HatchLoopData lData(1);
    dxf.writeHatchLoop1(*dw, lData);
    
    // line edge.
    DL_HatchEdgeData(double x1, double y1,
                     double x2, double y2) :
        defined(true),
        type(1),
        x1(x1),
        y1(y1),
        x2(x2),
        y2(y2) {
    }

    // arc edge.
    DL_HatchEdgeData(double cx, double cy,
                     double mx, double my,
                     double ratio,
                     double angle1, double angle2,
                     bool ccw) :
        defined(true),
        type(3),
        cx(cx),
        cy(cy),
        angle1(angle1),
        angle2(angle2),
        ccw(ccw),
        mx(mx),
        my(my),
        ratio(ratio) {
    }

    // write edge:
    DL_HatchEdgeData eData(
        0.0,
        0.0,
        100.0,
        0.0,
        M_PI*2,
        true
    );
    dxf.writeHatchEdge(*dw, eData);
 
    // end loop:
    dxf.writeHatchLoop2(*dw, lData);
 
    // end hatch:
    dxf.writeHatch2(*dw, data, attributes);
    */
    // end section ENTITIES:
    dw->sectionEnd();
    dxf.writeObjects(*dw, "MY_OBJECTS");
    dxf.writeObjectsEnd(*dw);
 
    dw->dxfEOF();
    dw->close();
    delete dw;

    std::cout << "File " << fileName << " created." << endl;
}

// void rido::paroi::getEdgeList(vector<dxf::entity> entities, vector<string> layers, vector<string> entitiesTypes) {
//     // vector<dxf::entity> entitiesList;
//     // vector<edge> edgeList;
//     int idEdge = 0, i;
//     for (auto ent: entities) {
//         if (stringF::stringInStringVector(ent.layer, layers) & stringF::stringInStringVector(ent.type, entitiesTypes) & (!ent.isBlock)) {
//             // entitiesList.push_back(ent);
//             if (ent.type == "LINE") {
//                 idEdge++;
//                 edge newEdge(idEdge, ent.points[0], ent.points[1]);
//                 edgesList.push_back(newEdge);
//             } else if (ent.type == "LWPOLYLINE") {
//                 i = 0;
//                 for (auto p: ent.points) {
//                     if (i>0) {
//                         idEdge++;
//                         edge newEdge(idEdge, ent.points[i-1], ent.points[i]);
//                         edgesList.push_back(newEdge);
//                     }
//                     if (i == (ent.points.size()-1)) {
//                         idEdge++;
//                         edge newEdge(idEdge, ent.points[i], ent.points[0]);
//                         edgesList.push_back(newEdge);
//                     }
//                     i++;
//                 }
//             }
            
//         }
//     }
//     // dxf::showDXFentities(entitiesList);
//     // return edgeList;
// }

void rido::paroi::getEdgesList(json paroiParam) {
    // std::cout << paroiParam.dump(4) << std::endl;
    std::string extension = fileF::getFileExtension(paroiParam["coffrage"]);

    // int i;
    if (extension == "dxf") {
        dxf::readDXF readDXF;
        readDXF.readDXFfile(paroiParam["folder"].get<std::string>() + "/" + paroiParam["coffrage"].get<std::string>());

        // readDXF.showDXFentities();

        // vector<dxf::entity> entities = dxf::readDXF();
        // dxf::showDXFentities(entities);
        std::vector<std::string> layers = stringF::splitStrSpace(paroiParam["dxfLayers"].get<std::string>(), ".");
        std::vector<std::string> entitiesTypes = stringF::splitStrSpace(paroiParam["dxfEntities"].get<std::string>(), ".");
        
        // newParoi.getEdgeList(readDXF.entityList, layers, entitiesTypes);
        int idEdge = 0, i;
        for (auto ent: readDXF.entityList) {
            if (stringF::stringInStringVector(ent.layer, layers) & stringF::stringInStringVector(ent.type, entitiesTypes) & (!ent.isBlock)) {
                // entitiesList.push_back(ent);
                if (ent.type == "LINE") {
                    idEdge++;
                    edge newEdge(idEdge, ent.points[0], ent.points[1]);
                    edgesList.push_back(newEdge);
                } else if (ent.type == "LWPOLYLINE") {
                    i = 0;
                    for (auto p: ent.points) {
                        if (i>0) {
                            idEdge++;
                            edge newEdge(idEdge, ent.points[i-1], ent.points[i]);
                            edgesList.push_back(newEdge);
                        }
                        if (i == (ent.points.size()-1)) {
                            idEdge++;
                            edge newEdge(idEdge, ent.points[i], ent.points[0]);
                            edgesList.push_back(newEdge);
                        }
                        i++;
                    }
                }
                
            }
        }

        //dxf::test(paroiParam["folder"].get<string>());
        
    } else if (extension == "csv") {
        // newParoi.edgesList = csv::csv2JsonList(paroiParam["folder"].get<string>() + "/" + paroiParam["coffrage"].get<string>(), 1, 2);
    }
}

void rido::dxfSection(string filename, string coorBars, string dispoBars) {
    // cout << "qsdfqsdf" << endl;

    DL_Dxf dxf;
    DL_WriterA* dw = dxf.out(filename.c_str(), DL_Codes::AC1015);
 
    writeDxfHeader(dxf, *dw);
    writeDxfLineType(dxf, *dw);
    writeDxfLayers(dxf, *dw, "", 1);
    writeDxfTextStyle(dxf, *dw);
    writeDxfHeaderEnd(dxf, *dw);

    // ENTITIES:
    dw->sectionEntities();

    std::vector<vector<float>> coorBarsCsv = csv::csv2List(coorBars);
    std::vector<json> dispoBarsJson = csv::csv2JsonList(dispoBars, 1, 2);
    for (auto line: dispoBarsJson) {
        // std::cout << line << std::endl;
        // output.dump(4) << endl;
        for (int i: stringF::str2IdList(line["Point"], ",")) {
            if (i>coorBarsCsv.size()) {
                std::cout << "Bar coor n?? " << i << " n'existe pas" << std::endl;
            } else {
                // float HA = line["HA"].get<float>();
                // float sup = line["sup"].get<float>();
                // float inf = line["inf"].get<float>();
                // float Lr = ancrageList[HA]*HA/1000.0;
                // BAsection::element newElement(coorBarsCsv[i-1][0], coorBarsCsv[i-1][1],
                // arma::getCoefficientEffectif(sup, inf, niveau, Lr)*HA*HA*PI/4000000.0);

                // bars.push_back(newElement);
                dxf.writeText(*dw, 
                    DL_TextData (
                        coorBarsCsv[i-1][0], coorBarsCsv[i-1][1], 0.0,
                        coorBarsCsv[i-1][0], coorBarsCsv[i-1][1], 0.0,
                        0.1, 1.0,   // double height, double xScaleFactor,
                        0,          // int textGenerationFlags,
                        0,          // int hJustification, 0 = Left (default), 1 = Center, 2 = Right OR 3 = Aligned, 4 = Middle, 5 = Fit (For 3, 4, 5 the vertical alignment has to be 0.)
                        1,          // int vJustification, 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
                        to_string(i), // Text string
                        "COTE_STYLE",    // Style (font).
                        0.0)                // Rotation angle of dimension text away from default orientation.
                    , DL_Attributes ("COTE", 256, -1, -1, "BYLAYER")
                );
            }
        }
    }

    // end section ENTITIES:
    dw->sectionEnd();
    dxf.writeObjects(*dw, "MY_OBJECTS");
    dxf.writeObjectsEnd(*dw);
 
    dw->dxfEOF();
    dw->close();
    delete dw;

    std::cout << "File " << filename << " created." << endl;
}
