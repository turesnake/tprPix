/*
 * ========================= MapEntCompass.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   ppos 在其所处的 mapent 中的 位置偏移 （左下角） 
 * ----------------------------
 */
#ifndef TPR_MAP_ENT_COMPASS_H_
#define TPR_MAP_ENT_COMPASS_H_

//--- glm - - 0.9.9.5 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- C --------------------//
//#include <cassert>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "IntVec.h"


class MapEntCompass{
public:
    //---- constructor -----//
    MapEntCompass() = default;
    MapEntCompass( int _x, int _y ):
        x(_x),
        y(_y)
        { tprAssert(   (x >= 0) && (x < PIXES_PER_MAPENT) && 
                    (y >= 0) && (y < PIXES_PER_MAPENT) ); }

    //---- funcs ----//
    inline void clear_all(){
        x = 0;
        y = 0;
    }

    //- 只是简单转换为 IntVec2, 在用于 MapCoord 时要注意
    inline IntVec2 to_IntVec2() const {
        return IntVec2{ x, y };
    } 

    inline glm::vec2 to_fpos() const {
        return glm::vec2{ (float)x, (float)y };
    }

    //======== vals ========//
    int x;  //- [0, 4]
    int y;  //- [0, 4]
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator == ( const MapEntCompass &_a, const MapEntCompass &_b ){
    return ( (_a.x==_b.x) && (_a.y==_b.y) );
}
inline bool operator != ( const MapEntCompass &_a, const MapEntCompass &_b ){
    return ( (_a.x!=_b.x) || (_a.y!=_b.y) );
}

/* ===========================================================
 *                 calc_ppos_compass
 * -----------------------------------------------------------
 * -- 传入一个 ppos，获得这个 ppos，在其mapent 中的位置 
 */
inline MapEntCompass calc_ppos_compass( const IntVec2 _ppos ){
    return MapEntCompass {  _ppos.x%PIXES_PER_MAPENT, 
                            _ppos.y%PIXES_PER_MAPENT };
}


#endif 

