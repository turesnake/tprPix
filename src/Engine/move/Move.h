/*
 * ========================= Move.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   Crawl / Fly 整合管理类
 * ----------------------------
 */
#ifndef _TPR_MOVE_H_
#define _TPR_MOVE_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "Crawl.h"
#include "Fly.h"
#include "TargetDrag.h"
#include "SpeedLevel.h"
#include "AnchorPos.h"
#include "MapCoord.h"

//-- need --
class GameObj;
class GameObjPos;
class Collision;



enum class MoveType : int {
    Crawl,
    Fly,
    TargetDrag //- 类似 camera的运动方式：go快速向 目标pos 靠拢
               //  被用于 UI 图标运动
};




//-- 初级版本，在未来可能会发展成 数个 crawl实例 ／ 数个 fly实例
class Move{
    using F_RenderUpdate = std::function< void() >;
public:
    Move() = default;

    //- 在 gameObj.init() 中被调用 --
    inline void init(   GameObj *_goPtr, 
                        GameObjPos *_goPosPtr,
                        Collision *_collisionPtr ){ //-- MUST --
        this->goPtr    = _goPtr;
        this->goPosPtr = _goPosPtr;

        //-- 这样实现有点浪费 --
        this->crawl.init( this->goPtr, 
                        (Move*)this, 
                        this->goPosPtr, 
                        _collisionPtr ); 

        this->fly.init( (Move*)this, 
                        goPosPtr );

        this->targetDrag.init(  (Move*)this,
                                goPosPtr );
    }

    inline void RenderUpdate(){
        this->renderUpdataFunc();
    }

    //------- flags -------//
    inline bool is_crawl() const {
        return (this->moveType==MoveType::Crawl);
    }

    //------- set -------//
    inline void set_MoveType( MoveType _type ){
        this->moveType = _type;
        switch ( _type ){
            case MoveType::Crawl:      
                this->renderUpdataFunc = std::bind( &Crawl::RenderUpdate, &this->crawl ); 
                return;
            case MoveType::Fly:        
                this->renderUpdataFunc = std::bind( &Fly::RenderUpdate, &this->fly ); 
                return;
            case MoveType::TargetDrag: 
                this->renderUpdataFunc = std::bind( &TargetDrag::RenderUpdate, &this->targetDrag ); 
                return;
            default:
                assert(0);
                return;
        }
    }
    inline void set_speedLv( SpeedLevel _lv ){
        this->speedLv = _lv;
    }
    inline void set_newCrawlDir( const NineBox &_newNB ){
        assert( this->moveType == MoveType::Crawl );
        crawl.set_newCrawlDir( _newNB );
    }
    inline void set_newflyIns( const FlyIns &_newFlyIns ){
        assert( this->moveType == MoveType::Fly );
        this->fly.set_newflyIns( _newFlyIns );
    }
    inline void set_maskCountLimit( int _limit ){
        assert( this->moveType == MoveType::Fly );
        this->fly.set_maskCountLimit( _limit );
    }
    inline void set_targetDrag_targetFPos( const glm::vec2 &_FPos ){
        assert( this->moveType == MoveType::TargetDrag );
        this->targetDrag.set_targetFPos( _FPos );
    }

    //------- get -------//
    inline const SpeedLevel &get_speedLv() const {
        return this->speedLv;
    }
    inline bool is_targetDrag_moving() const {
        assert( this->moveType == MoveType::TargetDrag );
        return this->targetDrag.is_moving();
    }            //-- 这个函数 实现得 很潦草 ...

private:
    GameObj     *goPtr    {nullptr}; //- 每个 fly实例 都属于一个 go实例, 强关联
    GameObjPos  *goPosPtr {nullptr};

    Crawl       crawl   {}; //- 未来可能被拓展为 一组 crawl实例
    Fly         fly     {}; //- 未来可能被拓展为 一组 fly实例
    TargetDrag  targetDrag {}; //- 目前主用于 ui go

    F_RenderUpdate renderUpdataFunc {nullptr}; //- functor
                                               //- 只在初始化阶段绑定，也许未来是可以切换的，但目前未实现

    SpeedLevel   speedLv  { SpeedLevel::LV_3 }; //- 未来可能被拓展为 一组 speedLv数据集

    MoveType   moveType {MoveType::Crawl};

};


#endif 

