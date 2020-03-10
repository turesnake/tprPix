/*
 * =================== BlueprintVarType.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "BlueprintVarType.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace bpv_inn {//------------------ namespace: bpv_inn start ---------------------//

    const std::map<VariableTypeIdx, RGBA> varTypeIdx_2_rgbas{
        { VariableTypeIdx::V_1, RGBA{ 255, 128, 128, 255 } }, // hsv: 0, 0.5, 1.0
        { VariableTypeIdx::V_2, RGBA{ 255,   0,   0, 255 } }, // hsv: 0, 1.0, 1.0
        { VariableTypeIdx::V_3, RGBA{ 128,   0,   0, 255 } }, // hsv: 0, 1.0, 0.5
        //--
        { VariableTypeIdx::V_4, RGBA{ 255, 191, 128, 255 } }, // hsv: 30, 0.5, 1.0
        { VariableTypeIdx::V_5, RGBA{ 255, 127,   0, 255 } }, // hsv: 30, 1.0, 1.0
        { VariableTypeIdx::V_6, RGBA{ 128,  64,   0, 255 } }, // hsv: 30, 1.0, 0.5
        //--
        { VariableTypeIdx::V_7, RGBA{ 255, 255, 128, 255 } }, // hsv: 60, 0.5, 1.0
        { VariableTypeIdx::V_8, RGBA{ 204, 204,   0, 255 } }, // hsv: 60, 1.0, 0.8
        { VariableTypeIdx::V_9, RGBA{ 102, 102,   0, 255 } }, // hsv: 60, 1.0, 0.4
        //--
        { VariableTypeIdx::V_10, RGBA{ 153, 255, 153, 255 } }, // hsv: 120, 0.4, 1.0
        { VariableTypeIdx::V_11, RGBA{   0, 255,   0, 255 } }, // hsv: 120, 1.0, 1.0
        { VariableTypeIdx::V_12, RGBA{   0, 102,   0, 255 } }, // hsv: 120, 1.0, 0.4
        //--
        { VariableTypeIdx::V_13, RGBA{   0, 255, 255, 255 } }, // hsv: 180, 1.0, 1.0
        { VariableTypeIdx::V_14, RGBA{   0, 153, 153, 255 } }, // hsv: 180, 1.0, 0.6
        { VariableTypeIdx::V_15, RGBA{   0,  77,  77, 255 } }, // hsv: 180, 1.0, 0.3
        //--
        { VariableTypeIdx::V_16, RGBA{   0, 170, 255, 255 } }, // hsv: 200, 1.0, 1.0
        { VariableTypeIdx::V_17, RGBA{   0, 102, 153, 255 } }, // hsv: 200, 1.0, 0.6
        { VariableTypeIdx::V_18, RGBA{   0,  60,  89, 255 } }, // hsv: 200, 1.0, 0.35
        //--
        { VariableTypeIdx::V_19, RGBA{ 128, 128, 255, 255 } }, // hsv: 240, 0.5, 1.0
        { VariableTypeIdx::V_20, RGBA{   0,   0, 255, 255 } }, // hsv: 240, 1.0, 1.0
        { VariableTypeIdx::V_21, RGBA{   0,   0, 102, 255 } }, // hsv: 240, 1.0, 0.4
        //--
        { VariableTypeIdx::V_22, RGBA{ 204, 153, 255, 255 } }, // hsv: 270, 0.4, 1.0
        { VariableTypeIdx::V_23, RGBA{ 128,   0, 255, 255 } }, // hsv: 270, 1.0, 1.0
        { VariableTypeIdx::V_24, RGBA{  64,   0, 128, 255 } }, // hsv: 270, 1.0, 0.5
        //--
        { VariableTypeIdx::V_25, RGBA{ 255, 166, 255, 255 } }, // hsv: 300, 0.35, 1.0
        { VariableTypeIdx::V_26, RGBA{ 178,   0, 179, 255 } }, // hsv: 300, 1.0, 0.7
        { VariableTypeIdx::V_27, RGBA{ 102,   0, 102, 255 } }, // hsv: 300, 1.0, 0.4
        //--
        { VariableTypeIdx::V_28, RGBA{ 255,  76, 166, 255 } }, // hsv: 330, 0.7, 1.0
        { VariableTypeIdx::V_29, RGBA{ 204,   0, 102, 255 } }, // hsv: 330, 1.0, 0.8
        { VariableTypeIdx::V_30, RGBA{ 102,   0,  51, 255 } } // hsv: 330, 1.0, 0.4
    };





}//--------------------- namespace: bpv_inn end ------------------------//



std::optional<VariableTypeIdx> rgba_2_VariableTypeIdx( RGBA rgba_ )noexcept{

    RGBA rgba = rgba_;
    rgba.a = 255; // ignore alpha 

    // 最简单最直接的方案，完全不考虑性能 
    for( const auto & [iKey, iVal] : bpv_inn::varTypeIdx_2_rgbas ){ // each color
        if( rgba.is_near(iVal, 8) ){
            return { iKey };
        }
    }
    // not find
    // 要么是 辅助色，要么是颜色出错了，一切留给 caller 处理
    return std::nullopt;
}



VariableTypeIdx str_2_variableTypeIdx( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<VariableTypeIdx>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find VariableTypeIdx: {}", name_ );
        tprAssert(0);
        return VariableTypeIdx::V_1; // never reach
    }
}


}//--------------------- namespace: blueprint end ------------------------//

