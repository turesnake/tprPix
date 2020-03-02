/*
 * ========================= RenderLayerType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   render layer
 * ----------------------------
 */
#ifndef TPR_RENDER_LAYER_H
#define TPR_RENDER_LAYER_H

#include <string>

// from zFar -> zNear 
enum class RenderLayerType{

    Ground,
    GroundGo,
    Floor,

    BioSoup, // 深度区间 和 MajorGoes 一样大

    GoShadows,
    Debug,

    MajorGoes,

    AboveMajorGoes,
    //....
    UIs
};


std::string renderLayerType_2_str( RenderLayerType dir_ )noexcept;
RenderLayerType str_2_renderLayerType( const std::string &str_ )noexcept;



#endif

