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

#include "esrc_time.h"
#include "esrc_chunk.h"

#include "speedLog.h"

#include "tprDebug.h" 



/* ===========================================================
 *               set_newCrawlDirAxes
 * -----------------------------------------------------------
 */
void Move::set_newCrawlDirAxes( const DirAxes &newDirAxes_ ){
    tprAssert( this->is_crawl() );
    //-----------//
    //  isMoving
    //-----------//
    newDirAxes.is_zero() ?
        this->isMoving = false :
        this->isMoving = true;

    //-- 设置 go 方向 --
    this->newDirAxes = newDirAxes_;

    //-- 当 majorGo 停止移动，其direction 保留原值 --
    NineDirection newDir = dirAxes_2_nineDirection(newDirAxes_);
    if( (newDir!=NineDirection::Mid) && (newDir!=goRef.get_actionDirection()) ){
        goRef.set_actionDirection( newDir );
        this->goRef.actionSwitch.call_func( ActionSwitchType::Move ); 
                            //-- 在未来，move 其实也有很多种.... 
    }                       //-- 有点丑陋的实现 ....
}


/* ===========================================================
 *              renderUpdate_crawl
 * -----------------------------------------------------------
 */
void Move::renderUpdate_crawl(){

            //-----------------------------------------
            //-- 这个检测，最好在，所有工作都结束后，
            //   此时的结果最准确
            /*
            {//-- 打印 当前帧的 ces 区域 --- 
                tprDebug::clear_mapEntSlices();
                for( const auto &i : this->goRef.get_currentSignINMapEntsRef() ){
                    tprDebug::insert_new_mapEntSlice( MapCoord{i} );
                }
            }
            */
            //-----------------------------------------

    //----------------------------//
    //    oldDirAxes & newDirAxes
    //   switch Move/Idle animAction
    //----------------------------//
    if( this->oldDirAxes.is_zero() && this->newDirAxes.is_zero() ){
        return;
    }
    
    //-- switch Move/Idle animAction --
    if( this->oldDirAxes.is_zero() && (this->newDirAxes.is_zero()==false) ){
        this->goRef.actionSwitch.call_func( ActionSwitchType::Move ); //-  move 其实也有很多种.... 
    }else if( (this->oldDirAxes.is_zero()==false) && this->newDirAxes.is_zero() ){
        this->goRef.actionSwitch.call_func( ActionSwitchType::Idle );
    }

    this->oldDirAxes = this->newDirAxes;
    if( this->newDirAxes.is_zero() ){
        return;
    }

    //----------------//
    //   speedDPos - dpos/frame
    //----------------//
    glm::dvec2 speedVec = this->newDirAxes.to_dpos() *
                        SpeedLevel_2_val(this->speedLvl) *
                        60.0 * esrc::get_timer().get_smoothDeltaTime();

    //---- inn -----//
    glm::dvec2 actuallySpped = this->goRef.detect_collision_for_move(speedVec);

    if( this->goRef.isControlByPlayer ){
        tprDebug::collect_playerSpeed( actuallySpped ); // debug
    }

    this->goRef.accum_dpos( this->goRef.detect_collision_for_move(speedVec) );

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
    //  计算 speedVec
    //----------------//
    // dposOff -- 当前fpos 到 目标fpod 的实际偏移 （fpos）
    // speedVec  -- 沿着方向能行走的 标准距离向量 (dpos)
    // -------
    // 在 drag 的最后一帧，dposOff 往往会小于 speedV。
    // 此时，应该手动 校准 speedV。
    // 从而让 go 走到指定的位置（而不是走过头）
    glm::dvec2 dposOff = this->targetDPos - this->goRef.get_dpos();

    //- 距离过小时直接不 drag 
    if( (std::abs(dposOff.x)<1.0) && (std::abs(dposOff.y)<1.0) ){
        return;
    }

    //- 等效于 DirAxes 的计算。
    glm::dvec2 speedVec =  glm::normalize( dposOff ) *
                            SpeedLevel_2_val(this->speedLvl) *
                            60.0 * esrc::get_timer().get_smoothDeltaTime();

    bool isLastFrame = false;
    if( (std::abs(speedVec.x) > std::abs(dposOff.x)) ||
        (std::abs(speedVec.y) > std::abs(dposOff.y)) ){
        isLastFrame = true;
        speedVec = dposOff;
    }

    //---- inn -----//
    if( this->goRef.family == GameObjFamily::UI ){
        this->goRef.accum_dpos( speedVec );
    }else{
        this->goRef.accum_dpos( this->goRef.detect_collision_for_move(speedVec) );
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

    glm::dvec2 dposOff = this->targetDPos - this->goRef.get_dpos();

    if( (std::abs(dposOff.x)<0.1) && (std::abs(dposOff.y)<0.1) ){
        this->isMoving = false; //- 放在这里 不够严谨，毕竟本帧还是 移动的。
        return;
    }

    double ProximityRate = 0.3; //- 暂用固定的 接近倍率
    glm::dvec2 speedDPos = dposOff * ProximityRate;

    this->goRef.accum_dpos( speedDPos );
}


