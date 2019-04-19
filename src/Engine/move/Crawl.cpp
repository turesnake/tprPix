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
#include <iomanip>

//-------------------- Engine --------------------//
#include "Move.h" 
#include "SpeedLevel.h"
#include "GameObjPos.h"
#include "MapCoord.h"
#include "Collision.h"
#include "GameObj.h" 
#include "esrc_chunk.h"

#include "debug.h" 



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
    this->goPtr = _goPtr; 
    this->movePtr  = _movePtr;
    this->goPosPtr = _goPosPtr;
    this->collisionPtr = _collisionPtr;
    //-- 暂时设置为 3档速度， 在go正式运行时，这个值会被改回去 --
    std::pair<int, float> pair = get_speed( SpeedLevel::LV_3 );
    this->max = pair.first;
    this->speed = pair.second;
    this->count = 0;
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
    if( this->currentNB.is_zero() && this->newNB.is_zero() ){
        return;
    }

    std::pair<int, float> pair;
    bool isObstruct {false}; //- 碰撞检测返回值，是否检测到 "无法穿过"的碰撞
        
    //----------------------------//
    //     Node Frame ／ 节点
    //----------------------------//
    if( this->count == 0 ){

        //-- 在 move状态切换的 两个点 调用 OnMove() ／ OnIdle() --
        if( this->currentNB.is_zero() && (this->newNB.is_zero()==false) ){
            this->goPtr->actionSwitch.call_func( ActionSwitchType::Move_Move );
        }else if( (this->currentNB.is_zero()==false) && this->newNB.is_zero() ){
           this->goPtr->actionSwitch.call_func( ActionSwitchType::Move_Idle );
        }

        this->currentNB = this->newNB;
        if( this->newNB.is_zero() ){
            return; //- end_frame of one_piece_input
        }

        //-- 执行碰撞检测，并获知 此回合移动 是否可穿过 --
        isObstruct = this->collisionPtr->collide_for_crawl( NineBox_XY_2_Idx(this->currentNB) );

        //-------- refresh speed / max -------//
        if( (this->currentNB.x!=0) && (this->currentNB.y!=0) ){ //- 斜向
            pair = get_speed_next( this->movePtr->get_speedLv() );
        }else{ //- 横移竖移
            pair = get_speed( this->movePtr->get_speedLv() );
        }
        this->max = pair.first;
        (isObstruct) ?
            this->speed=0.0f :        //- 移动被遮挡，仅仅将速度写0.并不取消回合本身
            this->speed=pair.second;  //- 移动未被遮挡，获得位移速度
    }

    //---------------------------//
    //  如果确认需要位移
    //   -- 检查本go 的 新chunk，如果发生变化，释放旧chunk 中的 goids, edgeGoIds
    //       登记到 新chunk 的 goids
    //   -- 重新统计 本go 的 chunkKeys，如果确认为 临界go，  
    //       登记到 主chunk 的 edgegoids 容器中
    //---------------------------//
    Chunk   *oldChunkPtr;
    Chunk   *newChunkPtr;
    goid_t   goid = this->goPtr->id;

    chunkKey_t newChunkKey = anyMPos_2_chunkKey( anyPPos_2_mpos( this->goPosPtr->get_currentPPos() ) );
    newChunkPtr = esrc::get_chunkPtr( newChunkKey );

    if( newChunkKey!=goPtr->currentChunkKey ){
        oldChunkPtr = esrc::get_chunkPtr( this->goPtr->currentChunkKey );
        assert( oldChunkPtr->goIds.erase(goid) == 1 );
        oldChunkPtr->edgeGoIds.erase(goid);
        //---
        goPtr->currentChunkKey = newChunkKey;
        newChunkPtr->goIds.insert( goid );
    }

    this->goPtr->reset_chunkKeys();
    size_t chunkKeysSize = this->goPtr->get_chunkKeysRef().size();
    if( chunkKeysSize == 1 ){
        newChunkPtr->edgeGoIds.erase( goid );
    }else if( chunkKeysSize > 1 ){
        newChunkPtr->edgeGoIds.insert( goid );
    }else{
        assert(0);
    }


    //---------------------------//
    //  确保本回合移动成立后（未碰撞）
    //  再实现真正的移动
    //---------------------------//
    if( this->speed!=0.0f ){
        if( this->currentNB.x == -1 ){
            this->goPosPtr->accum_currentFPos( -this->speed, 0.0f );    //- left -
        }else if( this->currentNB.x == 1 ){
            this->goPosPtr->accum_currentFPos( this->speed, 0.0f );     //- right -
        }
        if( this->currentNB.y == 1 ){
            this->goPosPtr->accum_currentFPos( 0.0f, this->speed );     //- up -
        }else if( this->currentNB.y == -1 ){
            this->goPosPtr->accum_currentFPos( 0.0f, -this->speed );    //- down -
        }
    }
    
    //---------------------------//
    //  累加计数器
    //  如果确认为回合结束点，务必校正 currentFPos 的值
    //---------------------------//
    this->count++;
    //----------//
    if( this->count == this->max ){
        this->count = 0;
        //-- 将 goPos.currentFPos 对齐与 mapent坐标系（消除小数偏移）
        this->goPosPtr->align_currentFPos_by_currentMCPos();
    }

}


/* ===========================================================
 *                   set_newCrawlDir
 * -----------------------------------------------------------
 * -- 
 */
void Crawl::set_newCrawlDir( const NineBox &_newNB ){
    this->newNB = _newNB;

    //-- 设置 go 方向 --
    if( this->newNB.x < 0 ){
        this->goPtr->direction = GODirection::Left;
        this->goPtr->set_isFlipOver_auto();  //-- 也许不该放在此处...

    }else if(this->newNB.x > 0){
        this->goPtr->direction = GODirection::Right;
        this->goPtr->set_isFlipOver_auto();  //-- 也许不该放在此处...

    }
 
}


namespace{//-------------- namespace ------------------//

/* ===========================================================
 *                     get_speed
 * -----------------------------------------------------------
 * -- 返回 对应的 speed pair
 */
std::pair<int, float>& get_speed( SpeedLevel _lv ){
    
    return speeds.at( speeds.size() - speedLevel_2_int(_lv) );
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
        return speeds.at( speeds.size() - speedLevel_2_int(_lv) + 1 );
    }
}

}//------------------ namespace: end ------------------//

