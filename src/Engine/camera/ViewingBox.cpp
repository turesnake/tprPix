/*
 * ========================= ViewingBox.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ViewingBox.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"

#include "esrc_state.h"


//-- init is delay to ViewingBox::init() --
IntVec2  ViewingBox::windowSZ { 0, 0 };
IntVec2  ViewingBox::gameSZ   { 0, 0 };
bool   ViewingBox::isFullScreen {false};


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void ViewingBox::init(){

    json::WindowConfigJsonData windowConfig = json::parse_windowConfigJsonFile();

    // 用户设置的 window 尺寸（像素）
    double originSrcWidth  = static_cast<double>(windowConfig.windowPixW);
    double originSrcHeight = static_cast<double>(windowConfig.windowPixH);

    //---------------------//
    //  针对 windowConfig.json 中数据的修正
    // 过扁，或过窄的窗口，都会破坏 UI界面布局。
    // 所以，必须对 窗口尺寸做修正：长宽比例限制在 4:3 ~ 16:9 之间
    // 修正的原则，是把窗口往小了修：
    // 当长宽比 小于 4:3,  维持 原来 w值 不变，然后基于 4:3,  计算新的 h值
    // 当长宽比 大于 16:9, 维持 原来 h值 不变，然后基于 16:9, 计算新的 w值
    //---------------------//
    double srcWidth {};
    double srcHeight {};
    {
        double scale = originSrcWidth / originSrcHeight;

        if( scale < (4.0/3.0) ){
            srcWidth = originSrcWidth;
            srcHeight = 3.0 * srcWidth / 4.0;

        }else if( scale > (16.0/9.0) ){
            srcHeight = originSrcHeight;
            srcWidth = 16.0 * srcHeight / 9.0;
        }else{
            srcWidth = originSrcWidth;
            srcHeight = originSrcHeight;
        }
    }
    ViewingBox::windowSZ.x = static_cast<int>( floor(srcWidth) );
    ViewingBox::windowSZ.y = static_cast<int>( floor(srcHeight) );



    //--------------------//
    double windowSZ_vs_gameSZ_ {};

    windowSZ_vs_gameSZ_ = srcHeight / 1200.0; // 测试版简易法，玩家设置的窗口变小，mapent显示尺寸也将变小
    //windowSZ_vs_gameSZ_ = srcHeight / 2000.0;
    //windowSZ_vs_gameSZ_ = srcHeight / 3200.0;
    //windowSZ_vs_gameSZ_ = srcHeight / 4000.0;
                    // eque to windowConfig val 1920:1200 

    ViewingBox::gameSZ.x = static_cast<int>(floor(srcWidth/windowSZ_vs_gameSZ_));
    ViewingBox::gameSZ.y = static_cast<int>(floor(srcHeight/windowSZ_vs_gameSZ_));


    ViewingBox::isFullScreen = windowConfig.isFullScreen;
    tprAssert( ViewingBox::isFullScreen == false );
                        // 暂不支持 全屏模式 

    //---
    esrc::insertState( "viewingBox" );

    //---------

            tprDebug::console( 
                " ViewingBox:\n" \
                " === windowSZ: {0}, {1};" \
                " === gameSZ: {2}, {3};\n",
                ViewingBox::windowSZ.x, ViewingBox::windowSZ.y,
                ViewingBox::gameSZ.x, ViewingBox::gameSZ.y
            );

}




