/*
 * ========================= Move.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_MOVE_H
#define TPR_MOVE_H

//-------------------- CPP --------------------//
#include <functional>
#include <memory>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "SpeedLevel.h"
#include "MapCoord.h"
#include "DirAxes.h"

//-- need --
class GameObj;
class GameObjPos;
class Collision;


enum class MoveType : int {
    Crawl,
    Drag //- 类似 camera的运动方式：go快速向 目标pos 靠拢
               //  被用于 UI 图标运动
};

inline MoveType str_2_MoveType( const std::string name_ ){
    if( name_ == std::string{"Crawl"} ){
        return MoveType::Crawl;
    }else if( name_ == std::string{"Drag"} ){
        return MoveType::Drag;
    }else{
        tprAssert(0);
        return MoveType::Crawl; //- never reach
    }
}


//-- 初级版本，在未来可能会发展成 数个 crawl实例 ／ 数个 fly实例
class Move{
    using F_RenderUpdate = std::function< void() >;
public:
    explicit Move( GameObj &goRef_ ):
        goRef(goRef_)
        {}

    inline void RenderUpdate(){
        this->renderUpdataFunc();
    }

    //------- flags -------//
    inline bool is_crawl() const {
        return (this->moveType==MoveType::Crawl);
    }

    //------- set -------//
    inline void set_MoveType( MoveType type_ ){
        this->moveType = type_;
        switch ( type_ ){
            case MoveType::Crawl:      
                this->renderUpdataFunc = std::bind( &Move::crawl_renderUpdate, this ); 
                return;
            case MoveType::Drag: 
                this->renderUpdataFunc = std::bind( &Move::drag_renderUpdate, this ); 
                return;
            default:
                tprAssert(0);
                return;
        }
    }
    inline void set_speedLvl( SpeedLevel lv_ ){
        this->speedLvl = lv_;
    }
    void set_newCrawlDirAxes( const DirAxes &newDirAxes_ );


    inline void set_drag_targetDPos( const glm::dvec2 &DPos_ ){
        tprAssert( this->moveType == MoveType::Drag );
        if( DPos_ == this->targetDPos ){
            return;
        }
        this->targetDPos = DPos_;
        this->isMoving = true;
    }

    //------- get -------//
    inline const SpeedLevel &get_speedLvl() const {
        return this->speedLvl;
    }
    inline bool is_drag_moving() const {
        tprAssert( this->moveType == MoveType::Drag );
        return this->isMoving;
    } 

private:

    void crawl_renderUpdate();
    void drag_renderUpdate();

    void renderUpdate_inn(  const DirAxes &newDirAxes_,
                            const glm::dvec2 &speedV_ );

    //===== vals =====//
    GameObj      &goRef;

    MoveType    moveType  { MoveType::Crawl };
    SpeedLevel  speedLvl  { SpeedLevel::LV_3 };

    
    DirAxes  newDirAxes     {};  //- 本次渲染帧，新传入的 方向值（每一帧都被外部代码更新）
    DirAxes  currentDirAxes {};  //- 当前正在处理的  方向值。（只在节点帧被改写）

    glm::dvec2 targetDPos  {};

    F_RenderUpdate renderUpdataFunc {nullptr}; //- functor
                                               //- 只在初始化阶段绑定，也许未来是可以切换的，但目前未实现

    //===== flags =====//
    bool   isMoving {false};
};


#endif 

