/*
 * ========================= Move.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.13
 *                                        修改 -- 2019.01.13
 * ----------------------------------------------------------
 *   Crawl / Fly 整合管理类
 * ----------------------------
 */
#ifndef _TPR_MOVE_H_
#define _TPR_MOVE_H_


//-------------------- Engine --------------------//
#include "Crawl.h"
#include "Fly.h"
#include "SpeedLevel.h"

//-- need --
class GameObj;


//-- 初级版本，在未来可能会发展成 数个 crawl实例 ／ 数个 fly实例
class Move{
public:
    Move() = default;

    inline void init( GameObj *_goPtr ){ //-- MUST --
        goPtr = _goPtr;
        crawl.init( (Move*)this ); 
        fly.init(   (Move*)this );
    }

    inline void RenderUpdate(){
        (is_crawl_==true) ? crawl.RenderUpdate() : fly.RenderUpdate();
    }

    //---- set ----//

    //-- "放置go实例到map" 的制定函数 --
    //  在调用此函数之前，应先确保 潜在碰撞区的 干净
    inline void set_initPos( const MapCoord &_pos ){

        //...



    }



    //-- 简单设置 currentFPos 的值 --
    //  禁止用此函数来 “放置go实例到map”，改用 set_initPos()
    //
    inline void set_currentFPos( const glm::vec2 &_fpos ){
        currentFPos = _fpos;
    }

    //-- 对 currentFPos 进行 累加累减 --
    inline void accum_currentFPos( const glm::vec2 &_fpos ){
        currentFPos += _fpos;
    }
    inline void accum_currentFPos( float _x, float _y ){
        currentFPos.x += _x;
        currentFPos.y += _y;
    }

    inline void set_MoveType( bool _is_crawl ){
        is_crawl_ = _is_crawl;
    }

    inline bool is_crawl() const {
        return is_crawl_;
    }

    inline void set_speedLv( SpeedLevel _lv ){
        speedLv = _lv;
    }

    inline SpeedLevel get_speedLv(){
        return speedLv;
    }

    inline void set_newCrawlDir( const NineBox &_newNB ){
        assert( is_crawl_ == true );
        crawl.set_newCrawlDir( _newNB );
    }

    inline void set_newflyIns( const FlyIns &_newFlyIns ){
        assert( is_crawl_ == false );
        fly.set_newflyIns( _newFlyIns );
    }

    inline void set_maskCountLimit( int _limit ){
        assert( is_crawl_ == false );
        fly.set_maskCountLimit( _limit );
    }

    //---- get ----//
    inline const glm::vec2 &get_currentFPos() const {
        return currentFPos;
    }

private:
    GameObj  *goPtr {nullptr}; //- 每个 fly实例 都属于一个 go实例
                                //  两者 强关联，共存亡

    glm::vec2  currentFPos {};  //- 当前帧 pos，float，不一定对齐与mapent
                                //- based on go.rootAnchor

    Crawl   crawl   {}; //- 未来可能被拓展为 一组 crawl实例
    Fly     fly     {}; //- 未来可能被拓展为 一组 fly实例
    bool    is_crawl_  {true};

    SpeedLevel   speedLv  { SpeedLevel::LV_3 }; //- 未来可能被拓展为 一组 speedLv数据集

};


#endif 

