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
#include <cstdint> // uint8_t


// GoFunctorSet :key
enum class GoFunctorLabel : uint32_t {
    Nil=0, // 空值

    Tmp, 
    

    //...
};

GoFunctorLabel str_2_goFunctorLabel( const std::string &str_ )noexcept;
std::string goFunctorLabel_2_str( GoFunctorLabel l_ )noexcept;



#endif 

