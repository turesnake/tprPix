/*
 * ===================== BioSoupColor.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.10
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/bioSoup/BioSoupColor.h"


namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------
namespace color_inn {//------------- namespace color_inn ----------------

    //==============================================//
    //               base colors 
    //==============================================//
    // 蛋黄色 1
    const Color baseColor_yolk_1 {
        rgba_2_floatVec3(RGBA{ 215, 191, 135, 255 }),
        rgba_2_floatVec3(RGBA{ 193, 165, 120, 255 }),
        rgba_2_floatVec3(RGBA{ 173, 141, 105, 255 })
    };
    // 抹茶色 1
    const Color baseColor_matcha_1 {
        rgba_2_floatVec3(RGBA{ 186, 186, 134, 255 }),
        rgba_2_floatVec3(RGBA{ 163, 159, 121, 255 }),
        rgba_2_floatVec3(RGBA{ 145, 136, 106, 255 })
    };
    // daefForest 中 松树干 的黑色
    const Color baseColor_dark_1 {
        rgba_2_floatVec3(RGBA{ 112, 113, 101, 255 }),
        rgba_2_floatVec3(RGBA{  98,  98,  98, 255 }),
        rgba_2_floatVec3(RGBA{  77,  76,  70, 255 })
    };
    // 白杨树杆 的颜色
    const Color baseColor_poplarWhite_1 {
        rgba_2_floatVec3(RGBA{ 253, 247, 209, 255 }),
        rgba_2_floatVec3(RGBA{ 213, 207, 177, 255 }),
        rgba_2_floatVec3(RGBA{ 174, 173, 147, 255 })
    };


}//------------- namespace color_inn: end ----------------


//===== static =====//
std::unordered_map<std::string, std::unique_ptr<ColorNodeChain>>    ColorNodeChain::chains {};
const ColorNodeChain                                                *ColorNodeChain::currentChainPtr {nullptr};
size_t                                                              ColorNodeChain::nodeIdx {0};
Color                                                               ColorNodeChain::currentColor {};


// 
void ColorNodeChain::init_for_state()noexcept{

    auto [insertIt, insertBool] = ColorNodeChain::chains.emplace(
        "test", 
        new ColorNodeChain( //- can not use make_unique
            std::vector<ColorNode>{
            ColorNode{ &color_inn::baseColor_yolk_1,        0.01f },
            ColorNode{ &color_inn::baseColor_matcha_1,      0.03f },
            //ColorNode{ &color_inn::baseColor_poplarWhite_1, 0.03f },
            ColorNode{ &color_inn::baseColor_dark_1,        0.02f }
        })
    );
    tprAssert( insertBool );

    //...

    //---- bind default chain -----
    ColorNodeChain::bind_a_chain( "test" );


    ColorNodeChain::currentColor = color_inn::baseColor_yolk_1; // 人为定义一个 初始色

}


}//------------- namespace gameObjs::bioSoup: end ----------------

