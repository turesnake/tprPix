/*
 * ========================= esrc_coordinate.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.15
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_coordinate.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <memory>

//-------------------- Engine --------------------//
#include "esrc_state.h"


#include <iostream>
using std::cout;
using std::endl;


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
    double radian_x = atan( 954.0 / 2156.0 );
    glm::dvec2 xVec {   cos( radian_x ), 
                        sin( radian_x ) };
    double radian_y = atan( 1652.0 / 1245.0 );
    glm::dvec2 yVec {   -1.0 * cos( radian_y ), 
                        sin( radian_y ) };

    //---
    coord_inn::worldCoordUPtr = std::make_unique<Coordinate>( xVec, yVec );
    //---
    esrc::insertState("coordinate");
}


const Coordinate &get_worldCoordRef()noexcept{
    return *(coord_inn::worldCoordUPtr);
}





}//---------------------- namespace: esrc -------------------------//

