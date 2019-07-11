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


//namespace move_inn {//-------------- namespace: move_inn ------------------//
//}//------------------ namespace: move_inn end ------------------//



/* ===========================================================
 *               set_newCrawlDirAxes
 * -----------------------------------------------------------
 */
void Move::set_newCrawlDirAxes( const DirAxes &newDirAxes_ ){
    tprAssert( this->moveType == MoveType::Crawl );
    //-----------//
    //  isMoving
    //-----------//
    newDirAxes.is_zero() ?
        this->isMoving = false :
        this->isMoving = true;

    //-- 设置 go 方向 --
    this->newDirAxes = newDirAxes_;
    if( this->newDirAxes.get_x() < -DirAxes::threshold ){
        this->goRef.set_direction_and_isFlipOver( GODirection::Left );
    }else if(this->newDirAxes.get_x() > DirAxes::threshold ){
        this->goRef.set_direction_and_isFlipOver( GODirection::Right );
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
        this->goRef.actionSwitch.call_func( ActionSwitchType::Move_Move );
    }else if( (this->currentDirAxes.is_zero()==false) && this->newDirAxes.is_zero() ){
        this->goRef.actionSwitch.call_func( ActionSwitchType::Move_Idle );
    }

    this->currentDirAxes = this->newDirAxes;
    if( this->currentDirAxes.is_zero() ){
        return; //- end_frame of one_piece_input
    }

    //----------------//
    //   speedV - dpos/frame
    //----------------//
    glm::dvec2 speedV = this->currentDirAxes.to_dmpos();
    speedV *= SpeedLevel_2_val(this->speedLvl) *
                        60.0 * esrc::get_timer().get_smoothDeltaTime();
    //---- crawl -----//
    this->renderUpdate_inn( this->currentDirAxes, speedV );
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
    // dposOff -- 当前fpos 到 目标fpod 的实际偏移 （fpos）
    // speedV  -- 沿着方向能行走的 标准距离向量
    // -------
    // 在 drag 的最后一帧，dposOff 往往会小于 speedV。
    // 此时，应该手动 校准 speedV。
    // 从而让 go 走到指定的位置（而不是走过头）
    glm::dvec2 dposOff = this->targetDPos - this->goRef.goPos.get_currentDPos();
    glm::dvec2 speedV = glm::normalize( dposOff ); //- 等效于 DirAxes 的计算。
    speedV *=   SpeedLevel_2_val(this->speedLvl) *
                60.0 * esrc::get_timer().get_smoothDeltaTime();

    bool isLastFrame = false;
    if( (std::abs(speedV.x) > std::abs(dposOff.x)) ||
        (std::abs(speedV.y) > std::abs(dposOff.y)) ){
        isLastFrame = true;
        speedV = dposOff;
    }

    //---- crawl -----//
    this->renderUpdate_inn( this->currentDirAxes, speedV );
    
    if( isLastFrame ){
        this->isMoving = false;
                //- 放在这里 不够严谨，毕竟本帧还是 移动的。
    }
}


/* ===========================================================
 *               renderUpdate_inn
 * -----------------------------------------------------------
 * -- 通用
 */
void Move::renderUpdate_inn(    const DirAxes &newDirAxes_,
                                const glm::dvec2 &speedV_ ){

    GameObjPos &goPosRef = this->goRef.goPos;

    bool isObstruct {false}; //- 碰撞检测返回值，是否检测到 "无法穿过"的碰撞
    bool isCross    {false}; //- currentMidDPos 是否进入新的 mapent

    glm::dvec2 oldMidDPos = goPosRef.calc_rootAnchor_midDPos();
    glm::dvec2 newMidDPos = oldMidDPos + speedV_;

    //-- 这里计算的是 rootAnchorMidFPos 的 mcpos --
    MapCoord oldMCPos = dpos_2_mcpos( oldMidDPos );
    MapCoord newMCPos = dpos_2_mcpos( newMidDPos );

    MapCoord off {}; // newMCPos - oldMCPos
    NineBox  nb {};
    
    if( oldMCPos != newMCPos ){
        isCross = true;

        off = newMCPos - oldMCPos;
            tprAssert( off.is_match_with_nineBox() ); //- 一道简陋的检测, 确保 单帧位移 不超过 周边 8 mapent
        nb.set( off.get_mpos().x, off.get_mpos().y );

        //-- 执行碰撞检测，并获知 此回合移动 是否可穿过 --
        isObstruct = this->goRef.detect_collision( NineBox_XY_2_Idx(nb) ); // MAJOR !!!! 
    }

    if( isObstruct == false ){
        goPosRef.accum_currentDPos_2( speedV_ );
        if( isCross ){
            goPosRef.accum_currentMCPos_2(nb);
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
    goid_t   goid = this->goRef.id;

    chunkKey_t newChunkKey = anyMPos_2_chunkKey( goPosRef.get_currentMPos() );
                        //-- 确保在调用本函数之前，gopos 已经发生了位移
    Chunk &newChunkRef = esrc::get_chunkRef( newChunkKey );

    if( newChunkKey != this->goRef.currentChunkKey ){
        Chunk &oldChunkRef = esrc::get_chunkRef( this->goRef.currentChunkKey );

        tprAssert( oldChunkRef.erase_from_goIds(goid) == 1 );
        oldChunkRef.erase_from_edgeGoIds(goid);
        //---
        this->goRef.currentChunkKey = newChunkKey;
        newChunkRef.insert_2_goIds(goid);
    }
                    
    // 重新收集 rootCES 中每个 colliEnt 所在的 chunkKeys
    this->goRef.reCollect_chunkKeys();

    size_t chunkKeysSize = this->goRef.get_chunkKeysRef().size();
    if( chunkKeysSize == 1 ){
        newChunkRef.erase_from_edgeGoIds(goid);
    }else if( chunkKeysSize > 1 ){
        newChunkRef.insert_2_edgeGoIds(goid);
    }else{
        tprAssert(0);
    }
            // 这个方法存在问题，当 rootCES 中存在很多 colliEnt 时，
            // ge可能横跨 3，4 个 chunk。
            // 在当前方案中，只是简单设想了一种 只横跨 2个 chunk 的简单实现
            // 但也可能是我理解错了 。。。
            //

        //-----------------------------------------
        //-- 这个检测，最好在，所有工作都结束后，
        //   此时的结果最准确
        {//-- 打印 当前帧的 ces 区域 --- 
            //IntVec2 currentMPos = this->goRef.goPos.get_currentMPos();
            const ColliEntHead &doCehRef = this->goRef.get_rootColliEntHeadRef();
            const ColliEntSet &doCesRef = esrc::get_colliEntSetRef( doCehRef.colliEntSetIdx ); //- get do_ces_ref
            MapCoord cesMCPos;
            cesMCPos.set_by_mpos( this->goRef.get_rootCES_leftBottom_MPos() );

            tprDebug::clear_mapEntSlices();
            for( const auto &i : doCesRef.get_colliEnts() ){
                tprDebug::insert_new_mapEntSlice( i+cesMCPos );
            }
        }

}




