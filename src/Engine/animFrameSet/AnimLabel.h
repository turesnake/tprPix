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
enum class AnimLabel : u32_t{
    Default=0, //- 未设置 label 的，自动使用此标签来搜索
    //---
    Sml,
    Mid,
    Big,
    Huge,
    //---
    Light,
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

    //...
};


AnimLabel str_2_AnimLabel( const std::string &str_ )noexcept;


#endif 

