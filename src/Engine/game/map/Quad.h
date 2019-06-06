/*
 * ========================== Quad.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  quadrant
 * ----------------------------
 */
#ifndef TPR_QUAD_H_
#define TPR_QUAD_H_

//-------------------- C --------------------//
//#include <cassert>

//-------------------- Engine --------------------//
#include "tprAssert.h"

//-- 一共有 4个 扇区 --
#define QUAD_NUM 4


//-- 4个象限类型，[left-bottom] --
//  排列顺序是为了 方便地遍历 vector
enum class QuadType : int {
    Left_Bottom  = 0,
    Right_Bottom = 1,
    Left_Top     = 2,
    Right_Top    = 3
};


inline int QuadType_2_Idx( QuadType _type ){
    return (int)_type;
}

inline QuadType QuadIdx_2_Type( int _idx ){
    switch(_idx){
        case 0: return QuadType::Left_Bottom;
        case 1: return QuadType::Right_Bottom;
        case 2: return QuadType::Left_Top;
        case 3: return QuadType::Right_Top;
        default:
            tprAssert(0);
            return QuadType::Left_Bottom; //- never touch
    }
}


//--- 统一管理 4个象限的 bool值 ---
//   暂时未被使用
class QuadFlag{
public:
    explicit QuadFlag( bool _val=false ):
        is_left_bottom(_val),
        is_right_bottom(_val),
        is_left_top(_val),
        is_right_top(_val)
        {}

    inline bool is_all_true() const {
        return( is_left_bottom && is_right_bottom && is_left_top && is_right_top );
    }
    inline bool is_all_false() const {
        return ( (is_left_bottom==false) &&  
                 (is_right_bottom==false) &&
                 (is_left_top==false) &&
                 (is_right_top==false) );
    }

    //======== flags ========//
    bool is_left_bottom  {false};
    bool is_right_bottom {false};
    bool is_left_top     {false};
    bool is_right_top    {false};
};



#endif 

