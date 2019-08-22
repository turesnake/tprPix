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
    using F_Get_Off_From_RootAnchor_2_MapEntMid = std::function<const glm::dvec2 &()>;

    GameObjPos() = default;

    // 需要2段init：
    // --1-- 调用 init(); 此时，currentDPos 并未设置
    // --2-- 调用 init_currentDPos(); 正式设置 currentDPos。
    inline void init(   const IntVec2 mpos_,
                        const IntVec2 pposOff_,
                        const F_Get_RootAnchorCompass &func_1_,
                        const F_Get_Off_From_RootAnchor_2_MapEntMid &func_2_ ){
        this->currentMCPos.set_by_mpos( mpos_ );
        this->tmpPPosOff = pposOff_; //- 仅做临时存储
        this->get_rootAnchorCompass_functor = func_1_;
        this->get_off_from_rootAnchor_2_mapEntMid_functor = func_2_;
    }

    inline void init_currentDPos(){        
        IntVec2 p = this->currentMCPos.get_ppos() + 
                    this->tmpPPosOff +
                    this->get_rootAnchorCompass_functor().to_IntVec2();
        this->currentDPos.x = static_cast<double>(p.x);
        this->currentDPos.y = static_cast<double>(p.y);
    }

    //- 计算 go.rootAnchor 当前所在的 collient 的 midDPos
    //  也就是，currentDPos 在 ces 中所处的 mapent，的中点 fpos
    //  由于 currentDPos 不一定处于 ces 的 mapent 的正中心，不适合用来表示 当前go是否 跨mapent
    //  而这里的 midDPos 就非常适合。
    //  ---
    //  此值 和 currentMCPos 基于同一个点
    inline const glm::dvec2 calc_rootAnchor_midDPos(){
        return ( this->currentDPos + this->get_off_from_rootAnchor_2_mapEntMid_functor() );
    }
    
    //------- set -------//
    inline void set_alti( double alti_ ){
        this->alti = alti_;
    }

    //-- 在 新版 move 系统, crawl模式中，更新 goPos --
    //-- 使用 NineBox 来传递参数，
    //   决定了当前模式下的 最大速度，不能超过 1_mapent_per_frame 
    //   想要突破这个限制，就要 进一步 完善 collision 函数
    //   让它支持，一次检测 数个 mapent
    inline void accum_current_dpos_and_mcpos(   const glm::dvec2 &dpos_,
                                                const NineBox &nbOff_,
                                                bool  isCross_ ){
        this->currentDPos += dpos_;
        if( isCross_ ){
            this->currentMCPos.accum_mpos( nbOff_.to_mpos() );
        }
    }

    //-- 用于 move.locate
    //   直接将一个 go，摆放到一个 dpos 上
    //inline void locate_by_dpos( const glm::dvec2 &dpos_ ){
    //}

    //------- get -------//

    inline const glm::dvec2 &get_currentDPos() const {
        return this->currentDPos;
    }

    inline const double &get_alti() const {
        return this->alti;
    }
    inline const IntVec2 &get_currentMPos() const {
        return this->currentMCPos.get_mpos();
    }
    //-- 用于 db 存储时 --
    inline IntVec2 calc_current_pposOff() const{
        return  dpos_2_ppos(this->currentDPos) - 
                this->currentMCPos.get_ppos() -
                this->get_rootAnchorCompass_functor().to_IntVec2();
    }

private:

    //--- currentDPos 和 currentMCPos 并不指向 同一个点 !!! ---
    // rootAnchor 和 rootAnchor 所在的 mapent 的 MidPPos 并不一定是 同一个点
    glm::dvec2  currentDPos  {};
                    // rootAnchor 本点 当前dpos，最适合用来表达 go 的坐标（尤其是对外，涉及图像时）(无需对齐与mapent)
                    // 那个理想中的 代表 gopos 的值  
    MapCoord    currentMCPos {};  
                    //- rootAnchor 所在 collient 的 MidPPos， 当前所在的 mapent
                    //  很多 rootAnchor 都不在 mapent的中心，所以无法直接代表 mapent的位置
                    //- 此值往往在 crawl的 回合中段 发生切换。
                    //  ---
                    //  currentMCPos 代表了 碰撞盒 的位置
                    //  由于 go 和 碰撞盒 处于 牵风筝 的关系
                    //  所以 这两个值 并不百分百同步

    double       alti         {0.0}; //- 腾空高度。

    F_Get_RootAnchorCompass                get_rootAnchorCompass_functor {nullptr};
    F_Get_Off_From_RootAnchor_2_MapEntMid  get_off_from_rootAnchor_2_mapEntMid_functor {nullptr};

    //--- tmp ---//
    IntVec2     tmpPPosOff   {};   //- init 阶段缓存值，禁止被外部使用！(不算是什么好方案...)
};




#endif 

