/*
 * ======================= BrokenLvl.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BROKEN_LEVEL_H
#define TPR_BROKEN_LEVEL_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"



// go 的 破损等级
// 会被记录在 蓝图 .D.png 数据中，代表每个 mp-go 的健全状态
// 比如，石头有几种破损状态
// ---
// 每种 go 自身的 HealthState 划分，是完全自定义的，可能不会用完全部的值
enum class BrokenLvl{
    Lvl_0, // 最完好，最健全的状态
    Lvl_1, // 往下依次变得破损
    Lvl_2,
    Lvl_3,
    Lvl_4
};


std::string brokenLvl_2_str( BrokenLvl bl_ )noexcept;
BrokenLvl str_2_brokenLvl( const std::string &str_ )noexcept;


inline BrokenLvl int_2_brokenLvl( int val_ )noexcept{
    switch (val_){
        case 0: return BrokenLvl::Lvl_0;
        case 1: return BrokenLvl::Lvl_1;
        case 2: return BrokenLvl::Lvl_2;
        case 3: return BrokenLvl::Lvl_3;
        case 4: return BrokenLvl::Lvl_4;
        default:
            tprAssert(0);
            return BrokenLvl::Lvl_0;
    }
}



#endif 

