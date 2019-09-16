/*
 * ====================== SignInMapEnts.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SignInMapEnts_H
#define TPR_SignInMapEnts_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <functional>
#include <vector>
#include <set>
#include <utility> // pair


//-------------------- Engine --------------------//
#include "config.h" 
#include "IntVec.h"
#include "MapCoord.h"




class SignInMapEnts{
    using F_get_colliPointDPosOffsRef = std::function<const std::vector<glm::dvec2> &()>;
public:

    SignInMapEnts() = default;


    //-- only call in go init --
    inline void init_datas( const glm::dvec2 &newRootAnchorDPos_,
                            F_get_colliPointDPosOffsRef func_1_ )noexcept{

        this->get_colliPointDPosOffsRefFunc = func_1_;                     
        bool out = this->forecast_signINMapEnts( newRootAnchorDPos_ );
        tprAssert( out );
        this->sync_currentSignINMapEnts_from_future();
    }


    inline void rebind_datas( const glm::dvec2 &newRootAnchorDPos_,
                            F_get_colliPointDPosOffsRef func_1_ )noexcept{

        this->get_colliPointDPosOffsRefFunc = func_1_;                     
        bool out = this->forecast_signINMapEnts( newRootAnchorDPos_ );
            //tprAssert( out );
        this->sync_currentSignINMapEnts_from_future();
    }


    //------------------------------------//
    //--1-- 当一个 regularGo 新生成时，传入参数{0.0,0.0}，制作最初的 SignINMapEnts 数据，
    //      然后 call sync_currentSignINMapEnts_from_new()
    //--2-- 每一移动帧, 调用此函数来生成 addsDels,
    //      然后做碰撞检测。在此期间，这个函数可能被调用多次，但都不会改写 currentSignINMapEnts
    //      ----
    //      直到确认位移后，才调用 sync_currentSignINMapEnts_from_new()，同步数据
    //-----
    // param: newRootAnchorDPos_ = currentDPos + speedDPos_
    inline bool forecast_signINMapEnts( const glm::dvec2 &newRootAnchorDPos_ )noexcept{
    
        if( !this->is_forecast_called ){
            this->is_forecast_called = true;
        }
        
        this->futureAdds.clear();
        this->futureDels.clear();
        this->futureSignINMapEnts.clear();
        //-- update news --
        for( const auto &i : this->get_colliPointDPosOffsRefFunc() ){
            this->futureSignINMapEnts.insert( dpos_2_mpos(i+newRootAnchorDPos_) );
        }
        //-- adds --
        for( const auto &i : this->futureSignINMapEnts ){
            if( this->currentSignINMapEnts.find(i) == this->currentSignINMapEnts.end() ){
                this->futureAdds.push_back( i );//- copy
            }
        }
        //-- dels --
        for( const auto &i : this->currentSignINMapEnts ){
            if( this->futureSignINMapEnts.find(i) == this->futureSignINMapEnts.end() ){
                this->futureDels.push_back( i );//- copy
            }
        }
        //-- return --
        if( this->futureAdds.empty() && this->futureDels.empty() ){
            return false;
        }else{
            return true;
        }
    }


    //-- 同步数据 --
    inline void sync_currentSignINMapEnts_from_future()noexcept{
        tprAssert( this->is_forecast_called );
        this->is_forecast_called = false;

        //- only swap when change happened
        //  确保在调用此函数时，addsdels 一定发生了更新
        tprAssert( !(this->futureAdds.empty() && this->futureDels.empty()) );

        this->currentAdds.swap( this->futureAdds );
        this->currentDels.swap( this->futureDels );
        this->currentSignINMapEnts.swap( this->futureSignINMapEnts ); 
        //-- new容器 会在下次 调用 forecast_signINMapEnts 时自动清理 --
    }


    inline const std::set<IntVec2> &get_currentSignINMapEntsRef() const noexcept{ return this->currentSignINMapEnts; }
    inline const std::set<IntVec2> &get_futureSignINMapEntsRef()  const noexcept{ return this->futureSignINMapEnts; }

    inline const std::vector<IntVec2> &get_currentAddsRef() const noexcept{ return this->currentAdds; }
    inline const std::vector<IntVec2> &get_currentDelsRef() const noexcept{ return this->currentDels; }

private:

    F_get_colliPointDPosOffsRef  get_colliPointDPosOffsRefFunc {nullptr};

    //--- current ---//
    std::set<IntVec2> currentSignINMapEnts {};  //- 每一帧移动后，go都会及时更新自己当前登记的 mapents
    std::vector<IntVec2> currentAdds {}; //- 每帧移动后，将要被新登记的 mapents
    std::vector<IntVec2> currentDels {}; //- 每帧移动后，将要被取消登记的 mapents

    //--- future ---//
    // each time call forecast_(), the future datas will be update
    // only sync to current datas when sync_() is called
    std::set<IntVec2> futureSignINMapEnts {};
    std::vector<IntVec2> futureAdds       {}; 
    std::vector<IntVec2> futureDels       {};

    //----- flags -----//
    bool is_forecast_called {false}; //- 协调 forecast 和 sync 两个函数的 次序。
                                    // forecast 可以被调用多次
                                    // 每次调用 sync 之前，forecast 至少要被调用一次

};


#endif 

