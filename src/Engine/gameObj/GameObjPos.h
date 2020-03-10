/*
 * ========================= GameObjPos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.20
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GAME_OBJ_POS_H
#define TPR_GAME_OBJ_POS_H

#include "pch.h"

//-------------------- CPP --------------------//
#include <utility> // pair

//-------------------- Engine --------------------//
#include "MapCoord.h"
#include "GoAltiRange.h"


//-- based on go.rootAnchor 
class GameObjPos{
public:
    GameObjPos() = default;

    inline void init( const glm::dvec2 &dpos_ )noexcept{
        this->currentDPos = dpos_;
    }

    //------- set -------//
    inline void set_alti( double alti_ )noexcept{ this->alti = alti_; }
    inline void set_lAltiRange ( GoAltiRange new_ )noexcept{ this->lAltiRange = new_; }
    inline void accum_dpos( const glm::dvec2 &addDPos_ )noexcept{ this->currentDPos += addDPos_; }

    //------- get -------//
    inline const glm::dvec2 &get_dpos()const noexcept{ return this->currentDPos; }
    inline double           get_alti()const noexcept{ return this->alti; }
    inline GoAltiRange      get_lAltiRange()const noexcept{ return this->lAltiRange; }

private:
    glm::dvec2   currentDPos  {}; // rootAnchor 本点 当前dpos，(无需对齐与mapent)
    double       alti         {0.0}; //- 腾空高度。
    GoAltiRange  lAltiRange   {};  // 碰撞体 高度区间
};


#endif 

