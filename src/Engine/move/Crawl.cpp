/*
 * ========================= Crawl.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    专门管理 GameObj实例 的 位移运动
 * ----------------------------
 */
#include "Crawl.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <utility> //- pair

//-------------------- Engine --------------------//
#include "GameObj.h" 
#include "SpeedLevel.h"

#include "debug.h" 


namespace{//-------------- namespace ------------------//

    //--- speed table, 8-level ----
    std::vector<std::pair<int, float>> speeds {
    //  max,   speed,                 speedLevel //
        { 1,    3.0f },   //- idx = 0,  LV_8
        { 2,    1.5f },   //- idx = 1,  LV_7
        { 3,    1.0f },   //- idx = 2,  LV_6
        { 4,    0.75f },  //- idx = 3,  LV_5
        { 5,    0.6f },   //- idx = 4,  LV_4
        { 6,    0.5f },   //- idx = 5,  LV_3
        { 8,    0.375f }, //- idx = 6,  LV_2
        { 10,   0.3f }    //- idx = 7,  LV_1
    };
    std::pair<int, float>& get_speed( SpeedLevel _lv );
    std::pair<int, float>& get_speed_next( SpeedLevel _lv );

}//------------------ namespace: end ------------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Crawl::init( GameObj *_goPtr, Move *_movePtr ){
    goPtr   = _goPtr;
    movePtr = _movePtr;
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
 * -- 通过参数 获得 每一帧的 最新 NineBox 信息。
 * -- 结合原有的 cs信息，做成计算／决策
 */
void Crawl::RenderUpdate(){

    //-- skip the time without "crossState" input --
    if( (currentNB.x==0) && (currentNB.y==0) 
        && (newNB.x==0) && (newNB.y==0) ){
            return;
    }

    if( count == max ){
        count = 0;
    }

    std::pair<int, float> pair;

    //=== node frame ／ 节点 ===
    if( count == 0 ){
        currentNB = newNB;

        //-- 此处需要检测 新 mapent 是否被 占有／预定 --
        // 根据 currentCS，确定 哪一个mapent 是 target
        // 访问这个 mapent 的数据，确保其没有被 占有
        //   -- 如果被占有了，查看此mapent上的 go，是否可“碰” 
        //      不管是否 “可碰”，本次移动都将被终止。将 target 设置为当前占有的格子。
        //      return， 结束本次函数调用
        //   -- 如果未被占有，正式 占有 此mapent
        //      然后执行下方的 操作
        //...

        //-- 设置 goPtr->targetPos --

        //-------- refresh speed / max -------//
        if( (currentNB.x!=0) && (currentNB.y!=0) ){ //- 斜向
            pair = get_speed_next( movePtr->get_speedLv() );
        }else{ //- 横移竖移
            pair = get_speed( movePtr->get_speedLv() );
        }
        max = pair.first;
        speed = pair.second;
    }

    count++;
    //-------------------------//
    //   可能会 延迟到别处
    //-------------------------//
    if( currentNB.x == -1 ){
        goPtr->currentPos += glm::vec2{ -speed, 0.0f };  //- left -
    }else if( currentNB.x == 1 ){
        goPtr->currentPos += glm::vec2{ speed, 0.0f };   //- right -
    }
    if( currentNB.y == 1 ){
        goPtr->currentPos += glm::vec2{ 0.0f, speed };   //- up -
    }else if( currentNB.y == -1 ){
        goPtr->currentPos += glm::vec2{ 0.0f, -speed };   //- down -
    }

    //-- 如果本帧为 节点帧，需确保 goPtr->currentPos 对齐于 mapent
    //...
}


namespace{//-------------- namespace ------------------//

/* ===========================================================
 *                     get_speed
 * -----------------------------------------------------------
 * -- 返回 对应的 speed pair
 */
std::pair<int, float>& get_speed( SpeedLevel _lv ){
    
    return speeds.at( speeds.size() - (int)_lv );
}

/* ===========================================================
 *                     get_speed
 * -----------------------------------------------------------
 * -- 返回 下一级的 speed pair
 * -- 若参数 _lv 为最高级，返回最高级的（tmp）
 */
std::pair<int, float>& get_speed_next( SpeedLevel _lv ){
    
    if( _lv == SpeedLevel::LV_1 ){
        return speeds.at(7);
    }else{
        return speeds.at( speeds.size() - (int)_lv + 1 );
    }
}

}//------------------ namespace: end ------------------//

