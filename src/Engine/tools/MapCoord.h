/*
 * ====================== MapCoord.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
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



inline IntVec2 mpos_2_ppos( IntVec2 mpos_ ) noexcept {
    return (mpos_*PIXES_PER_MAPENT<>);
}


//-- 当 dpos 无限接近 mapent 边界时，这个返回值就会非常不精确...
inline IntVec2 dpos_2_mpos( const glm::dvec2 &dpos_ ) noexcept {
    //-- double除法
    double fx = dpos_.x / PIXES_PER_MAPENT_D;
    double fy = dpos_.y / PIXES_PER_MAPENT_D;
    //-- math.floor() 
    return IntVec2{ static_cast<int>(floor(fx)), 
                    static_cast<int>(floor(fy)) };
}


inline glm::dvec2 mpos_2_dpos( IntVec2 mpos_ ) noexcept {
    return glm::dvec2{  static_cast<double>(mpos_.x * PIXES_PER_MAPENT<>),
                        static_cast<double>(mpos_.y * PIXES_PER_MAPENT<>) };
}



inline glm::dvec2 mpos_2_midDPos( IntVec2 mpos_ ) noexcept {
    return glm::dvec2{  static_cast<double>(mpos_.x * PIXES_PER_MAPENT<> + HALF_PIXES_PER_MAPENT<>),
                        static_cast<double>(mpos_.y * PIXES_PER_MAPENT<> + HALF_PIXES_PER_MAPENT<>) };
}



/* ===========================================================
 *               calc_fast_mpos_distance
 * -----------------------------------------------------------
 * 计算两个 mpos 的距离（快速版，不开根号）
 * 和上一个函数并没有本质区别。
 * 返回的结果，只是一个 “含糊的距离概念” [主要用来 生成 cell-noise]
 */
/*
inline int calc_fast_mpos_distance( IntVec2 aMPos_, IntVec2 bMPos_ ) noexcept {
    IntVec2 off = aMPos_ - bMPos_;
        //-- 没有做 溢出检测...
    return (off.x*off.x + off.y*off.y);
}
*/


#endif 

