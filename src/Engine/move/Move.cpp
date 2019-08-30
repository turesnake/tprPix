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
    tprAssert( this->is_crawl() );
    //-----------//
    //  isMoving
    //-----------//
    // 虽然设置了 isMoving，但暂时并未起作用
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
 *              renderUpdate_crawl
 * -----------------------------------------------------------
 */
void Move::renderUpdate_crawl(){

    //----------------------------//
    //    oldDirAxes & newDirAxes
    //   switch Move/Idle animAction
    //----------------------------//
    if( this->oldDirAxes.is_zero() && this->newDirAxes.is_zero() ){
        return;
    }
    
    //-- switch Move/Idle animAction --
    if( this->oldDirAxes.is_zero() && (this->newDirAxes.is_zero()==false) ){
        this->goRef.actionSwitch.call_func( ActionSwitchType::Move_Move );
    }else if( (this->oldDirAxes.is_zero()==false) && this->newDirAxes.is_zero() ){
        this->goRef.actionSwitch.call_func( ActionSwitchType::Move_Idle );
    }

    this->oldDirAxes = this->newDirAxes;
    if( this->newDirAxes.is_zero() ){
        return;
    }

    //----------------//
    //   speedV - dpos/frame
    //----------------//
    glm::dvec2 speedV = this->newDirAxes.to_dmpos();
    speedV *= SpeedLevel_2_val(this->speedLvl) *
                        60.0 * esrc::get_timer().get_smoothDeltaTime();

    //---- inn -----//
    this->for_regularGo_inn(  speedV );
}



/* ===========================================================
 *                  renderUpdate_drag
 * -----------------------------------------------------------
 */
void Move::renderUpdate_drag(){

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
    glm::dvec2 dposOff = this->targetDPos - this->goRef.get_pos_currentDPos();

    //- 距离过小时直接不 drag 
    if( (std::abs(dposOff.x)<1.0) && (std::abs(dposOff.y)<1.0) ){
        return;
    }

    glm::dvec2 speedV = glm::normalize( dposOff ); //- 等效于 DirAxes 的计算。
    speedV *=   SpeedLevel_2_val(this->speedLvl) *
                60.0 * esrc::get_timer().get_smoothDeltaTime();

    bool isLastFrame = false;
    if( (std::abs(speedV.x) > std::abs(dposOff.x)) ||
        (std::abs(speedV.y) > std::abs(dposOff.y)) ){
        isLastFrame = true;
        speedV = dposOff;
    }

    //---- inn -----//
    if( this->goRef.family == GameObjFamily::UI ){
        this->goRef.accum_uiGoPos_currentDPos( speedV );
    }else{
        this->for_regularGo_inn( speedV );
    }
    
    if( isLastFrame ){
        this->isMoving = false;
                //- 放在这里 不够严谨，毕竟本帧还是 移动的。
    }
}


/* ===========================================================
 *                  renderUpdate_adsorb
 * -----------------------------------------------------------
 */
void Move::renderUpdate_adsorb(){

    if( this->isMoving == false ){
        return;
    }

    glm::dvec2 dposOff = this->targetDPos - this->goRef.get_pos_currentDPos();

    if( (std::abs(dposOff.x)<0.1) && (std::abs(dposOff.y)<0.1) ){
        this->isMoving = false; //- 放在这里 不够严谨，毕竟本帧还是 移动的。
        return;
    }

    double ProximityRate = 0.3; //- 暂用固定的 接近倍率
    glm::dvec2 speedV = dposOff * ProximityRate;

    this->goRef.accum_uiGoPos_currentDPos( speedV );
}


/* ===========================================================
 *               for_regularGo_inn
 * -----------------------------------------------------------
 * -- uiGo 不会调用此函数
 */
void Move::for_regularGo_inn( const glm::dvec2 &speedV_ ){

    bool isObstruct {false}; //- 碰撞检测返回值，是否检测到 "无法穿过"的碰撞
    bool isCross    {false}; //- currentMidDPos 是否进入新的 mapent

    glm::dvec2 oldMidDPos = this->goRef.calc_goPos_rootAnchor_midDPos();
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
        //   事先设置为 不碰撞的 go，直接跳过 碰撞检测函数调用
        if( this->goRef.isMoveCollide ){
            isObstruct = this->goRef.detect_collision_for_move( NineBox_XY_2_Idx(nb) ); // MAJOR !!!! 
        }else{
            isObstruct = false;
        }

    }

    //-- 更新 goPos --
    if( isObstruct == false ){
        this->goRef.accum_goPos_current_dpos_and_mcpos( speedV_, nb, isCross );
    }

                // 如果移动被阻挡，其实也是可以发生位移的：
                // 正对碰撞面法线 的 move向量会被去除，
                // 平行于 碰撞面 的 move向量，会被保留
                // 于是，go可以贴着碰撞面滑动
                // 等待被实现。



    
    //---------------------------//
    //  如果确认需要位移
    //   -- 检查本go 的 新chunk，如果发生变化，释放旧chunk 中的 goids, edgeGoIds
    //       登记到 新chunk 的 goids
    //   -- 重新统计 本go 的 chunkKeys，如果确认为 临界go，  
    //       登记到 主chunk 的 edgegoids 容器中
    //---------------------------//

                // 这里存在漏洞，如果没有发生位移，不需要更新 chunk 数据
                // 未实现 ...




    //-- 只有在 移动碰撞检测中，确认发生了 mapent跨越的帧，才会释放和重登记 mapents
    //-- 但在每一个 移动帧，都会检测 chunkKeys 是否要更新


                //  也许不是很好的主意 ...
                //  下方的很多运算 都应该被限定 执行条件，不用每帧都执行 ...

    goid_t   goid = this->goRef.id;

    chunkKey_t newChunkKey = anyMPos_2_chunkKey( this->goRef.get_goPos_currentMPos() );
                        //-- 确保在调用本函数之前，gopos 已经发生了位移
    Chunk &newChunkRef = esrc::get_chunkRef( newChunkKey );

    size_t eraseNum {};

    if( newChunkKey != this->goRef.currentChunkKey ){
        Chunk &oldChunkRef = esrc::get_chunkRef( this->goRef.currentChunkKey );

        eraseNum = oldChunkRef.erase_from_goIds(goid);
        tprAssert( eraseNum == 1 );
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

        //-----------------------------------------
        //-- 这个检测，最好在，所有工作都结束后，
        //   此时的结果最准确
        {//-- 打印 当前帧的 ces 区域 --- 
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




