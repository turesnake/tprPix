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

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- CPP --------------------//
#include <vector>

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"


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
    LV_17,
    LV_18,
    LV_19  //- highest speed for crawl mode，can't faster than 1mapEnt/1frame
};

inline size_t speedLevel_2_size_t( SpeedLevel lvl_ )noexcept{
    return static_cast<size_t>(lvl_);
}


//-- 60HZ 时，一帧的 fpos 速度
extern const std::vector<double> speedTable;


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
        case 18: return SpeedLevel::LV_18;
        case 19: return SpeedLevel::LV_19;
        default:
            tprAssert(0);
            return SpeedLevel::LV_0; //- never reach
    }
}


inline double SpeedLevel_2_val( SpeedLevel lvl_ )noexcept{
    return speedTable.at( speedLevel_2_size_t(lvl_) );
}


inline SpeedLevel calc_higher_speedLvl( SpeedLevel lvl_ )noexcept{
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
        case SpeedLevel::LV_17: return SpeedLevel::LV_18;
        case SpeedLevel::LV_18: return SpeedLevel::LV_19;
        case SpeedLevel::LV_19: return SpeedLevel::LV_19; // no change
        default:
            tprAssert(0);
            return SpeedLevel::LV_0; //- never reach
    }
}
inline SpeedLevel calc_lower_speedLvl( SpeedLevel lvl_ )noexcept{
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
        case SpeedLevel::LV_18: return SpeedLevel::LV_17;
        case SpeedLevel::LV_19: return SpeedLevel::LV_18;
        default:
            tprAssert(0);
            return SpeedLevel::LV_0; //- never reach
    }
}


inline glm::dvec2 limit_moveSpeed( const glm::dvec2 &speedV_ )noexcept{

    // Avoid Radical Sign / 避免开根号 --
    double moveLen =  speedV_.x*speedV_.x + speedV_.y*speedV_.y;
    if( moveLen < PIXES_PER_MAPENT_D * PIXES_PER_MAPENT_D ){
        return speedV_;
    }
    //-- max legal speed vec --
    return glm::normalize(speedV_) * SpeedLevel_2_val(SpeedLevel::LV_19);
}


#endif 

