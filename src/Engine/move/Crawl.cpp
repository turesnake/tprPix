/*
 * ========================= Crawl.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY --
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
#include "Move.h" 
#include "SpeedLevel.h"
#include "GameObjPos.h"
#include "MapCoord.h"
#include "Collision.h"
#include "GameObj.h" 

#include "debug.h" 
#include <iomanip>


namespace{//-------------- namespace ------------------//

    //--- speed table, 8-level ----
    //-- 基于 5*5 mapent 的新速度表 --
    //  注意，3/6 两档无法整除。需要在每个 节点帧做修正...
    std::vector<std::pair<int, float>> speeds {
    //  max,   speed,                 speedLevel //
        { 1,    8.0f   },   //- idx = 0,  LV_8
        { 2,    4.0f   },   //- idx = 1,  LV_7
        { 3,    2.667f },   //- idx = 2,  LV_6 : 非整除，1回合移动 5.001; 需要修正
        { 4,    2.0f  },    //- idx = 3,  LV_5
        { 5,    1.6f   },   //- idx = 4,  LV_4
        { 6,    1.333f },   //- idx = 5,  LV_3 : 非整除，1回合移动 4.998; 需要修正
        { 8,    1.0f },     //- idx = 6,  LV_2
        { 10,   0.8f   }    //- idx = 7,  LV_1
    };
    std::pair<int, float>& get_speed( SpeedLevel _lv );
    std::pair<int, float>& get_speed_next( SpeedLevel _lv );

}//------------------ namespace: end ------------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Crawl::init(   GameObj *_goPtr, 
                    Move *_movePtr, 
                    GameObjPos *_goPosPtr, 
                    Collision *_collisionPtr  ){
    goPtr = _goPtr; 
    movePtr  = _movePtr;
    goPosPtr = _goPosPtr;
    collisionPtr = _collisionPtr;
    //-- 暂时设置为 3档速度， 在go正式运行时，这个值会被改回去 --
    std::pair<int, float> pair = get_speed( SpeedLevel::LV_3 );
    max = pair.first;
    speed = pair.second;
    count = 0;
    //---
}

/* ===========================================================
 *                     RenderUpdate
 * -----------------------------------------------------------
 * -- 本函数 在每1渲染帧，被 goPtr->RenderUpdate() 调用
 * -- 通过参数 获得 每一帧的 最新 NineBox 信息。
 * -- 结合原有的 cs信息，做成计算／决策
 */
void Crawl::RenderUpdate(){

    //-- skip the time without "NineBox" input --
    if( currentNB.is_zero() && newNB.is_zero() ){
        return;
    }

    std::pair<int, float> pair;
    bool isObstruct {false}; //- 碰撞检测返回值，是否检测到 "无法穿过"的碰撞
        
    //----------------------------//
    //     Node Frame ／ 节点
    //----------------------------//
    if( count == 0 ){

        //-- 在 move状态切换的 两个点 调用 OnMove() ／ OnIdle() --
        if( currentNB.is_zero() && (newNB.is_zero()==false) ){
            goPtr->actionSwitch.call_func( ActionSwitchType::Move_Move );

        }else if( (currentNB.is_zero()==false) && newNB.is_zero() ){
           goPtr->actionSwitch.call_func( ActionSwitchType::Move_Idle );
        }

        currentNB = newNB;
        if( newNB.is_zero() ){
            return; //- end_frame of one_piece_input
        }

        //-- 执行碰撞检测，并获知 此回合移动 是否可穿过 --
        isObstruct = collisionPtr->collide_for_crawl( NineBox_XY_2_Idx(currentNB) );

        //-------- refresh speed / max -------//
        if( (currentNB.x!=0) && (currentNB.y!=0) ){ //- 斜向
            pair = get_speed_next( movePtr->get_speedLv() );
        }else{ //- 横移竖移
            pair = get_speed( movePtr->get_speedLv() );
        }
        max = pair.first;
        (isObstruct) ?
            speed=0.0f :        //- 移动被遮挡，仅仅将速度写0.并不取消回合本身
            speed=pair.second;  //- 移动未被遮挡，获得位移速度
    }
    
    //---------------------------//
    //  确保本回合移动成立后（未碰撞）
    //  再实现真正的移动
    //---------------------------//
    if( speed!=0.0f ){
        if( currentNB.x == -1 ){
            goPosPtr->accum_currentFPos( -speed, 0.0f );    //- left -
        }else if( currentNB.x == 1 ){
            goPosPtr->accum_currentFPos( speed, 0.0f );     //- right -
        }
        if( currentNB.y == 1 ){
            goPosPtr->accum_currentFPos( 0.0f, speed );     //- up -
        }else if( currentNB.y == -1 ){
            goPosPtr->accum_currentFPos( 0.0f, -speed );    //- down -
        }
    }
    
    //---------------------------//
    //  累加计数器
    //  如果确认为回合结束点，务必校正 currentFPos 的值
    //---------------------------//
    count++;
    //----------//
    if( count == max ){
        count = 0;
        //-- 将 goPos.currentFPos 对齐与 mapent坐标系（消除小数偏移）
        goPosPtr->align_currentFPos_by_currentMCPos();
    }

}


/* ===========================================================
 *                   set_newCrawlDir
 * -----------------------------------------------------------
 * -- 
 */
void Crawl::set_newCrawlDir( const NineBox &_newNB ){
    newNB = _newNB;

    //-- 设置 go 方向 --
    if( newNB.x < 0 ){
        goPtr->direction = GODirection::Left;
        goPtr->set_isFlipOver_auto();  //-- 也许不该放在此处...

    }else if(newNB.x > 0){
        goPtr->direction = GODirection::Right;
        goPtr->set_isFlipOver_auto();  //-- 也许不该放在此处...

    }
 
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

