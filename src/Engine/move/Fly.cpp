/*
 * ========================= Fly.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.12
 *                                        修改 -- 2019.01.12
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#include "Fly.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "Move.h" 
#include "SpeedLevel.h"
#include "GameObjPos.h"

//#include "debug.h" 


namespace{//-------------- namespace ------------------//

    /*
    float PI { 3.14159f };
    //-- [0, 2PI)
    float rad_up      { 0.5f*PI };
    float rad_down    { 1.5f*PI };
    float rad_left    {  PI };
    float rad_right   { 0.0f };
    float rad_right_2 { 2*PI };
    */


}//------------------ namespace: end ------------------//


/* ===========================================================
 *                   RenderUpdate   
 * -----------------------------------------------------------
 */
void Fly::RenderUpdate(){

    //-- 暂时不管 maskCount
    //...

    //-- 
    if( newflyIns.rad!=currentFlyIns.rad ){
        currentFlyIns = newflyIns;
    }else{
        calc_dir();
    }

    //-- 更新 collient集，并检测新的 地面，作不同计算
    //...

    //-- 正式移动 --
    goPosPtr->accum_currentFPos( translate );
}


/* ===========================================================
 *                       calc_dir  
 * -----------------------------------------------------------
 * -- 根据 currentFlyIns，计算出 本帧 位移向量
 */
void Fly::calc_dir(){

    translate = glm::normalize( glm::vec2{ cos(currentFlyIns.rad), sin(currentFlyIns.rad) } );
    float speed = (float)(int)movePtr->get_speedLv();
    translate *= speed; //-- 乘以 位移距离
}


