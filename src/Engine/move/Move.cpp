/*
 * ========================= Move.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Move.h"

//-------------------- CPP --------------------//
#include <utility> //- pair
#include <iomanip>

//--------------- Libs ------------------//
#include "magic_enum.hpp"

//-------------------- Engine --------------------//
#include "SpeedLevel.h"
#include "GameObjPos.h"
#include "MapCoord.h"
#include "Collision.h"
#include "GameObj.h" 

#include "esrc_time.h"
#include "esrc_chunk.h"
#include "esrc_gameObj.h"

#include "speedLog.h"


MoveType str_2_moveType( const std::string name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<MoveType>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find MoveType: {}", name_ );
        tprAssert(0);
        return MoveType::Crawl; // never reach
    }
}


// 参数可为 0
// input: player / AI 
/*
void Move::set_newCrawlDirAxes( const DirAxes &newDirAxes_ ){

    tprAssert( this->is_crawl() );

    this->crawlDirAxes.set_newVal( newDirAxes_ );

    //  isMoving
    //newDirAxes_.is_zero() ?
    //    this->isMoving = false :
    //    this->isMoving = true;
    

                // isMoving 暂时不在 crawl 中起任何作用 ...


    //----------------------------
    // 通过下面这段繁琐的调用，来避免在 同一帧内 重复调用 go.actionSwitch.call_func()

    NineDirection newDir = dirAxes_2_nineDirection( newDirAxes_ );
    NineDirection oldDir = this->goRef.actionDirection.get_oldVal();

    // 先修改 go.dir
    // 当本帧停止运动时，将保留原来的 go.dir 值 （此处的 Center值 是无意义的）
    if( (newDir!=NineDirection::Center) && (newDir!=oldDir) ){
        this->goRef.actionDirection.set_newVal( newDir );
        this->goRef.actionDirection.sync(); // MUST 
                    // 尽管如此，oldDir 仍然保存原来的值，方便下方做判断用

                    // 不应该放在此处 !!!

    }   

    // switch aaction
    if( this->crawlDirAxes.get_oldVal().is_zero() ){
        // last frame is Idle
        if( !newDirAxes_.is_zero() ){
            // this frame is Move
            this->goRef.actionSwitch.call_func( ActionSwitchType::Move ); 
                                //-  move 存在很多种类：walk,run,fly
                                //   统一让 具象go类 去处理
        }
    }else{
        // last frame is Move
        if( newDirAxes_.is_zero() ){
            // this frame is Idle
            this->goRef.actionSwitch.call_func( ActionSwitchType::Idle );
        }else{
            // this frame is Move
            if( newDir != oldDir ){
                this->goRef.actionSwitch.call_func( ActionSwitchType::Move ); 
            }
        }
    }
}
*/




void Move::set_newCrawlDirAxes( const DirAxes &newDirAxes_ ){
    tprAssert( this->is_crawl() );
    this->crawlDirAxes.set_newVal( newDirAxes_ );
}



void Move::renderUpdate_crawl(){

    if( !this->isMovingFunc() ){
        return;
    }

    //----------------//
    //   speedDPos - dpos/frame
    //----------------//
    glm::dvec2 speedVec = this->crawlDirAxes.get_newVal().get_worldVal() *
                        SpeedLevel_2_val( this->moveSpeedLvl.get_newVal() ) *
                        60.0 * esrc::get_timer().get_smoothDeltaTime();

    //---- inn -----//
    glm::dvec2 actuallySpped {};
    if( this->goRef.family == GameObjFamily::Major ){
        actuallySpped = this->goRef.get_collisionRef().detect_moveCollide(speedVec);
    }else if( this->goRef.family == GameObjFamily::WorldUI ){
        actuallySpped = speedVec;
        esrc::refresh_worldUIGo_chunkSignUpData( this->goRef, actuallySpped );
                            // 非 Major-go 中，只有 WorldUI-go 需要 重登记 chunk 信息
    }else{
        tprAssert(0); // 剩余几种 family，就不该允许到此处！
    }


    if( this->goRef.isControlByPlayer ){
        tprDebug::collect_playerSpeed( actuallySpped ); // debug
    }

    this->goRef.accum_dpos( actuallySpped );
}



void Move::renderUpdate_drag(){

    if( !this->isMovingFunc() ){
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

    //- 若距离太小，直接不 drag 
    if( (std::abs(dposOff.x)<1.0) && (std::abs(dposOff.y)<1.0) ){
        return;
    }

    //- 等效于 DirAxes 的计算。
    glm::dvec2 speedVec =  glm::normalize( dposOff ) *
                            SpeedLevel_2_val( this->moveSpeedLvl.get_newVal() ) *
                            60.0 * esrc::get_timer().get_smoothDeltaTime();

    bool isLastFrame = false;
    if( (std::abs(speedVec.x) > std::abs(dposOff.x)) ||
        (std::abs(speedVec.y) > std::abs(dposOff.y)) ){
        isLastFrame = true;
        speedVec = dposOff;
    }

    //---- inn -----//
    glm::dvec2 actuallySpped {};
    if( this->goRef.family == GameObjFamily::Major ){
        actuallySpped = this->goRef.get_collisionRef().detect_moveCollide(speedVec);
    }else if( this->goRef.family == GameObjFamily::WorldUI ){
        actuallySpped = speedVec;
        esrc::refresh_worldUIGo_chunkSignUpData( this->goRef, actuallySpped );
                            // 非 Major-go 中，只有 WorldUI-go 需要 重登记 chunk 信息
    }else{
        tprAssert(0); // 剩余几种 family，就不该允许到此处！
    }

    this->goRef.accum_dpos( actuallySpped );

    if( isLastFrame ){
        this->isMoving = false;
                //- 放在这里 不够严谨，毕竟本帧还是 移动的。
    }

}



void Move::renderUpdate_adsorb(){

    if( !this->isMovingFunc() ){
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


