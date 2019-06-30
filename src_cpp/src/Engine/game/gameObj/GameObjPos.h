/*
 * ========================= GameObjPos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.20
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GAME_OBJ_POS_H
#define TPR_GAME_OBJ_POS_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "config.h" 
#include "IntVec.h"
#include "MapCoord.h"
#include "MapEntCompass.h"
#include "NineBox.h"


//-- based on go.rootAnchor 
class GameObjPos{
public:
    using F_Get_RootAnchorCompass = std::function<const MapEntCompass &()>;
    using F_Get_Off_From_RootAnchor_2_MapEntMid = std::function<const glm::vec2 &()>;

    GameObjPos() = default;

    inline void init( const F_Get_RootAnchorCompass &func_1_,
                      const F_Get_Off_From_RootAnchor_2_MapEntMid &func_2_ ){
        this->get_rootAnchorCompass_functor = func_1_;
        this->get_off_from_rootAnchor_2_mapEntMid_functor = func_2_;
    }

    //-- 将 go.goPos.currentFPos 放到 参数 _pos 指定的 mpos 上
    //   在调用此函数之前，应先确保 潜在碰撞区的 干净
    //   此函数不需要什么性能，反正不会经常设置 go的 mcpos
    inline void init_by_currentMPos( const IntVec2 &mpos_ ){
        this->currentMCPos.set_by_mpos( mpos_ );
        //---
        const MapEntCompass &compassRef = this->get_rootAnchorCompass_functor();
        IntVec2 p = this->currentMCPos.get_ppos() + compassRef.to_IntVec2();
        this->currentFPos.x = static_cast<float>(p.x);
        this->currentFPos.y = static_cast<float>(p.y);
    }


    //- 计算 go.rootAnchor 当前所在的 collient 的 midFPos
    //  也就是，currentFPos 在 ces 中所处的 mapent，的中点 fpos
    //  由于 currentFPos 不一定处于 ces 的 mapent 的正中心，不适合用来表示 当前go是否 跨mapent
    //  而这里的 midFPos 就非常适合。
    inline const glm::vec2 calc_rootAnchor_midFPos(){
        return ( this->currentFPos + this->get_off_from_rootAnchor_2_mapEntMid_functor() );
    }
    

    //------- set -------//
    inline void set_alti( float alti_ ){
        this->alti = alti_;
    }


    //---- 临时函数，有问题.... ----
    //  仅用于 crawl
    inline void accum_currentFPos_2( const glm::vec2 &fpos_ ){
        this->currentFPos += fpos_;
    }
    inline void accum_currentMCPos_2( const NineBox &nbOff_ ){
        this->currentMCPos.accum_mpos( nbOff_.to_mpos() );
    }


    //------- get -------//
    inline const glm::vec2 &get_currentFPos() const {
        return this->currentFPos;
    }
    inline const float &get_alti() const {
        return this->alti;
    }
    inline const MapCoord &get_currentMCPos() const {
        return this->currentMCPos;
    }
    inline const IntVec2 &get_currentMPos() const {
        return this->currentMCPos.get_mpos();
    }



    //-- 一个为了验证功能，临时实现的函数， 在未来，被优化取代 .......................
    inline const IntVec2 get_rootAnchorCompassPPos() const {
        const auto &compass = this->get_rootAnchorCompass_functor();
        return IntVec2{ compass.x, compass.y};
    }                           



    //-- 注意，这个函数返回的是 currentFPos 的整形值（将丢失小数）
    //  只可用于：明确知道 当前 rootAnchor所在 collient 已对齐于 mapent 时。
    //  比如在 crawl 节点帧
    inline const IntVec2 get_currentPPos() const {
        return IntVec2{ static_cast<int>(this->currentFPos.x), 
                        static_cast<int>(this->currentFPos.y) };
    }

private:
    glm::vec2   currentFPos  {};  //- 基于 go.rootAnchor 的， 当前 fpos，无需对齐与mapent
    MapCoord    currentMCPos {};  //- rootAnchor所在的 collient 的中点， 当前所在的 mapent
                                  //  很多 rootAnchor 都不在 mapent的中心，所以无法直接代表 mapent的位置
                                  //- 此值往往在 crawl的 回合中段 发生切换。
    float       alti         {0.0f}; //- 腾空高度。
    //-- 这个值暂时 没被使用 --
    //glm::vec2   targetFPos  { 0.0f, 0.0f };  //- 基于 go.rootAnchor 的，目标 fpos，无需对齐与mapent
                                //- 此值的用途很多样，有待开发...
                                //  目前版本中，主要用于 crawl 节点帧 对齐。
                                //  用来记录 新回合的 最终 位移绝对地址

    F_Get_RootAnchorCompass                get_rootAnchorCompass_functor {nullptr};
    F_Get_Off_From_RootAnchor_2_MapEntMid  get_off_from_rootAnchor_2_mapEntMid_functor {nullptr};

};




#endif 

