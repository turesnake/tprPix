/*
 * ====================== GoFunctorLabel.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_FUNTOR_LABEL_H
#define TPR_GO_FUNTOR_LABEL_H

//------------------- CPP --------------------//
#include <string>

//------------------- Libs --------------------//
#include "tprDataType.h"


// GoFunctorSet :key
enum class GoFunctorLabel : u32_t {
    Nil=0, // 空值

    Tmp, 
    

    //...
};

GoFunctorLabel str_2_GoFunctorLabel( const std::string &str_ )noexcept;
std::string goFunctorLabel_2_str( GoFunctorLabel l_ )noexcept;



#endif 

