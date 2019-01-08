/*
 * ========================= Move.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    专门管理 GameObj实例 的 位移运动
 * ----------------------------
 */
#include "Move.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <utility> //- pair

//-------------------- Engine --------------------//
#include "GameObj.h" 

#include "debug.h" 


namespace{//-------------- namespace ------------------//

    //--- speed table, 8-level ----
    std::vector<std::pair<int, float>> speeds {
    //  max,   speed,  //
        { 1,    3.0f },   //- idx = 0,  LV_1
        { 2,    1.5f },   //- idx = 1,  LV_2
        { 3,    1.0f },   //- idx = 2,  LV_3
        { 4,    0.75f },  //- idx = 3,  LV_4
        { 5,    0.6f },   //- idx = 4,  LV_5
        { 6,    0.5f },   //- idx = 5,  LV_6
        { 8,    0.375f }, //- idx = 6,  LV_7
        { 10,   0.3f }    //- idx = 7,  LV_8
    };
    std::pair<int, float>& get_speed( SpeedLevel _lv );
    std::pair<int, float>& get_speed_next( SpeedLevel _lv );

}//------------------ namespace: end ------------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Move::init( GameObj *_goPtr ){
    goPtr = _goPtr;
    //-- 暂时设置为 3档速度， 在go正式运行时，这个值会被改回去 --
    std::pair<int, float> pair = get_speed( SpeedLevel::LV_3 );
    max = pair.first;
    speed = pair.second;
    count = 0;
}

/* ===========================================================
 *                     RenderUpdate
 * -----------------------------------------------------------
 * -- 本函数 在每1渲染帧，被 goPtr->RenderUpdate() 调用
 * -- 通过参数 获得 每一帧的 最新 CrossState 信息。
 * -- 结合原有的 cs信息，做成计算／决策
 */
void Move::RenderUpdate( CrossState _newCS ){

    if( count == max ){
        count = 0;
    }

    std::pair<int, float> pair;

    //=== node frame ／ 节点 ===
    if( count == 0 ){
        currentCS = _newCS;

        //-- 此处需要检测 新 mapent 是否被 占有／预定 --
        //...

        //-- 设置 goPtr->targetPos --

        //-------- refresh speed / max -------//
        if( (currentCS.x!=0) && (currentCS.y!=0) ){ //- 斜向
            pair = get_speed_next( goPtr->speedLevel );
        }else{ //- 横移竖移
            pair = get_speed( goPtr->speedLevel );
        }
        max = pair.first;
        speed = pair.second;
    }

    count++;
    //-------------------------//
    //   可能会 延迟到别处
    //-------------------------//
    if( currentCS.x == -1 ){
        goPtr->currentPos += glm::vec2{ -speed, 0.0f };  //- left -
    }else if( currentCS.x == 1 ){
        goPtr->currentPos += glm::vec2{ speed, 0.0f };   //- right -
    }
    if( currentCS.y == 1 ){
        goPtr->currentPos += glm::vec2{ 0.0f, speed };   //- up -
    }else if( currentCS.y == -1 ){
        goPtr->currentPos += glm::vec2{ 0.0f, -speed };   //- down -
    }

    //-- 如果本帧为 节点帧，因确保 goPtr->currentPos 对齐于 mapent
    //...
}


namespace{//-------------- namespace ------------------//

/* ===========================================================
 *                     get_speed
 * -----------------------------------------------------------
 * -- 返回 对应的 speed pair
 */
std::pair<int, float>& get_speed( SpeedLevel _lv ){
    
    return speeds.at( (int)_lv - 1 );
}

/* ===========================================================
 *                     get_speed
 * -----------------------------------------------------------
 * -- 返回 下一级的 speed pair
 * -- 若参数 _lv 为最高级，返回最高级的（tmp）
 */
std::pair<int, float>& get_speed_next( SpeedLevel _lv ){
    
    if( _lv == SpeedLevel::LV_8 ){
        return speeds.at(7);
    }else{
        return speeds.at( (int)_lv );
    }
}

}//------------------ namespace: end ------------------//

