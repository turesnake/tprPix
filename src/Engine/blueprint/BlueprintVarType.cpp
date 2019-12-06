/*
 * =================== BlueprintVarType.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "BlueprintVarType.h"

//-------------------- CPP --------------------//
#include <unordered_map>
#include <map>


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace bpv_inn {//------------------ namespace: bpv_inn start ---------------------//

    const std::unordered_map<std::string, VariableTypeIdx> str_2_varTypeIdxs {
        { "V_1",    VariableTypeIdx::V_1 },
        { "V_2",    VariableTypeIdx::V_2 },
        { "V_3",    VariableTypeIdx::V_3 },
        { "V_4",    VariableTypeIdx::V_4 },
        { "V_5",    VariableTypeIdx::V_5 },
        { "V_6",    VariableTypeIdx::V_6 },
        { "V_7",    VariableTypeIdx::V_7 },
        { "V_8",    VariableTypeIdx::V_8 },
        { "V_9",    VariableTypeIdx::V_9 },
        { "V_10",   VariableTypeIdx::V_10 },
        { "V_11",   VariableTypeIdx::V_11 },
        { "V_12",   VariableTypeIdx::V_12 },
        { "V_13",   VariableTypeIdx::V_13 },
        { "V_14",   VariableTypeIdx::V_14 },
        { "V_15",   VariableTypeIdx::V_15 },
        { "V_16",   VariableTypeIdx::V_16 },
        { "V_17",   VariableTypeIdx::V_17 },
        { "V_18",   VariableTypeIdx::V_18 },
        { "V_19",   VariableTypeIdx::V_19 },
        { "V_20",   VariableTypeIdx::V_20 },
        { "V_21",   VariableTypeIdx::V_21 },
        { "V_22",   VariableTypeIdx::V_22 },
        { "V_23",   VariableTypeIdx::V_23 },
        { "V_24",   VariableTypeIdx::V_24 },
        { "V_25",   VariableTypeIdx::V_25 },
        { "V_26",   VariableTypeIdx::V_26 },
        { "V_27",   VariableTypeIdx::V_27 },
        { "V_28",   VariableTypeIdx::V_28 },
        { "V_29",   VariableTypeIdx::V_29 },
        { "V_30",   VariableTypeIdx::V_30 }
    };



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
    for( const auto &ipair : bpv_inn::varTypeIdx_2_rgbas ){ // each color
        if( rgba.is_near(ipair.second, 8) ){

            return { ipair.first };
        }
    }
    // not find
    // 要么是 辅助色，要么是颜色出错了，一切留给 caller 处理
    return std::nullopt;
}



VariableTypeIdx str_2_variableTypeIdx( const std::string &name_ )noexcept{
    tprAssert( bpv_inn::str_2_varTypeIdxs.find(name_) != bpv_inn::str_2_varTypeIdxs.end() );
    return bpv_inn::str_2_varTypeIdxs.at(name_);
}


}//--------------------- namespace: blueprint end ------------------------//

