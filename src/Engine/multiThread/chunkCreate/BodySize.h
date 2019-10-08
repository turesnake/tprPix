/*
 * ========================= BodySize.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   body size of gameObj, affect the go's distribution in field
 * ----------------------------
 */
#ifndef TPR_BODY_SIZE_H
#define TPR_BODY_SIZE_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"


enum class BodySize{
    NotCare,// same as NULL, target go can't be used in field distribution

    Sml,    // 1 field 3 go: flower
            // radius: [0, 1] mapent

    Mid,    // 1 field 2 go: pineTree, human
            // radius: [1, 1.5] mapent

    Big,    // 1 field 1 go: hugTree
            // redius  [1.5, 2] mapent
};


inline BodySize str_2_bodySize( const std::string &str_ )noexcept{
    if(         str_ == "NotCare" ){    return BodySize::NotCare;
    }else if(   str_ == "Sml" ){        return BodySize::Sml;
    }else if(   str_ == "Mid" ){        return BodySize::Mid;
    }else if(   str_ == "Big" ){        return BodySize::Big;
    }else{
        tprAssert(0);
        return BodySize::NotCare; // never reach
    }
}






#endif 

