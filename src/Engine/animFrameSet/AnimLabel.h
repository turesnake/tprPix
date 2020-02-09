/*
 * ========================== AnimLabel.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ANIM_LABEL_H
#define TPR_ANIM_LABEL_H

//------------------- CPP --------------------//
#include <string>

//------------------- Libs --------------------//
#include "tprDataType.h"


//-- 亚种 标签 --
// 一个亚种，最多允许携带 0～1 个标签
// ---
// 代替 std::string, 提高运行时效率
// 缺点: 随着游戏内容的膨胀，会有越来越多的 元素加入进来。
// 每一次都会影响本 enum h文件
// 从而需要重编译
enum class AnimLabel : u32_t{
    Default=0, //- 未设置 label 的，自动使用此标签来搜索
    Nil, // 空值
    //---
    Sml,
    Mid,
    Big,
    Huge,
    //---
    Light,
    Grey, // 暂时没有更好的，表达中间色的 词...
    Dark,
    //---
    Fat,
    Thin,
    HalfDead, //- tmp

    //    将被废弃 .....
    Field_1f1,
    Field_2f2,
    Field_3f3,

    //--- used for floor RandEnt
    MapEnt_1m1,
    MapEnt_2m2,
    MapEnt_3m3,
    MapEnt_4m4,

    //-- rocks
    Big_Light_Bald,    // tmp
    Big_Light_Lichen,  // tmp

    //---
    Dead,

    T1,
    T2,

    Front,
    Back,

    Normal,
    Special,

    //---
    // floorGo:road: 
    TwoPorts,   // 2个端口
    ThreePorts, // 3个端口


    //...
};


AnimLabel str_2_AnimLabel( const std::string &str_ )noexcept;


#endif 

