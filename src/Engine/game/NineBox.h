/*
 * ========================= NineBox.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.26
 *                                        修改 -- 2018.12.26
 * ----------------------------------------------------------
 *      九宫格 
 *  可以表达 crawl 的 8方向， 
 *  以及 像素点 在某个 mapent 中的位置
 * ----------------------------
 */
#ifndef _TPR_NINE_BOX_H_
#define _TPR_NINE_BOX_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "config.h"
#include "PixVec.h"


//-- 九宫格中的 9个位置：
//  -1 -- 左／下
//   0 -- 无／中心
//   1 -- 右／上
//--- 但是为了增强 结构的鲁棒性，
// 一切小于0的 当作 -1
// 一切大于0的 当作 1
struct NineBox{
    int x;
    int y;
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator == ( const NineBox &_a, const NineBox &_b ){
    return ( (_a.x==_b.x) && (_a.y==_b.y) );
}
inline bool operator != ( const NineBox &_a, const NineBox &_b ){
    return ( (_a.x!=_b.x) || (_a.y!=_b.y) );
}


//-- 仅有的 (1+9)个 变量值 -- 
inline const NineBox  nineBoxNull    {0,0};
//---
inline const NineBox  nb_left_bottom  { -1, -1 };
inline const NineBox  nb_mid_bottom   {  0, -1 };
inline const NineBox  nb_right_bottom {  1, -1 };

inline const NineBox  nb_left_mid  { -1,  0 };
inline const NineBox  nb_mid_mid   {  0,  0 };
inline const NineBox  nb_right_mid {  1,  0 };

inline const NineBox  nb_left_top  { -1,  1 };
inline const NineBox  nb_mid_top   {  0,  1 };
inline const NineBox  nb_right_top {  1,  1 };


//-- 九宫格 另一种表达法。
enum class NineBoxIdx : int {
    //--
    Left_Bottom  = 0,
    Mid_Bottom   = 1,
    Right_Bottom = 2,
    //--
    Left_Mid  = 3,
    Mid_Mid   = 4,
    Right_Mid = 5,
    //--
    Left_Top  = 6,
    Mid_Top   = 7,
    Right_Top = 8
};
inline const int NineBoxIdxSize {9}; //- 9个子单位


/* ===========================================================
 *                   NineBox_XY_2_Idx
 * -----------------------------------------------------------
 */
inline NineBoxIdx NineBox_XY_2_Idx( const NineBox &_nb ){

    if( _nb.y < 0 ){
        if( _nb.x<0 ){          return NineBoxIdx::Left_Bottom;
        }else if( _nb.x==0 ){   return NineBoxIdx::Mid_Bottom;
        }else{                  return NineBoxIdx::Right_Bottom;
        }

    }else if( _nb.y == 0 ){
        if( _nb.x<0 ){         return NineBoxIdx::Left_Mid;
        }else if( _nb.x==0 ){  return NineBoxIdx::Mid_Mid;
        }else{                 return NineBoxIdx::Right_Mid;
        }

    }else{
        if( _nb.x<0 ){         return NineBoxIdx::Left_Top;
        }else if( _nb.x==0 ){  return NineBoxIdx::Mid_Top;
        }else{                 return NineBoxIdx::Right_Top;
        }
    }
}


/* ===========================================================
 *                 NineBox_Idx_2_XY
 * -----------------------------------------------------------
 */
inline const NineBox& NineBox_Idx_2_XY( NineBoxIdx _idx ){

    switch( _idx ){
        case NineBoxIdx::Left_Bottom:   return nb_left_bottom;
        case NineBoxIdx::Mid_Bottom:    return nb_mid_bottom;
        case NineBoxIdx::Right_Bottom:  return nb_right_bottom;

        case NineBoxIdx::Left_Mid:   return nb_left_mid;
        case NineBoxIdx::Mid_Mid:    return nb_mid_mid;
        case NineBoxIdx::Right_Mid:  return nb_right_mid;

        case NineBoxIdx::Left_Top:   return nb_left_top;
        case NineBoxIdx::Mid_Top:    return nb_mid_top;
        case NineBoxIdx::Right_Top:  return nb_right_top;
        default:
            assert(0);
            return nb_left_bottom; //- nener touch
    }
}


/* ===========================================================
 *                 calc_ppos_compass
 * -----------------------------------------------------------
 * -- 传入一个 ppos，获得这个 ppos，在其mapent 中的位置
 */
inline NineBox calc_ppos_compass( const PixVec2 _ppos ){
    return NineBox { (_ppos.x%PIXES_PER_MAPENT)-1, 
                     (_ppos.y%PIXES_PER_MAPENT)-1 };
}


#endif 

