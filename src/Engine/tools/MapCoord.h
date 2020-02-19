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

//-------------------- CPP --------------------//
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
inline IntVec2 mpos_2_ppos( IntVec2 mpos_ ) noexcept {
    return (mpos_*PIXES_PER_MAPENT);
}

//-- [left-bottom] --
// 管理 mapent 的坐标数据，可以很快转换为各种 类型
// 这个 class 的使用正在被弱化
// 也许会在未来被取消，彻底打散为一些 独立的 函数
class MapCoord{
public:
    //---- constructor -----//
    MapCoord() = default;
    //-- 只支持 mpos初始化，若想用 ppos来初始化，先用 anyPPos_2_mpos() 转换
    explicit MapCoord( IntVec2 mpos_ ):
        mpos(mpos_),
        ppos( mpos * PIXES_PER_MAPENT )
        {}
    MapCoord( int mpos_x_, int mpos_y_ ):
        mpos( IntVec2{ mpos_x_, mpos_y_ } ),
        ppos( mpos * PIXES_PER_MAPENT )
        {}
    
    //---- clear -----//
    inline void clear_all() noexcept {
        this->mpos.clear_all();
        this->ppos.clear_all();
    }

    //----- set ------//
    inline void set_by_mpos( IntVec2 mpos_ ) noexcept {
        this->mpos = mpos_;
        this->ppos = mpos_2_ppos( this->mpos );
    }

    inline void set_by_mpos( int x_, int y_ ) noexcept {
        this->mpos.set( x_, y_ );
        this->ppos = mpos_2_ppos( this->mpos );
    }

    //-- 新版 crawl 方案实施后，对齐于 mpos 的 go 几乎不存在了 
    //   需要一组更加 完备的函数
    //   -------
    //   还是需要使用 这个 严谨版
    inline void set_by_ppos_( IntVec2 ppos_ ) noexcept {
            tprAssert( (ppos_.x%PIXES_PER_MAPENT==0) && (ppos_.y%PIXES_PER_MAPENT==0) );
        this->ppos = ppos_;
        this->mpos = floorDiv( this->ppos, static_cast<double>(PIXES_PER_MAPENT) );
    }
    inline void set_by_ppos_( int x_, int y_ ) noexcept {
            tprAssert( (x_%PIXES_PER_MAPENT==0) && (y_%PIXES_PER_MAPENT==0) );
        this->ppos.set( x_, y_ );
        this->mpos = floorDiv( this->ppos, static_cast<double>(PIXES_PER_MAPENT) );
    }

    //-- 宽松版，目前仅被 gameObj_mem.cpp -> signUp_newGO_to_chunk_and_mapEnt() 使用 --
    inline void set_by_anyPPos( IntVec2 anyPPos_ ) noexcept {
        this->mpos = anyPPos_.floorDiv( static_cast<double>(PIXES_PER_MAPENT) );
        this->ppos = mpos_2_ppos( this->mpos );
    }
    inline void set_by_anyPPos( int x_, int y_ ) noexcept {
        this->mpos = floorDiv( IntVec2{x_, y_}, static_cast<double>(PIXES_PER_MAPENT) );
        this->ppos = mpos_2_ppos( this->mpos );
    }
    

    inline void accum_mpos( IntVec2 mposOff_ ) noexcept {
        this->mpos += mposOff_;
        this->ppos = mpos_2_ppos( this->mpos );
    }


    //--- get ---
    inline IntVec2 get_mpos() const noexcept {
        return this->mpos;
    }

    inline IntVec2 get_ppos() const noexcept {
        return this->ppos;
    }
    inline glm::dvec2 get_midDPos() const noexcept { //- ppos of the mid_box
        return glm::dvec2{  static_cast<double>(this->ppos.x + HALF_PIXES_PER_MAPENT),
                            static_cast<double>(this->ppos.y + HALF_PIXES_PER_MAPENT) };
    }
    inline glm::dvec2 get_dpos() const noexcept {
        return glm::dvec2{ static_cast<double>(this->ppos.x), 
                          static_cast<double>(this->ppos.y) };
    }

    //-- 仅用于 crawl --
    //   mpos 值是否在 [-1,1] 之间
    inline bool is_match_with_nineBox() const noexcept {
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
inline bool operator == ( const MapCoord &a_, const MapCoord &b_ ) noexcept {
    return ( a_.get_mpos() == b_.get_mpos() );
}
inline bool operator != ( const MapCoord &a_, const MapCoord &b_ ) noexcept {
    return ( a_.get_mpos() != b_.get_mpos() );
}

/* ===========================================================
 *                 operator <
 * -----------------------------------------------------------
 * -- 通过这个 "<" 运算符重载，MapCoord 类型将支持 set.find() 
 */
inline bool operator < ( const MapCoord &a_, const MapCoord &b_ ) noexcept {
    return ( a_.get_mpos() < b_.get_mpos() );
}

/* ===========================================================
 *                 operator +, -
 * -----------------------------------------------------------
 */
inline MapCoord operator + ( const MapCoord &a_, const MapCoord &b_ ) noexcept {
    return MapCoord { a_.get_mpos() + b_.get_mpos() };
}
inline MapCoord operator - ( const MapCoord &a_, const MapCoord &b_ ) noexcept {
    return MapCoord { a_.get_mpos() - b_.get_mpos() };
}


/* ===========================================================
 *                   ppos_2_mpos  [严格对齐]
 * -----------------------------------------------------------
 * -- 参数 ppos_ 必须对齐于 mapent坐标系
 */
inline IntVec2 ppos_2_mpos( IntVec2 ppos_ ) noexcept {
    tprAssert( (ppos_.x%PIXES_PER_MAPENT==0) && (ppos_.y%PIXES_PER_MAPENT==0) );
    return floorDiv( ppos_, static_cast<double>(PIXES_PER_MAPENT) );
}

inline IntVec2 anyPPos_2_mpos( IntVec2 anyPPos_ ) noexcept {
    return floorDiv( anyPPos_, static_cast<double>(PIXES_PER_MAPENT) );
}


inline MapCoord dpos_2_mcpos( const glm::dvec2 &dpos_ ) noexcept {
    //-- double除法
    double fx = dpos_.x / static_cast<double>(PIXES_PER_MAPENT);
    double fy = dpos_.y / static_cast<double>(PIXES_PER_MAPENT);
    //-- math.floor() 
    return MapCoord{    static_cast<int>(floor(fx)), 
                        static_cast<int>(floor(fy)) };
}

//-- 当 dpos 无限接近 mapent 边界时，这个返回值就会非常不精确...
inline IntVec2 dpos_2_mpos( const glm::dvec2 &dpos_ ) noexcept {
    //-- double除法
    double fx = dpos_.x / static_cast<double>(PIXES_PER_MAPENT);
    double fy = dpos_.y / static_cast<double>(PIXES_PER_MAPENT);
    //-- math.floor() 
    return IntVec2{ static_cast<int>(floor(fx)), 
                    static_cast<int>(floor(fy)) };
}

inline IntVec2 dpos_2_ppos( const glm::dvec2 &dpos_ ) noexcept {
    return IntVec2{ static_cast<int>(floor( dpos_.x )), 
                    static_cast<int>(floor( dpos_.y )) };
}


inline glm::dvec2 calc_dpos_from_mpos_and_pposOff( const IntVec2 mpos_, const IntVec2 pposOff_ ) noexcept {
    IntVec2 ppos = mpos_2_ppos(mpos_) + pposOff_;
    return glm::dvec2{  static_cast<double>(ppos.x),
                        static_cast<double>(ppos.y) };
}

inline glm::dvec2 mpos_2_dpos( IntVec2 mpos_ ) noexcept {
    return glm::dvec2{  static_cast<double>(mpos_.x * PIXES_PER_MAPENT),
                        static_cast<double>(mpos_.y * PIXES_PER_MAPENT) };
}


/* ===========================================================
 *                mpos_2_midPPos
 * -----------------------------------------------------------
 * -- 获得 mapent 中间pixel 的 ppos 
 */
inline IntVec2 mpos_2_midPPos( IntVec2 mpos_ ) noexcept {
    return IntVec2{ mpos_.x*PIXES_PER_MAPENT + HALF_PIXES_PER_MAPENT,
                    mpos_.y*PIXES_PER_MAPENT + HALF_PIXES_PER_MAPENT };
}


inline glm::dvec2 mpos_2_midDPos( IntVec2 mpos_ ) noexcept {
    return glm::dvec2{  static_cast<double>(mpos_.x * PIXES_PER_MAPENT + HALF_PIXES_PER_MAPENT),
                        static_cast<double>(mpos_.y * PIXES_PER_MAPENT + HALF_PIXES_PER_MAPENT) };
}



/* ===========================================================
 *             calc_fast_ppos_distance
 * -----------------------------------------------------------
 * 计算两个 ppos 的距离（快速版，不开根号）
 * 返回的结果，只是一个 “含糊的距离概念” [主要用来 生成 cell-noise]
 */
inline int calc_fast_ppos_distance( IntVec2 aPPos_, IntVec2 bPPos_ ) noexcept {
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
inline int calc_fast_mpos_distance( IntVec2 aMPos_, IntVec2 bMPos_ ) noexcept {
    IntVec2 off = aMPos_ - bMPos_;
        //-- 没有做 溢出检测...
    return (off.x*off.x + off.y*off.y);
}


#endif 

