/*
 * ========================= NineBox.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *      九宫格
 *  仅适用于 表达 8方向，
 *  是 crawl.collision 唯一接受的 方向参数
 *  但不再是 crawl 的 方向参数
 *  新版本的 crawl，支持 17个 运动方向 ( CrawlCompass ) 
 * ----------------------------
 */
#ifndef _TPR_NINE_BOX_H_
#define _TPR_NINE_BOX_H_

//--- glm - 0.9.9.5 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"


//-- 九宫格中的 9个位置：
//  -1 -- 左／下
//   0 -- 无／中心
//   1 -- 右／上
//--- 但是为了增强 结构的鲁棒性，
// 一切小于0的 当作 -1
// 一切大于0的 当作 1
class NineBox{
public:
    NineBox() = default;
    NineBox( int _x, int _y ):
        x(_x),
        y(_y)
        { assert( (x>=-1) && (x<=1) && (y>=-1) && (y<=1) ); }

    inline void clear_all(){
        x = 0;
        y = 0;
    }

    inline void set( int _x, int _y ){
        assert( (_x>=-1) && (_x<=1) && (_y>=-1) && (_y<=1) );
        x = _x;
        y = _y;
    }
    
    //-- 专用于 crawl模式，将方向转换为 位移值 --
    inline IntVec2 to_mpos() const {
        return IntVec2{ x, y };
    }
    inline IntVec2 to_ppos() const {
        return IntVec2{ x*PIXES_PER_MAPENT, 
                        y*PIXES_PER_MAPENT };
    } 
    inline glm::vec2 to_fpos() const {
        return glm::vec2{ (float)(x*PIXES_PER_MAPENT), 
                          (float)(y*PIXES_PER_MAPENT) };
    } 

    inline bool is_zero() const {
        if( (x==0) && (y==0) ){
            return true;
        }
        return false;
    }

    //======== vals ========//
    int x {0};  //- [-1, 1]
    int y {0};  //- [-1, 1]
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


//-- 仅有的 9个 变量值 -- 
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


#endif 

