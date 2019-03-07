/*
 * ====================== MapCoord.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   mapEntPos / pixPos  2-axis
 *  --------------------------
 *  游戏中用到的 各种 pos:
 *   FPos   - float   pos
 *	 PPos   - pixel   pos
 *	 MPos   - mapEnt  pos
 *	 FDPos  - field   pos
 *	 CPos   - chunk   pos 
 *	 SPos   - section pos (未实现)
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
#include <math.h>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "config.h" 


//-- [left-bottom] --
class MapCoord{
public:
    //---- constructor -----//
    MapCoord() = default;
    //-- 只支持 mpos初始化，若想用 ppos来初始化，先用 ppos_2_mpos() 转换
    explicit MapCoord( const IntVec2 &_mpos ):
        mpos(_mpos),
        ppos( mpos * PIXES_PER_MAPENT )
        {}
    MapCoord( int _mpos_x, int _mpos_y ):
        mpos( IntVec2{ _mpos_x, _mpos_y } ),
        ppos( mpos * PIXES_PER_MAPENT )
        {}
    
    //---- clear -----//
    inline void clear_all(){
        mpos.clear_all();
        ppos.clear_all();
    }

    //----- set ------//
    inline void set_by_mpos( const IntVec2 &_mpos ){
        mpos = _mpos;
        ppos = mpos * PIXES_PER_MAPENT;
    }

    inline void set_by_mpos( int _x, int _y ){
        mpos.set( _x, _y );
        ppos = mpos * PIXES_PER_MAPENT;
    }

    inline void set_by_ppos( const IntVec2 &_ppos ){
        assert( (_ppos.x%PIXES_PER_MAPENT==0) && (_ppos.y%PIXES_PER_MAPENT==0) );
        ppos = _ppos;
        mpos = ppos.floorDiv( (float)PIXES_PER_MAPENT );
    }

    inline void set_by_ppos( int _x, int _y ){
        assert( (_x%PIXES_PER_MAPENT==0) && (_y%PIXES_PER_MAPENT==0) );
        ppos.set( _x, _y );
        mpos = ppos.floorDiv( (float)PIXES_PER_MAPENT );
    }

    //--- get ---
    inline const IntVec2& get_mpos() const {
        return mpos;
    }

    inline const IntVec2& get_ppos() const{
        return ppos;
    }

    inline const glm::vec2 get_midFPos() const { //- ppos of the mid_box
        return glm::vec2{   (float)ppos.x + HALF_PIXES_PER_MAPENT,
                            (float)ppos.y + HALF_PIXES_PER_MAPENT };
    }

    inline const glm::vec2 get_fpos() const {
        return glm::vec2{ (float)ppos.x, (float)ppos.y };
    }

private:
    IntVec2   mpos  {0, 0}; //- based on mapEnt
    IntVec2   ppos  {0, 0}; //- based on pixel

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


/* ===========================================================
 *                   ppos_2_mpos  [严格对齐]
 * -----------------------------------------------------------
 * -- 参数 _ppos 必须对齐于 mapent坐标系
 */
inline IntVec2 ppos_2_mpos( const IntVec2 &_ppos ){
    assert( (_ppos.x%PIXES_PER_MAPENT==0) && (_ppos.y%PIXES_PER_MAPENT==0) );
    return floorDiv( _ppos, (float)PIXES_PER_MAPENT );
}

/* ===========================================================
 *                   ppos_2_mpos  [宽松]    IMPORTANT !!!
 * -----------------------------------------------------------
 * -- 参数 _fpos 可以为任意值。 无需对齐于 mapent坐标系
 * -- 
 */
inline MapCoord fpos_2_mcpos( const glm::vec2 &_fpos ){

    //-- float除法
    float fx = _fpos.x / (float)PIXES_PER_MAPENT;
    float fy = _fpos.y / (float)PIXES_PER_MAPENT;
    //-- math.floor() 
    return MapCoord{    (int)floor(fx), 
                        (int)floor(fy) };
}

/* ===========================================================
 *             mpos_2_ppos / mpos_2_fpos  
 * -----------------------------------------------------------
 */
inline IntVec2 mpos_2_ppos( const IntVec2 &_mpos ){
    return (_mpos*PIXES_PER_MAPENT);
}
inline glm::vec2 mpos_2_fpos( const IntVec2 &_mpos ){
    return glm::vec2{   (float)(_mpos.x*PIXES_PER_MAPENT),
                        (float)(_mpos.y*PIXES_PER_MAPENT) };
}

/* ===========================================================
 *                mpos_2_midPPos
 * -----------------------------------------------------------
 * -- 获得 mapent 中间pixel 的 ppos 
 */
inline IntVec2 mpos_2_midPPos( const IntVec2 &_mpos ){
    return IntVec2{ _mpos.x*PIXES_PER_MAPENT + MID_PPOS_IDX_IN_MAPENT,
                    _mpos.y*PIXES_PER_MAPENT + MID_PPOS_IDX_IN_MAPENT };
}
















#endif 

