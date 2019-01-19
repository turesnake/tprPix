/*
 * ====================== MapCoord.h =======================
 *                          -- tpr --
 *                                        创建 -- 2019.01.09
 *                                        修改 -- 2019.01.09
 * ----------------------------------------------------------
 *   mapEntPos / pixPos  2-axis
 * ----------------------------
 */
#ifndef _TPR_MAP_COORD_H_
#define _TPR_MAP_COORD_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "PixVec.h" 
#include "config.h" 


//-- [left-bottom] --
class MapCoord{
public:
    //---- constructor -----//
    MapCoord() = default;

    explicit MapCoord( const PixVec2 &_mpos ):
        mpos(_mpos),
        ppos( mpos * PIXES_PER_MAPENT )
        {}

    //---- clear -----//
    inline void clear_all(){
        mpos.clear_all();
        ppos.clear_all();
    }

    //----- set ------//
    inline void set_by_mpos( const PixVec2 &_mpos ){
        mpos = _mpos;
        ppos = mpos * PIXES_PER_MAPENT;
    }

    inline void set_by_mpos( int _x, int _y ){
        mpos.set( _x, _y );
        ppos = mpos * PIXES_PER_MAPENT;
    }

    inline void set_by_ppos( const PixVec2 &_ppos ){
        assert( (_ppos.x%PIXES_PER_MAPENT==0) && (_ppos.y%PIXES_PER_MAPENT==0) );
        ppos = _ppos;
        mpos = ppos / PIXES_PER_MAPENT;
    }

    inline void set_by_ppos( int _x, int _y ){
        assert( (_x%PIXES_PER_MAPENT==0) && (_y%PIXES_PER_MAPENT==0) );
        ppos.set( _x, _y );
        mpos = ppos / PIXES_PER_MAPENT;
    }

    //--- get ---
    inline const PixVec2& get_mpos() const {
        return mpos;
    }

    inline const PixVec2& get_ppos() const{
        return ppos;
    }

    inline glm::vec2 get_ppos_glm() const {
        return glm::vec2( (float)ppos.x, (float)ppos.y );
    }

    //--- add ---

private:
    PixVec2   mpos  {0, 0}; //- based on mapEnt
    PixVec2   ppos  {0, 0}; //- based on pixel
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator == ( const MapCoord &_a, const MapCoord &_b ){
    return ( _a.get_mpos() == _b.get_mpos() );
}
inline bool operator != ( const MapCoord &_a, const MapCoord &_b ){
    return ( _a.get_mpos() != _b.get_mpos() );
}

/* ===========================================================
 *                 operator <
 * -----------------------------------------------------------
 * -- 通过这个 "<" 运算符重载，MapCoord 类型将支持 set.find() 
 */
inline bool operator < ( const MapCoord &_a, const MapCoord &_b ){
    return ( _a.get_mpos() < _b.get_mpos() );
}

/* ===========================================================
 *                 operator +, -
 * -----------------------------------------------------------
 */
inline MapCoord operator + ( const MapCoord &_a, const MapCoord &_b ){
    return MapCoord { _a.get_mpos() + _b.get_mpos() };
}
inline MapCoord operator - ( const MapCoord &_a, const MapCoord &_b ){
    return MapCoord { _a.get_mpos() - _b.get_mpos() };
}




#endif 

