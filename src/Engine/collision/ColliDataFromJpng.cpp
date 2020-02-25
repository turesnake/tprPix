/*
 * ==================== ColliDataFromJpng.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ColliDataFromJpng.h"

//------------------- Engine --------------------//
#include "calc_colliPoints.h"

#include "esrc_coordinate.h"

#include "tprDebug.h"


//===== static =====//
std::vector<glm::dvec2> ColliDataFromJpng_Nil::emptyVec   {};


std::unique_ptr<ColliDataFromJpng> ColliDataFromJpng_Nil::nillInstance = std::make_unique<ColliDataFromJpng_Nil>();

//--- circular ---

//--- square ---
bool        ColliDataFromJpng_Square::isStaticInit {false};
std::vector<glm::dvec2> ColliDataFromJpng_Square::colliPointDPosOffs {};



void ColliDataFromJpng_Square::init_for_static()noexcept{

    const auto &worldCoordRef = esrc::get_worldCoordRef();

    glm::dvec2 innDPos {};
    glm::dvec2 outDPos {};

    ColliDataFromJpng_Square::colliPointDPosOffs.clear();

    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){
            innDPos.x = i * Square::unifiedRadius;
            innDPos.y = j * Square::unifiedRadius;
            
            outDPos = worldCoordRef.calc_outDPos( innDPos );
            ColliDataFromJpng_Square::colliPointDPosOffs.push_back( outDPos );
        }
    }
}






