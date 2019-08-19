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
#include "Script/json/WindowConfigJsonData.h"


#include "tprDebug.h"


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void ViewingBox::init(){

    WindowConfigJsonData windowConfig = parse_from_windowConfigJsonFile();

        cout << "windowConfig.windowPix: " << windowConfig.windowPixW
            << ", " << windowConfig.windowPixH
            << endl;


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
    windowSZ_vs_gameSZ_ = srcHeight / 1200.0; // 测试版简易法，玩家设置的窗口变小，mapent显示尺寸也将变小

    ViewingBox::windowSZ.x = windowConfig.windowPixW;
    ViewingBox::windowSZ.y = windowConfig.windowPixH;

    ViewingBox::gameSZ.x = static_cast<int>(floor(srcWidth/windowSZ_vs_gameSZ_));
    ViewingBox::gameSZ.y = static_cast<int>(floor(srcHeight/windowSZ_vs_gameSZ_));


    ViewingBox::isFullScreen = windowConfig.isFullScreen;

}




