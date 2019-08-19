/*
 * ====================== MapCoord.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   mapEntPos / pixPos  2-axis
 *  --------------------------
 *  some kinds of pos:
 *   FPos   - float   pos (pix)
 *   DPos   - double  pos (pix)
 *	 PPos   - pixel   pos
 *	 MPos   - mapEnt  pos
 *	 FDPos  - field   pos
 *	 CPos   - chunk   pos 
 *	 SPos   - section pos (unimplement)
 * ----------------------------
 */
#ifndef TPR_MAP_COORD_H
#define TPR_MAP_COORD_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 
#include "config.h" 


/* ===========================================================
 *                  mpos_2_ppos
 * -----------------------------------------------------------
 * --  放在前面，因为会被 class MapCoord 用到
 */
inline const IntVec2 mpos_2_ppos( const IntVec2 &mpos_ ){
    return (mpos_*PIXES_PER_MAPENT);
}

//-- [left-bottom] --
class MapCoord{
public:
    //---- constructor -----//
    MapCoord() = default;
    //-- 只支持 mpos初始化，若想用 ppos来初始化，先用 anyPPos_2_mpos() 转换
    explicit MapCoord( const IntVec2 &mpos_ ):
        mpos(mpos_),
        ppos( mpos * PIXES_PER_MAPENT )
        {}
    MapCoord( int mpos_x_, int mpos_y_ ):
        mpos( IntVec2{ mpos_x_, mpos_y_ } ),
        ppos( mpos * PIXES_PER_MAPENT )
        {}
    
    //---- clear -----//
    inline void clear_all(){
        this->mpos.clear_all();
        this->ppos.clear_all();
    }

    //----- set ------//
    inline void set_by_mpos( const IntVec2 &mpos_ ){
        this->mpos = mpos_;
        this->ppos = mpos_2_ppos( this->mpos );
    }

    inline void set_by_mpos( int x_, int y_ ){
        this->mpos.set( x_, y_ );
        this->ppos = mpos_2_ppos( this->mpos );
    }

    //-- 新版 crawl 方案实施后，对齐于 mpos 的 go 几乎不存在了 
    //   需要一组更加 完备的函数
    //   -------
    //   还是需要使用 这个 严谨版
    inline void set_by_ppos_( const IntVec2 &ppos_ ){
            tprAssert( (ppos_.x%PIXES_PER_MAPENT==0) && (ppos_.y%PIXES_PER_MAPENT==0) );
        this->ppos = ppos_;
        this->mpos = floorDiv( this->ppos, static_cast<double>(PIXES_PER_MAPENT) );
    }
    inline void set_by_ppos_( int x_, int y_ ){
            tprAssert( (x_%PIXES_PER_MAPENT==0) && (y_%PIXES_PER_MAPENT==0) );
        this->ppos.set( x_, y_ );
        this->mpos = floorDiv( this->ppos, static_cast<double>(PIXES_PER_MAPENT) );
    }

    //-- 宽松版，目前仅被 gameObj_mem.cpp -> signUp_newGO_to_mapEnt() 使用 --
    inline void set_by_anyPPos( const IntVec2 &anyPPos_ ){
        this->mpos = anyPPos_.floorDiv( static_cast<double>(PIXES_PER_MAPENT) );
        this->ppos = mpos_2_ppos( this->mpos );
    }
    inline void set_by_anyPPos( int x_, int y_ ){
        this->mpos = floorDiv( IntVec2{x_, y_}, static_cast<double>(PIXES_PER_MAPENT) );
        this->ppos = mpos_2_ppos( this->mpos );
    }
    

    inline void accum_mpos( const IntVec2 &mposOff_ ){
        this->mpos += mposOff_;
        this->ppos = mpos_2_ppos( this->mpos );
    }


    //--- get ---
    inline const IntVec2 &get_mpos() const {
        return this->mpos;
    }

    inline const IntVec2 &get_ppos() const{
        return this->ppos;
    }
    inline const glm::dvec2 get_midDPos() const { //- ppos of the mid_box
        IntVec2 ppos_l = mpos_2_ppos( this->mpos );
        return glm::dvec2{   static_cast<double>(ppos_l.x + HALF_PIXES_PER_MAPENT),
                            static_cast<double>(ppos_l.y + HALF_PIXES_PER_MAPENT) };
    }
    inline const glm::dvec2 get_dpos() const {
        IntVec2 ppos_l = mpos_2_ppos( this->mpos );
        return glm::dvec2{ static_cast<double>(ppos_l.x), 
                          static_cast<double>(ppos_l.y) };
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
inline bool operator == ( const MapCoord &a_, const MapCoord &b_ ){
    return ( a_.get_mpos() == b_.get_mpos() );
}
inline bool operator != ( const MapCoord &a_, const MapCoord &b_ ){
    return ( a_.get_mpos() != b_.get_mpos() );
}

/* ===========================================================
 *                 operator <
 * -----------------------------------------------------------
 * -- 通过这个 "<" 运算符重载，MapCoord 类型将支持 set.find() 
 */
inline bool operator < ( const MapCoord &a_, const MapCoord &b_ ){
    return ( a_.get_mpos() < b_.get_mpos() );
}

/* ===========================================================
 *                 operator +, -
 * -----------------------------------------------------------
 */
inline const MapCoord operator + ( const MapCoord &a_, const MapCoord &b_ ){
    return MapCoord { a_.get_mpos() + b_.get_mpos() };
}
inline const MapCoord operator - ( const MapCoord &a_, const MapCoord &b_ ){
    return MapCoord { a_.get_mpos() - b_.get_mpos() };
}


/* ===========================================================
 *                   ppos_2_mpos  [严格对齐]
 * -----------------------------------------------------------
 * -- 参数 ppos_ 必须对齐于 mapent坐标系
 */
inline const IntVec2 ppos_2_mpos( const IntVec2 &ppos_ ){
    tprAssert( (ppos_.x%PIXES_PER_MAPENT==0) && (ppos_.y%PIXES_PER_MAPENT==0) );
    return floorDiv( ppos_, static_cast<double>(PIXES_PER_MAPENT) );
}

inline const IntVec2 anyPPos_2_mpos( const IntVec2 &anyPPos_ ){
    return floorDiv( anyPPos_, static_cast<double>(PIXES_PER_MAPENT) );
}

/* ===========================================================
 *                   dpos_2_mcpos  [宽松]    IMPORTANT !!!
 * -----------------------------------------------------------
 * -- 参数 fpos_ 可以为任意值。 无需对齐于 mapent坐标系
 */
inline const MapCoord dpos_2_mcpos( const glm::dvec2 &dpos_ ){
    //-- double除法
    double fx = dpos_.x / static_cast<double>(PIXES_PER_MAPENT);
    double fy = dpos_.y / static_cast<double>(PIXES_PER_MAPENT);
    //-- math.floor() 
    return MapCoord{    static_cast<int>(floor(fx)), 
                        static_cast<int>(floor(fy)) };
}

inline const IntVec2 dpos_2_mpos( const glm::dvec2 &dpos_ ){
    //-- double除法
    double fx = dpos_.x / static_cast<double>(PIXES_PER_MAPENT);
    double fy = dpos_.y / static_cast<double>(PIXES_PER_MAPENT);
    //-- math.floor() 
    return IntVec2{ static_cast<int>(floor(fx)), 
                    static_cast<int>(floor(fy)) };
}


/* ===========================================================
 *                mpos_2_midPPos
 * -----------------------------------------------------------
 * -- 获得 mapent 中间pixel 的 ppos 
 */
inline const IntVec2 mpos_2_midPPos( const IntVec2 &mpos_ ){
    return IntVec2{ mpos_.x*PIXES_PER_MAPENT + MID_PPOS_IDX_IN_MAPENT,
                    mpos_.y*PIXES_PER_MAPENT + MID_PPOS_IDX_IN_MAPENT };
}


/* ===========================================================
 *             calc_fast_ppos_distance
 * -----------------------------------------------------------
 * 计算两个 ppos 的距离（快速版，不开根号）
 * 返回的结果，只是一个 “含糊的距离概念” [主要用来 生成 cell-noise]
 */
inline int calc_fast_ppos_distance( const IntVec2 &aPPos_, const IntVec2 &bPPos_ ){
    IntVec2 off = aPPos_ - bPPos_;
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
inline int calc_fast_mpos_distance( const IntVec2 &aMPos_, const IntVec2 &bMPos_ ){
    IntVec2 off = aMPos_ - bMPos_;
        //-- 没有做 溢出检测...
    return (off.x*off.x + off.y*off.y);
}


#endif 

