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
#include <vector>
#include <set>
#include <utility> // pair


//-------------------- Engine --------------------//
#include "config.h" 
#include "IntVec.h"
#include "MapCoord.h"


//-- based on go.rootAnchor 
class GameObjPos{
public:
    GameObjPos() = default;

    inline void init( const glm::dvec2 &dpos_ )noexcept{
        this->currentDPos = dpos_;
    }

    //------- set -------//
    inline void set_alti( double alti_ )noexcept{ this->alti = alti_; }
    inline void accum_dpos( const glm::dvec2 &addDPos_ )noexcept{ this->currentDPos += addDPos_; }

    //------- get -------//
    inline const glm::dvec2 &get_dpos() const noexcept{ return this->currentDPos; }
    inline double           get_alti() const noexcept{ return this->alti; }

private:
    glm::dvec2   currentDPos  {}; // rootAnchor 本点 当前dpos，(无需对齐与mapent)
    double       alti         {0.0}; //- 腾空高度。
};


#endif 

