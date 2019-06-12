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
#ifndef TPR_MAP_COORD_H_
#define TPR_MAP_COORD_H_

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 
#include "config.h" 


/* ===========================================================
 *             mpos_2_ppos / mpos_2_fpos  
 * -----------------------------------------------------------
 * --  放在前面，因为会被 class MapCoord 用到
 */
inline const IntVec2 mpos_2_ppos( const IntVec2 &_mpos ){
    return (_mpos*PIXES_PER_MAPENT);
}
inline const glm::vec2 mpos_2_fpos( const IntVec2 &_mpos ){
    return glm::vec2{   static_cast<float>(_mpos.x*PIXES_PER_MAPENT),
                        static_cast<float>(_mpos.y*PIXES_PER_MAPENT) };
}


//-- [left-bottom] --
class MapCoord{
public:
    //---- constructor -----//
    MapCoord() = default;
    //-- 只支持 mpos初始化，若想用 ppos来初始化，先用 anyPPos_2_mpos() 转换
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
        this->mpos.clear_all();
        this->ppos.clear_all();
    }

    //----- set ------//
    inline void set_by_mpos( const IntVec2 &_mpos ){
        this->mpos = _mpos;
        this->ppos = mpos_2_ppos( this->mpos );
    }

    inline void set_by_mpos( int _x, int _y ){
        this->mpos.set( _x, _y );
        this->ppos = mpos_2_ppos( this->mpos );
    }

    //-- 新版 crawl 方案实施后，对齐于 mpos 的 go 几乎不存在了 
    //   需要一组更加 完备的函数
    //   -------
    //   还是需要使用 这个 严谨版
    inline void set_by_ppos_( const IntVec2 &_ppos ){
            tprAssert( (_ppos.x%PIXES_PER_MAPENT==0) && (_ppos.y%PIXES_PER_MAPENT==0) );
        this->ppos = _ppos;
        this->mpos = floorDiv( this->ppos, static_cast<float>(PIXES_PER_MAPENT) );
    }
    inline void set_by_ppos_( int _x, int _y ){
            tprAssert( (_x%PIXES_PER_MAPENT==0) && (_y%PIXES_PER_MAPENT==0) );
        this->ppos.set( _x, _y );
        this->mpos = floorDiv( this->ppos, static_cast<float>(PIXES_PER_MAPENT) );
    }

    //-- 宽松版，目前仅被 gameObj_mem.cpp -> signUp_newGO_to_mapEnt() 使用 --
    inline void set_by_anyPPos( const IntVec2 &_anyPPos ){
        this->mpos = _anyPPos.floorDiv( static_cast<float>(PIXES_PER_MAPENT) );
        this->ppos = mpos_2_ppos( this->mpos );
    }
    inline void set_by_anyPPos( int _x, int _y ){
        this->mpos = floorDiv( IntVec2{_x, _y}, static_cast<float>(PIXES_PER_MAPENT) );
        this->ppos = mpos_2_ppos( this->mpos );
    }
    

    inline void accum_mpos( const IntVec2 &_mposOff ){
        this->mpos += _mposOff;
        this->ppos = mpos_2_ppos( this->mpos );
    }


    //--- get ---
    inline const IntVec2 &get_mpos() const {
        return this->mpos;
    }

    inline const IntVec2 &get_ppos() const{
        return this->ppos;
    }

    inline const glm::vec2 get_midFPos() const { //- ppos of the mid_box
        IntVec2 ppos_l = mpos_2_ppos( this->mpos );
        return glm::vec2{   static_cast<float>(ppos_l.x + HALF_PIXES_PER_MAPENT),
                            static_cast<float>(ppos_l.y + HALF_PIXES_PER_MAPENT) };
    }

    inline const glm::vec2 get_fpos() const {
        IntVec2 ppos_l = mpos_2_ppos( this->mpos );
        return glm::vec2{ static_cast<float>(ppos_l.x), 
                          static_cast<float>(ppos_l.y) };
    }

    //-- 仅用于 crawl --
    //   mpos 值是否在 [-1,1] 之间
    inline bool is_match_with_nineBox() const {
        return (    (std::abs(this->mpos.x) <= 1) && (std::abs(this->mpos.x) >= -1) &&
                    (std::abs(this->mpos.y) <= 1) && (std::abs(this->mpos.y) >= -1) );
    }



private:
    IntVec2   mpos  {0, 0}; //- based on mapEnt
    IntVec2   ppos  {0, 0}; //- 必须指向 mpos 左下角
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
inline const MapCoord operator + ( const MapCoord &_a, const MapCoord &_b ){
    return MapCoord { _a.get_mpos() + _b.get_mpos() };
}
inline const MapCoord operator - ( const MapCoord &_a, const MapCoord &_b ){
    return MapCoord { _a.get_mpos() - _b.get_mpos() };
}


/* ===========================================================
 *                   ppos_2_mpos  [严格对齐]
 * -----------------------------------------------------------
 * -- 参数 _ppos 必须对齐于 mapent坐标系
 */
inline const IntVec2 ppos_2_mpos( const IntVec2 &_ppos ){
    tprAssert( (_ppos.x%PIXES_PER_MAPENT==0) && (_ppos.y%PIXES_PER_MAPENT==0) );
    return floorDiv( _ppos, static_cast<float>(PIXES_PER_MAPENT) );
}

inline const IntVec2 anyPPos_2_mpos( const IntVec2 &_anyPPos ){
    return floorDiv( _anyPPos, static_cast<float>(PIXES_PER_MAPENT) );
}

/* ===========================================================
 *                   fpos_2_mcpos  [宽松]    IMPORTANT !!!
 * -----------------------------------------------------------
 * -- 参数 _fpos 可以为任意值。 无需对齐于 mapent坐标系
 */
inline const MapCoord fpos_2_mcpos( const glm::vec2 &_fpos ){
    //-- float除法
    float fx = _fpos.x / static_cast<float>(PIXES_PER_MAPENT);
    float fy = _fpos.y / static_cast<float>(PIXES_PER_MAPENT);
    //-- math.floor() 
    return MapCoord{    static_cast<int>(floor(fx)), 
                        static_cast<int>(floor(fy)) };
}

inline const IntVec2 fpos_2_mpos( const glm::vec2 &_fpos ){
    //-- float除法
    float fx = _fpos.x / static_cast<float>(PIXES_PER_MAPENT);
    float fy = _fpos.y / static_cast<float>(PIXES_PER_MAPENT);
    //-- math.floor() 
    return IntVec2{ static_cast<int>(floor(fx)), 
                    static_cast<int>(floor(fy)) };
}



/* ===========================================================
 *                mpos_2_midPPos
 * -----------------------------------------------------------
 * -- 获得 mapent 中间pixel 的 ppos 
 */
inline const IntVec2 mpos_2_midPPos( const IntVec2 &_mpos ){
    return IntVec2{ _mpos.x*PIXES_PER_MAPENT + MID_PPOS_IDX_IN_MAPENT,
                    _mpos.y*PIXES_PER_MAPENT + MID_PPOS_IDX_IN_MAPENT };
}


/* ===========================================================
 *             calc_fast_ppos_distance
 * -----------------------------------------------------------
 * 计算两个 ppos 的距离（快速版，不开根号）
 * 返回的结果，只是一个 “含糊的距离概念” [主要用来 生成 cell-noise]
 */
inline int calc_fast_ppos_distance( const IntVec2 &_aPPos, const IntVec2 &_bPPos ){
    IntVec2 off = _aPPos - _bPPos;
        //-- 没有做 溢出检测...
    return (off.x*off.x + off.y*off.y);
}

/* ===========================================================
 *               calc_fast_mpos_distance
 * -----------------------------------------------------------
 * 计算两个 ppos 的距离（快速版，不开根号）
 * 和上一个函数并没有本质区别。
 * 返回的结果，只是一个 “含糊的距离概念” [主要用来 生成 cell-noise]
 */
inline int calc_fast_mpos_distance( const IntVec2 &_aMPos, const IntVec2 &_bMPos ){
    IntVec2 off = _aMPos - _bMPos;
        //-- 没有做 溢出检测...
    return (off.x*off.x + off.y*off.y);
}


#endif 

