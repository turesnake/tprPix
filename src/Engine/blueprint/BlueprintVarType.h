/*
 * ======================= BlueprintVarType.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BLUE_PRINT_VARIABLE_H
#define TPR_BLUE_PRINT_VARIABLE_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "RGBA.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//

//-- 蓝图专用 变量
// 每种变量，在png数据中，以一种固定的颜色来表达
//
enum class VariableTypeIdx{
    V_1,
    V_2,
    V_3,
    V_4,
    V_5,
    V_6,
    V_7,
    V_8,
    V_9,
    V_10,
    V_11,
    V_12,
    V_13,
    V_14,
    V_15,
    V_16,
    V_17,
    V_18,
    V_19,
    V_20,
    V_21,
    V_22,
    V_23,
    V_24,
    V_25,
    V_26,
    V_27,
    V_28,
    V_29,
    V_30
    //...
};


std::optional<VariableTypeIdx> rgba_2_VariableTypeIdx( RGBA rgba_ )noexcept;


VariableTypeIdx str_2_variableTypeIdx( const std::string &name_ )noexcept;




}//--------------------- namespace: blueprint end ------------------------//
#endif 

