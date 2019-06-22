/*
 * ========================= Move.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "Move.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <vector>
#include <utility> //- pair
#include <iomanip>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "SpeedLevel.h"
#include "GameObjPos.h"
#include "MapCoord.h"
#include "Collision.h"
#include "GameObj.h" 
#include "ColliEntSet.h" //- tmp

#include "esrc_time.h"
#include "esrc_chunk.h"
#include "esrc_colliEntSet.h" //- tmp


#include "tprDebug.h" 


namespace move_inn {//-------------- namespace: move_inn ------------------//

    //--- 以下数据 被每一次 Crawl::RenderUpdate_2() 调用 共用 ---//
    bool isObstruct {false}; //- 碰撞检测返回值，是否检测到 "无法穿过"的碰撞
    bool isCross    {false}; //- currentMidFPos 是否进入新的 mapent

    glm::vec2 speedV {};     //- 当前帧的 速度: fpos/frame
    glm::vec2 oldMidFPos {}; //- rootAnchor 所在 mapent 的中点 的 fpos
    glm::vec2 newMidFPos {};

    glm::vec2 fposOff {}; //- 仅用于 drag 模式...

    MapCoord oldMCPos {};  //- rootAnchor 所在 mapent 的中点 的 mcpos
    MapCoord newMCPos {};

    MapCoord off {};  // newMCPos - oldMCPos
    NineBox  nb {};

}//------------------ namespace: move_inn end ------------------//



/* ===========================================================
 *               set_newCrawlDirAxes
 * -----------------------------------------------------------
 */
void Move::set_newCrawlDirAxes( const DirAxes &_newDirAxes ){
    tprAssert( this->moveType == MoveType::Crawl );
    //-----------//
    //  isMoving
    //-----------//
    newDirAxes.is_zero() ?
        this->isMoving = false :
        this->isMoving = true;

    //-- 设置 go 方向 --
    this->newDirAxes = _newDirAxes;
    if( this->newDirAxes.get_x() < -DirAxes::threshold ){
        this->goPtr->set_direction_and_isFlipOver( GODirection::Left );
    }else if(this->newDirAxes.get_x() > DirAxes::threshold ){
        this->goPtr->set_direction_and_isFlipOver( GODirection::Right );
    }
}


/* ===========================================================
 *              crawl_renderUpdate
 * -----------------------------------------------------------
 */
void Move::crawl_renderUpdate(){

    //----------------------------//
    //  currentDirAxes & newDirAxes
    //   控制 Move/Idle 动画的切换
    //----------------------------//
    //-- skip the time without "NineBox" input --
    if( this->currentDirAxes.is_zero() && this->newDirAxes.is_zero() ){
        return;
    }
    
    //-- 在 move状态切换的 两个点 调用 OnMove() ／ OnIdle() --
    if( this->currentDirAxes.is_zero() && (this->newDirAxes.is_zero()==false) ){
        this->goPtr->actionSwitch.call_func( ActionSwitchType::Move_Move );
    }else if( (this->currentDirAxes.is_zero()==false) && this->newDirAxes.is_zero() ){
        this->goPtr->actionSwitch.call_func( ActionSwitchType::Move_Idle );
    }

    this->currentDirAxes = this->newDirAxes;
    if( this->currentDirAxes.is_zero() ){
        return; //- end_frame of one_piece_input
    }

    //----------------//
    //  计算 speedV
    //----------------//
    move_inn::speedV = this->currentDirAxes.to_fmpos();
    move_inn::speedV *= SpeedLevel_2_val(this->speedLvl) *
                        60.0f * static_cast<float>(esrc::get_timer().get_smoothDeltaTime());
    //---- crawl -----//
    this->crawl_renderUpdate_inn( this->currentDirAxes, move_inn::speedV );
}



/* ===========================================================
 *              drag_renderUpdate
 * -----------------------------------------------------------
 */
void Move::drag_renderUpdate(){

    if( this->isMoving == false ){
        return;
    }

    //----------------//
    //  计算 speedV
    //----------------//
    // fposOff -- 当前fpos 到 目标fpod 的实际便宜 （fpos）
    // speedV  -- 沿着方向能行走的 标准距离向量
    // -------
    // 在 drag 的最后一帧，fposOff 往往会小于 speedV。
    // 此时，应该手动 校准 speedV。
    // 从而让 go 走到指定的位置（而不是走过头）
    move_inn::fposOff = this->targetFPos - this->goPosPtr->get_currentFPos();
    move_inn::speedV = glm::normalize( move_inn::fposOff ); //- 等效于 DirAxes 的计算。
    move_inn::speedV *=   SpeedLevel_2_val(this->speedLvl) *
                60.0f * static_cast<float>(esrc::get_timer().get_smoothDeltaTime());

    bool isLastFrame = false;
    if( (std::abs(move_inn::speedV.x) > std::abs(move_inn::fposOff.x)) ||
        (std::abs(move_inn::speedV.y) > std::abs(move_inn::fposOff.y)) ){
        isLastFrame = true;
        move_inn::speedV = move_inn::fposOff;
    }

    //---- crawl -----//
    this->crawl_renderUpdate_inn( this->currentDirAxes, move_inn::speedV );
    
    if( isLastFrame ){
        this->isMoving = false;
                //- 放在这里 不够严谨，毕竟本帧还是 移动的。
    }
}


/* ===========================================================
 *               crawl_renderUpdate_inn
 * -----------------------------------------------------------
 * -- 通用
 */
void Move::crawl_renderUpdate_inn(  const DirAxes &_newDirAxes,
                                    const glm::vec2 &_speedV ){


    move_inn::isObstruct = false; //- 碰撞检测返回值，是否检测到 "无法穿过"的碰撞
    move_inn::isCross    = false; //- currentMidFPos 是否进入新的 mapent


    move_inn::oldMidFPos = this->goPosPtr->calc_rootAnchor_midFPos();
    move_inn::newMidFPos = move_inn::oldMidFPos + _speedV;

    //-- 这里计算的是 rootAnchorMidFPos 的 mcpos --
    move_inn::oldMCPos = fpos_2_mcpos( move_inn::oldMidFPos );
    move_inn::newMCPos = fpos_2_mcpos( move_inn::newMidFPos );

    
    if( move_inn::oldMCPos != move_inn::newMCPos ){
        move_inn::isCross = true;

        move_inn::off = move_inn::newMCPos - move_inn::oldMCPos;
            tprAssert( move_inn::off.is_match_with_nineBox() ); //- 一道简陋的检测, 确保 单帧位移 不超过 周边 8 mapent
        move_inn::nb.set( move_inn::off.get_mpos().x, move_inn::off.get_mpos().y );

        //-- 执行碰撞检测，并获知 此回合移动 是否可穿过 --
        move_inn::isObstruct = this->collisionPtr->collide_for_crawl( NineBox_XY_2_Idx(move_inn::nb) );        
    }

    if( move_inn::isObstruct == false ){
        this->goPosPtr->accum_currentFPos_2( _speedV );
        if( move_inn::isCross ){
            this->goPosPtr->accum_currentMCPos_2(move_inn::nb);
                        //-- 使用 NineBox 来传递参数，
                        //   决定了当前模式下的 最大速度，不能超过 1_mapent_per_frame 
                        //   想要突破这个限制，就要 进一步 完善 collision 函数
                        //   让它支持，一次检测 数个 mapent
        }
    }
    
    //---------------------------//
    //  如果确认需要位移
    //   -- 检查本go 的 新chunk，如果发生变化，释放旧chunk 中的 goids, edgeGoIds
    //       登记到 新chunk 的 goids
    //   -- 重新统计 本go 的 chunkKeys，如果确认为 临界go，  
    //       登记到 主chunk 的 edgegoids 容器中
    //---------------------------//
    Chunk   *oldChunkPtr  {nullptr}; 
    Chunk   *newChunkPtr  {nullptr};
    goid_t   goid = this->goPtr->id;

    chunkKey_t newChunkKey = anyMPos_2_chunkKey(  this->goPosPtr->get_currentMPos() );
                        //-- 确保在调用本函数之前，gopos 已经发生了位移
    newChunkPtr = esrc::get_chunkPtr( newChunkKey );

    if( newChunkKey!=goPtr->currentChunkKey ){
        oldChunkPtr = esrc::get_chunkPtr( this->goPtr->currentChunkKey );
        tprAssert( oldChunkPtr->erase_from_goIds(goid) == 1 );
        oldChunkPtr->erase_from_edgeGoIds(goid);
        //---
        goPtr->currentChunkKey = newChunkKey;
        newChunkPtr->insert_2_goIds(goid);
    }

    this->goPtr->reset_chunkKeys();
    size_t chunkKeysSize = this->goPtr->get_chunkKeysRef().size();
    if( chunkKeysSize == 1 ){
        newChunkPtr->erase_from_edgeGoIds(goid);
    }else if( chunkKeysSize > 1 ){
        newChunkPtr->insert_2_edgeGoIds(goid);
    }else{
        tprAssert(0);
    }

        //-----------------------------------------
        //-- 这个检测，最好在，所有工作都结束后，
        //   此时的结果最准确
        {//-- 打印 当前帧的 ces 区域 --- 
            IntVec2 currentMPos = this->goPtr->goPos.get_currentMPos();
            //const IntVec2 compassPPos = this->goPtr->goPos.get_rootAnchorCompassPPos(); // 未被使用...
            const ColliEntHead &doCehRef = *(this->goPtr->get_rootColliEntHeadPtr());
            const ColliEntSet &doCesRef = esrc::get_colliEntSetRef( doCehRef.colliEntSetIdx ); //- get do_ces_ref
            MapCoord cesMCPos;
            cesMCPos.set_by_mpos( currentMPos - doCehRef.mposOff_from_cesLB_2_centerMPos );
            tprDebug::clear_mapEntSlices();
            for( const auto &i : doCesRef.get_colliEnts() ){
                tprDebug::insert_new_mapEntSlice( i+cesMCPos );
            }
        }

}




