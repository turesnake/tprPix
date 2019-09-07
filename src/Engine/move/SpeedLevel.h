/*
 * ========================= SpeedLevel.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    only for GameObj::move
 * ----------------------------
 */
#ifndef TPR_SPEED_LEVEL_H
#define TPR_SPEED_LEVEL_H

//------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "tprAssert.h"


//--  18 level --
enum class SpeedLevel : int {
    LV_0 = 0, //- no speed
    LV_1,     //- low
    LV_2, 
    LV_3, 
    LV_4, 
    LV_5, 
    LV_6, 
    LV_7, 
    LV_8,  
    LV_9, 
    LV_10, 
    LV_11, 
    LV_12, 
    LV_13,
    LV_14,
    LV_15,
    LV_16,
    LV_17     //- highest speed for crawl mode，can't faster than 1mapEnt/1frame
};

inline size_t speedLevel_2_size_t( const SpeedLevel &lvl_ )noexcept{
    return static_cast<size_t>(lvl_);
}


//-- 60HZ 时，一帧的 fpos 速度
inline const std::vector<double> speedTable{
    0.0,       //- LV_0 //- 无速度
    0.3*8.0,   //- LV_1
    0.6*8.0,   //- LV_2
    0.9*8.0,   //- LV_3
    1.2*8.0,   //- LV_4
    1.5*8.0,   //- LV_5
    1.8*8.0,   //- LV_6
    2.1*8.0,   //- LV_7
    2.4*8.0,   //- LV_8
    2.7*8.0,   //- LV_9
    3.0*8.0,   //- LV_10
    3.3*8.0,   //- LV_11
    3.6*8.0,   //- LV_12
    4.0*8.0,   //- LV_13
    4.5*8.0,   //- LV_14
    5.0*8.0,   //- LV_15
    6.0*8.0,   //- LV_16
    7.0*8.0    //- LV_17 : crawl 的最高速度，不能超过 1帧1mapent
               //  值7.5 会在 ubuntu 中出问题。暂改为 7.0 
};

inline SpeedLevel int_2_SpeedLevel( int num_ )noexcept{
    switch(num_){
        case 0: return SpeedLevel::LV_0;
        case 1: return SpeedLevel::LV_1;
        case 2: return SpeedLevel::LV_2;
        case 3: return SpeedLevel::LV_3;
        case 4: return SpeedLevel::LV_4;
        case 5: return SpeedLevel::LV_5;
        case 6: return SpeedLevel::LV_6;
        case 7: return SpeedLevel::LV_7;
        case 8: return SpeedLevel::LV_8;
        case 9: return SpeedLevel::LV_9;
        case 10: return SpeedLevel::LV_10;
        case 11: return SpeedLevel::LV_11;
        case 12: return SpeedLevel::LV_12;
        case 13: return SpeedLevel::LV_13;
        case 14: return SpeedLevel::LV_14;
        case 15: return SpeedLevel::LV_15;
        case 16: return SpeedLevel::LV_16;
        case 17: return SpeedLevel::LV_17;
        default:
            tprAssert(0);
            return SpeedLevel::LV_0; //- never reach
    }
}


inline double SpeedLevel_2_val( const SpeedLevel &lvl_ )noexcept{
    return speedTable.at( speedLevel_2_size_t(lvl_) );
}


inline SpeedLevel calc_higher_speedLvl( const SpeedLevel &lvl_ )noexcept{
    switch (lvl_){
        case SpeedLevel::LV_0:  return SpeedLevel::LV_1;
        case SpeedLevel::LV_1:  return SpeedLevel::LV_2;
        case SpeedLevel::LV_2:  return SpeedLevel::LV_3;
        case SpeedLevel::LV_3:  return SpeedLevel::LV_4;
        case SpeedLevel::LV_4:  return SpeedLevel::LV_5;
        case SpeedLevel::LV_5:  return SpeedLevel::LV_6;
        case SpeedLevel::LV_6:  return SpeedLevel::LV_7;
        case SpeedLevel::LV_7:  return SpeedLevel::LV_8;
        case SpeedLevel::LV_8:  return SpeedLevel::LV_9;
        case SpeedLevel::LV_9:  return SpeedLevel::LV_10;
        case SpeedLevel::LV_10: return SpeedLevel::LV_11;
        case SpeedLevel::LV_11: return SpeedLevel::LV_12;
        case SpeedLevel::LV_12: return SpeedLevel::LV_13;
        case SpeedLevel::LV_13: return SpeedLevel::LV_14;
        case SpeedLevel::LV_14: return SpeedLevel::LV_15;
        case SpeedLevel::LV_15: return SpeedLevel::LV_16;
        case SpeedLevel::LV_16: return SpeedLevel::LV_17;
        case SpeedLevel::LV_17: return SpeedLevel::LV_17; // no change
        default:
            tprAssert(0);
            return SpeedLevel::LV_0; //- never reach
    }
}
inline SpeedLevel calc_lower_speedLvl( const SpeedLevel &lvl_ )noexcept{
    switch (lvl_){
        case SpeedLevel::LV_0:  return SpeedLevel::LV_0; // no change
        case SpeedLevel::LV_1:  return SpeedLevel::LV_0;
        case SpeedLevel::LV_2:  return SpeedLevel::LV_1;
        case SpeedLevel::LV_3:  return SpeedLevel::LV_2;
        case SpeedLevel::LV_4:  return SpeedLevel::LV_3;
        case SpeedLevel::LV_5:  return SpeedLevel::LV_4;
        case SpeedLevel::LV_6:  return SpeedLevel::LV_5;
        case SpeedLevel::LV_7:  return SpeedLevel::LV_6;
        case SpeedLevel::LV_8:  return SpeedLevel::LV_7;
        case SpeedLevel::LV_9:  return SpeedLevel::LV_8;
        case SpeedLevel::LV_10: return SpeedLevel::LV_9;
        case SpeedLevel::LV_11: return SpeedLevel::LV_10;
        case SpeedLevel::LV_12: return SpeedLevel::LV_11;
        case SpeedLevel::LV_13: return SpeedLevel::LV_12;
        case SpeedLevel::LV_14: return SpeedLevel::LV_13;
        case SpeedLevel::LV_15: return SpeedLevel::LV_14;
        case SpeedLevel::LV_16: return SpeedLevel::LV_15;
        case SpeedLevel::LV_17: return SpeedLevel::LV_16;
        default:
            tprAssert(0);
            return SpeedLevel::LV_0; //- never reach
    }
}


#endif 

