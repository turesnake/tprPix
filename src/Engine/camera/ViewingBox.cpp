/*
 * ========================= ViewingBox.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ViewingBox.h"

//--------------- C ------------------//
#include <cmath>

//--------------- Script ------------------//
#include "Script/json/json_all.h"


#include "tprDebug.h"


//-- init is delay to ViewingBox::init() --
IntVec2  ViewingBox::windowSZ { 0, 0 };
IntVec2  ViewingBox::gameSZ   { 0, 0 };
//double ViewingBox::windowSZ_vs_gameSZ {0.0};
bool   ViewingBox::isFullScreen {false};

double ViewingBox::z { static_cast<double>(VIEWING_BOX_Z_DEEP) };

double  ViewingBox::ground_zOff           { 10.0 };
double  ViewingBox::groundGo_zOff         { 20.0 };
double  ViewingBox::mapSurfaceLower_zOff  { 30.0 };

double  ViewingBox::waterAnim_zOff        { 40.0 };

double  ViewingBox::mapSurfaceUpper_zOff  { 50.0 };
double  ViewingBox::underGoShadows_zOff   { 60.0 };
double  ViewingBox::goShadows_zOff        { 70.0 };

//...
double  ViewingBox::debug_zOff         { 100.0 };

//...
double  ViewingBox::aboveMajorGoes_zOff  { 1890.0 };
double  ViewingBox::UIs_zOff             { 1900.0 };


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void ViewingBox::init(){

    json::WindowConfigJsonData windowConfig = json::parse_from_windowConfigJsonFile();

    double srcWidth  = static_cast<double>(windowConfig.windowPixW);
    double srcHeight = static_cast<double>(windowConfig.windowPixH);

    double windowSZ_vs_gameSZ_ {};
    /*
    if( srcHeight <= 720.0 ){
        windowSZ_vs_gameSZ = 1.0;
    }else{
        windowSZ_vs_gameSZ = srcHeight / 720.0;
    }
    */

    //windowSZ_vs_gameSZ_ = srcHeight / 1200.0; // 测试版简易法，玩家设置的窗口变小，mapent显示尺寸也将变小
    //windowSZ_vs_gameSZ_ = srcHeight / 1000.0;
    windowSZ_vs_gameSZ_ = srcHeight / 1200.0;
    //windowSZ_vs_gameSZ_ = srcHeight / 4000.0;

    ViewingBox::windowSZ.x = windowConfig.windowPixW;
    ViewingBox::windowSZ.y = windowConfig.windowPixH;

    ViewingBox::gameSZ.x = static_cast<int>(floor(srcWidth/windowSZ_vs_gameSZ_));
    ViewingBox::gameSZ.y = static_cast<int>(floor(srcHeight/windowSZ_vs_gameSZ_));


    ViewingBox::isFullScreen = windowConfig.isFullScreen;

    //---------
            cout<< " ViewingBox:\n"
                << " === windowSZ: " << ViewingBox::windowSZ.x
                << ", " << ViewingBox::windowSZ.y << ";\n"
                << " === gameSZ: " << ViewingBox::gameSZ.x
                << ", " << ViewingBox::gameSZ.y << ";\n"
                << endl;

}




