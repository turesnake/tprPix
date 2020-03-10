/*
 * ========================= esrc_coordinate.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.15
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_coordinate.h"

//-------------------- Engine --------------------//
#include "esrc_state.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace coord_inn {//-------- namespace: coord_inn --------------//
    
    std::unique_ptr<Coordinate> worldCoordUPtr {nullptr};

}//------------- namespace: coord_inn end --------------//


void init_coordinate(){

    //-- 目前唯一的 客制坐标系: 世界坐标系
    //   在游戏主程序中，默认在这个坐标系内执行运算，包括位移和 碰撞检测
    //   当需要显示到 window 上时，需要将数据，转换成 out坐标系
    //-------------------------------

    /*
    //------------ 45-degrees --------------//
    double radian = atan( 2134.0 / 2786.0 ); //- 外部测量得值 45-degrees
    double xVal = 1.0 * cos( radian );
    double yVal = 1.0 * sin( radian );
    glm::dvec2 xVec { xVal, -yVal };
    glm::dvec2 yVec { xVal,  yVal };
    */
    
    //------------- 30-degrees --------------//
    //- 外部测量得值
    double xVec_x = 2156.0;
    double xVec_y = 954.0;
    double yVec_x = 1245.0;
    double yVec_y = 1652.0;

    double xLen = sqrt( xVec_x*xVec_x + xVec_y*xVec_y );
    double yLen = sqrt( yVec_x*yVec_x + yVec_y*yVec_y );
    double pct = yLen / xLen; // yVec长度要被适当压缩

    double scale = 1.04; // 一个修正值，让坐标系的尺寸，匹配与 美术资源的尺寸

    double radian_x = atan( xVec_y / xVec_x );
    glm::dvec2 xVec {  scale * cos(radian_x), 
                       scale * sin(radian_x) };
    double radian_y = atan( yVec_y / yVec_x );
    glm::dvec2 yVec {   -1.0 * pct * scale * cos(radian_y), 
                               pct * scale * sin(radian_y) };

    //---
    coord_inn::worldCoordUPtr = std::make_unique<Coordinate>( xVec, yVec );
    //---
    esrc::insertState("coordinate");



    // test
    
    std::vector<glm::dvec2> vi {
        glm::dvec2{ -64.0, -64.0 },
        glm::dvec2{ -64.0,  64.0 },
        glm::dvec2{  64.0, -64.0 },
        glm::dvec2{  64.0,  64.0 }
    };

    for( const auto &dpos : vi ){
        glm::dvec2 out = get_worldCoordRef().calc_outDPos( dpos );
        tprDebug::console( "out: {}, {}", out.x, out.y );
    }

}


const Coordinate &get_worldCoordRef()noexcept{
    return *(coord_inn::worldCoordUPtr);
}





}//---------------------- namespace: esrc -------------------------//

