/*
 * ========================= MapEntCompass.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   ppos 在其所处的 mapent 中的 位置偏移 （左下角） 
 * ----------------------------
 */
#ifndef TPR_MAP_ENT_COMPASS_H
#define TPR_MAP_ENT_COMPASS_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "IntVec.h"


class MapEntCompass{
public:
    //---- constructor -----//
    MapEntCompass() = default;
    MapEntCompass( int x_, int y_ ):
        x(x_),
        y(y_)
        { tprAssert( (this->x >= 0) && (this->x < PIXES_PER_MAPENT) && 
                    (this->y >= 0) && (this->y < PIXES_PER_MAPENT) ); }

    //---- funcs ----//
    inline void clear_all(){
        this->x = 0;
        this->y = 0;
    }

    //- 只是简单转换为 IntVec2, 在用于 MapCoord 时要注意
    inline IntVec2 to_IntVec2() const {
        return IntVec2{ this->x, this->y };
    } 
    inline glm::dvec2 to_dpos() const {
        return glm::dvec2{ static_cast<double>(this->x), static_cast<double>(this->y) };
    }

    //======== vals ========//
    int x;  //- [0, 63]
    int y;  //- [0, 63]
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator == ( const MapEntCompass &a_, const MapEntCompass &b_ ){
    return ( (a_.x==b_.x) && (a_.y==b_.y) );
}
inline bool operator != ( const MapEntCompass &a_, const MapEntCompass &b_ ){
    return ( (a_.x!=b_.x) || (a_.y!=b_.y) );
}

/* ===========================================================
 *                 calc_ppos_compass
 * -----------------------------------------------------------
 * -- 传入一个 ppos，获得这个 ppos，在其mapent 中的位置 
 */
inline MapEntCompass calc_ppos_compass( const IntVec2 ppos_ ){
    return MapEntCompass {  ppos_.x%PIXES_PER_MAPENT, 
                            ppos_.y%PIXES_PER_MAPENT };
}


#endif 

