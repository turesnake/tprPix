/*
 * ========================= SpeedLevel.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    专门管理 GameObj实例 的 位移运动: 爬行
 * ----------------------------
 */
#ifndef _TPR_SPEED_LEVEL_H_
#define _TPR_SPEED_LEVEL_H_

//------------------- C --------------------//
#include <cassert>

//------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-- go 可设置的 8档 速度 --
enum class SpeedLevel : u8_t {
    LV_0 = 0, //- 无速度
    LV_1,     //- 低速
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
    LV_12     //- crawl 的最高速度，不能超过 1帧1mapent
};


//-- 60HZ 时，一帧的 fpos 速度
inline const std::vector<float> speedTable{
    0.0,   //- LV_0 //- 无速度
    0.3,   //- LV_1
    0.8,   //- LV_2
    1.2,   //- LV_3
    1.6,   //- LV_4 //- 普通的慢速
    2.0,   //- LV_5
    2.5,   //- LV_6 //- 最适合的速度
    3.0,   //- LV_7
    3.5,   //- LV_8
    4.0,   //- LV_9  //- 偏快的速度
    5.0,   //- LV_10
    6.0,   //- LV_11
    7.5    //- LV_12 : crawl 的最高速度，不能超过 1帧1mapent
};


inline const float SpeedLevel_2_val( const SpeedLevel &_lvl ){
    return speedTable.at( static_cast<size_t>(_lvl) );
}


inline const SpeedLevel calc_higher_speedLvl( const SpeedLevel &_lvl ){
    switch (_lvl){
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
        case SpeedLevel::LV_12: return SpeedLevel::LV_12;
        default:
            assert(0);
    }
}
inline const SpeedLevel calc_lower_speedLvl( const SpeedLevel &_lvl ){
    switch (_lvl){
        case SpeedLevel::LV_0:  return SpeedLevel::LV_0;
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
        default:
            assert(0);
    }
}


#endif 

