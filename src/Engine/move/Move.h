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



//-- 初级版本，在未来可能会发展成 数个 crawl实例 ／ 数个 fly实例
class Move{
public:
    void init( GameObj *_goPtr ); //-- MUST --
    void RenderUpdate(); 

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

    inline void set_newCrawlIns( const CrawlIns &_newCI ){
        assert( is_crawl_ == true );
        crawl.set_newCrawlIns( _newCI );
    }

    inline void set_newflyIns( const FlyIns &_newFlyIns ){
        assert( is_crawl_ == false );
        fly.set_newflyIns( _newFlyIns );
    }

    inline void set_maskCountLimit( int _limit ){
        assert( is_crawl_ == false );
        fly.set_maskCountLimit( _limit );
    }


private:
    GameObj  *goPtr {nullptr}; //- 每个 fly实例 都属于一个 go实例
                                //  两者 强关联，共存亡

    Crawl   crawl   {}; //- 未来可能被拓展为 一组 crawl实例
    Fly     fly     {}; //- 未来可能被拓展为 一组 fly实例
    bool    is_crawl_  {true};

    SpeedLevel   speedLv  { SpeedLevel::LV_3 }; //- 未来可能被拓展为 一组 speedLv数据集

    

};


#endif 

