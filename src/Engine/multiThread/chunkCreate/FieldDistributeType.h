/*
 * =================== FieldDistributeType.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.10.07
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_FIELD_DISTRIBUTE_TYPE_H
#define TPR_FIELD_DISTRIBUTE_TYPE_H

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//-------------------- Engine --------------------//
#include "BodySize.h"
#include "tprAssert.h"


enum class FieldDistributeType{
    Sml1,
    Sml2,
    Sml3, // frequent

    Mid1,
    Mid2, // frequent

    Big1,

    Sml1_Mid1, // frequent
    Sml2_Mid1, // frequent
};


inline FieldDistributeType str_2_fieldDistributeType( const std::string &str_ )noexcept{
    if(         str_ == "Sml1" ){       return FieldDistributeType::Sml1;
    }else if(   str_ == "Sml2" ){       return FieldDistributeType::Sml2;
    }else if(   str_ == "Sml3" ){       return FieldDistributeType::Sml3;
    }
    else if(    str_ == "Mid1" ){       return FieldDistributeType::Mid1;
    }else if(   str_ == "Mid2" ){       return FieldDistributeType::Mid2;
    }
    else if(    str_ == "Big1" ){       return FieldDistributeType::Big1;
    }
    else if(    str_ == "Sml1_Mid1" ){  return FieldDistributeType::Sml1_Mid1;
    }else if(   str_ == "Sml2_Mid1" ){  return FieldDistributeType::Sml2_Mid1;
    }else{
        tprAssert(0);
        return FieldDistributeType::Sml1; // never reach
    }
}




inline std::vector<BodySize> bodySizes_in_fieldDistributeType( FieldDistributeType type_ ){

    switch (type_){
        case FieldDistributeType::Sml1:
        case FieldDistributeType::Sml2:
        case FieldDistributeType::Sml3:
            return std::vector<BodySize> { BodySize::Sml };

        case FieldDistributeType::Mid1:
        case FieldDistributeType::Mid2:
            return std::vector<BodySize> { BodySize::Mid };

        case FieldDistributeType::Big1:
            return std::vector<BodySize> { BodySize::Big };

        case FieldDistributeType::Sml1_Mid1:
        case FieldDistributeType::Sml2_Mid1:
            return std::vector<BodySize> { BodySize::Sml, BodySize::Mid };
        //...
        default:
            tprAssert(0);
            return std::vector<BodySize> {}; // never reach
    }
}


#endif 

