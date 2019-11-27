/*
 * ========================== Quad.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  quadrant
 * ----------------------------
 */
#ifndef TPR_QUAD_H
#define TPR_QUAD_H

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


inline int QuadType_2_Idx( QuadType type_ )noexcept{
    return (int)type_;
}

inline QuadType QuadIdx_2_Type( int idx_ )noexcept{
    switch(idx_){
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
    explicit QuadFlag( bool val_=false ):
        is_left_bottom(val_),
        is_right_bottom(val_),
        is_left_top(val_),
        is_right_top(val_)
        {}

    inline bool is_all_true() const noexcept{
        return( is_left_bottom && is_right_bottom && is_left_top && is_right_top );
    }
    inline bool is_all_false() const noexcept{
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

