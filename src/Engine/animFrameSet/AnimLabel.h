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


/*
inline AnimLabel str_2_AnimLabel( const std::string &str_ )noexcept{
    if(       (str_ == "Default") || (str_ == "") ){  return AnimLabel::Default;
    
    }else if( str_ == "Sml" ){      return AnimLabel::Sml;
    }else if( str_ == "Mid" ){      return AnimLabel::Mid;
    }else if( str_ == "Big" ){      return AnimLabel::Big;
    }else if( str_ == "Huge" ){      return AnimLabel::Huge;

    }else if( str_ == "Light" ){    return AnimLabel::Light;
    }else if( str_ == "Dark" ){     return AnimLabel::Dark;

    }else if( str_ == "Fat" ){      return AnimLabel::Fat;
    }else if( str_ == "Thin" ){     return AnimLabel::Thin;
    }else if( str_ == "HalfDead" ){ return AnimLabel::HalfDead;
    }

    else if(  str_ == "Field_1f1" ){ return AnimLabel::Field_1f1;
    }else if( str_ == "Field_2f2" ){ return AnimLabel::Field_2f2;
    }else if( str_ == "Field_3f3" ){ return AnimLabel::Field_3f3;
    }

    else if(  str_ == "MapEnt_1m1" ){ return AnimLabel::MapEnt_1m1;
    }else if( str_ == "MapEnt_2m2" ){ return AnimLabel::MapEnt_2m2;
    }else if( str_ == "MapEnt_3m3" ){ return AnimLabel::MapEnt_3m3;
    }else if( str_ == "MapEnt_4m4" ){ return AnimLabel::MapEnt_4m4;
    }

    else if( str_ == "Light_Bald" ){ return AnimLabel::Light_Bald;
    }else if( str_ == "Light_Lichen" ){ return AnimLabel::Light_Lichen;
    }

    else if( str_ == "Dead" ){ return AnimLabel::Dead;
    }

    else if( str_ == "T1" ){ return AnimLabel::T1;
    }else if( str_ == "T2" ){ return AnimLabel::T2;
    }

    else{
        tprAssert(0);
        return AnimLabel::Default; // never reach
    }
}
*/



#endif 

