/*
 * ===================== DPngColors.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- CPP --------------------//
#include <variant>


//-------------------- Engine --------------------//
#include "NineDirection.h"
#include "RGBA.h"
#include "BrokenLvl.h"
#include "FloorGoType.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace dpc_inn {//------------------ namespace: dpc_inn start ---------------------//


    const std::map<NineDirection, RGBA> lvl_0{
        { NineDirection::LeftTop,       RGBA{ 255, 178, 178, 255 } },// hsv: 0, 0.3, 1.0
        { NineDirection::Top,           RGBA{ 255, 102, 102, 255 } },// hsv: 0, 0.6, 1.0
        { NineDirection::RightTop,      RGBA{ 255,   0,   0, 255 } },// hsv: 0, 1.0, 1.0
        { NineDirection::Right,         RGBA{ 153,   0,   0, 255 } },// hsv: 0, 1.0, 0.6
        { NineDirection::RightBottom,   RGBA{  77,   0,   0, 255 } },// hsv: 0, 1.0, 0.3
        { NineDirection::Bottom,        RGBA{  77,  31,  31, 255 } },// hsv: 0, 0.6, 0.3
        { NineDirection::LeftBottom,    RGBA{  77,  54,  54, 255 } },// hsv: 0, 0.3, 0.3
        { NineDirection::Left,          RGBA{ 153, 107, 107, 255 } },// hsv: 0, 0.3, 0.6
        { NineDirection::Center,        RGBA{ 153,  61,  61, 255 } } // hsv: 0, 0.6, 0.6
    };

    const std::map<NineDirection, RGBA> lvl_1{
        { NineDirection::LeftTop,       RGBA{ 255, 229, 178, 255 } },// hsv: 40, 0.3, 1.0
        { NineDirection::Top,           RGBA{ 255, 204, 102, 255 } },// hsv: 40, 0.6, 1.0
        { NineDirection::RightTop,      RGBA{ 255, 170,   0, 255 } },// hsv: 40, 1.0, 1.0
        { NineDirection::Right,         RGBA{ 153, 102,   0, 255 } },// hsv: 40, 1.0, 0.6
        { NineDirection::RightBottom,   RGBA{  77,  51,   0, 255 } },// hsv: 40, 1.0, 0.3
        { NineDirection::Bottom,        RGBA{  77,  61,  31, 255 } },// hsv: 40, 0.6, 0.3
        { NineDirection::LeftBottom,    RGBA{  77,  69,  54, 255 } },// hsv: 40, 0.3, 0.3
        { NineDirection::Left,          RGBA{ 153, 138, 107, 255 } },// hsv: 40, 0.3, 0.6
        { NineDirection::Center,        RGBA{ 153, 122,  61, 255 } } // hsv: 40, 0.6, 0.6
    };

    const std::map<NineDirection, RGBA> lvl_2{
        { NineDirection::LeftTop,       RGBA{ 204, 255, 178, 255 } },// hsv: 100, 0.3, 1.0
        { NineDirection::Top,           RGBA{ 153, 255, 102, 255 } },// hsv: 100, 0.6, 1.0
        { NineDirection::RightTop,      RGBA{  85, 255,   0, 255 } },// hsv: 100, 1.0, 1.0
        { NineDirection::Right,         RGBA{  51, 153,   0, 255 } },// hsv: 100, 1.0, 0.6
        { NineDirection::RightBottom,   RGBA{  26,  77,   0, 255 } },// hsv: 100, 1.0, 0.3
        { NineDirection::Bottom,        RGBA{  46,  77,  31, 255 } },// hsv: 100, 0.6, 0.3
        { NineDirection::LeftBottom,    RGBA{  61,  77,  54, 255 } },// hsv: 100, 0.3, 0.3
        { NineDirection::Left,          RGBA{ 122, 153, 107, 255 } },// hsv: 100, 0.3, 0.6
        { NineDirection::Center,        RGBA{  92, 153,  61, 255 } } // hsv: 100, 0.6, 0.6
    };

    const std::map<NineDirection, RGBA> lvl_3{
        { NineDirection::LeftTop,       RGBA{ 178, 229, 255, 255 } },// hsv: 200, 0.3, 1.0
        { NineDirection::Top,           RGBA{ 102, 204, 255, 255 } },// hsv: 200, 0.6, 1.0
        { NineDirection::RightTop,      RGBA{   0, 170, 255, 255 } },// hsv: 200, 1.0, 1.0
        { NineDirection::Right,         RGBA{   0, 102, 153, 255 } },// hsv: 200, 1.0, 0.6
        { NineDirection::RightBottom,   RGBA{   0,  51,  77, 255 } },// hsv: 200, 1.0, 0.3
        { NineDirection::Bottom,        RGBA{  31,  61,  77, 255 } },// hsv: 200, 0.6, 0.3
        { NineDirection::LeftBottom,    RGBA{  54,  69,  77, 255 } },// hsv: 200, 0.3, 0.3
        { NineDirection::Left,          RGBA{ 107, 138, 153, 255 } },// hsv: 200, 0.3, 0.6
        { NineDirection::Center,        RGBA{  61, 122, 153, 255 } } // hsv: 200, 0.6, 0.6
    };

    const std::map<NineDirection, RGBA> lvl_4{
        { NineDirection::LeftTop,       RGBA{ 255, 178, 255, 255 } },// hsv: 300, 0.3, 1.0
        { NineDirection::Top,           RGBA{ 255, 102, 255, 255 } },// hsv: 300, 0.6, 1.0
        { NineDirection::RightTop,      RGBA{ 255,   0, 255, 255 } },// hsv: 300, 1.0, 1.0
        { NineDirection::Right,         RGBA{ 153,   0, 153, 255 } },// hsv: 300, 1.0, 0.6
        { NineDirection::RightBottom,   RGBA{  77,   0,  77, 255 } },// hsv: 300, 1.0, 0.3
        { NineDirection::Bottom,        RGBA{  77,  31,  77, 255 } },// hsv: 300, 0.6, 0.3
        { NineDirection::LeftBottom,    RGBA{  77,  54,  77, 255 } },// hsv: 300, 0.3, 0.3
        { NineDirection::Left,          RGBA{ 153, 107, 153, 255 } },// hsv: 300, 0.3, 0.6
        { NineDirection::Center,        RGBA{ 153,  61, 153, 255 } } // hsv: 300, 0.6, 0.6
    };
}//--------------------- namespace: dpc_inn end ------------------------//


// 完全不关心 运行效率
std::optional<std::pair<NineDirection, BrokenLvl>> rgba_2_DPngData( RGBA rgba_ )noexcept{

    HSV hsv = rgb_2_hsv( rgba_ ); // 返回值精度不够

    BrokenLvl broKenLvl {};
    const std::map<NineDirection, RGBA> *containerPtr {nullptr};

    if( is_closeEnough<double>(hsv.h, 0.0, 10.0) ){ 
        broKenLvl = BrokenLvl::Lvl_0;
        containerPtr = &(dpc_inn::lvl_0);
    }else if( is_closeEnough<double>(hsv.h, 40.0, 10.0) ){
        broKenLvl = BrokenLvl::Lvl_1;
        containerPtr = &(dpc_inn::lvl_1);
    }else if( is_closeEnough<double>(hsv.h, 100.0, 10.0) ){
        broKenLvl = BrokenLvl::Lvl_2;
        containerPtr = &(dpc_inn::lvl_2);
    }else if( is_closeEnough<double>(hsv.h, 200.0, 10.0) ){
        broKenLvl = BrokenLvl::Lvl_3;
        containerPtr = &(dpc_inn::lvl_3);
    }else if( is_closeEnough<double>(hsv.h, 300.0, 10.0) ){
        broKenLvl = BrokenLvl::Lvl_4;
        containerPtr = &(dpc_inn::lvl_4);
    }else{
        // not find
        // 要么是 辅助色，要么是颜色出错了，一切留给 caller 处理
        return std::nullopt;
    }
    //---

    //-- 逐个比较确认，效率最低的方案
    for( const auto & [iDir, iRGBA] : *containerPtr ){
        if( rgba_.is_near( iRGBA, 8) ){
            return { std::pair<NineDirection, BrokenLvl>{ iDir, broKenLvl } };
        }
    }

    // not find
    // 要么是 辅助色，要么是颜色出错了，一切留给 caller 处理
    return std::nullopt;
}





}//--------------------- namespace: blueprint end ------------------------//

