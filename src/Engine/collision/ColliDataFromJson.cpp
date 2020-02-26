/*
 * ==================== ColliDataFromJson.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ColliDataFromJson.h"

//------------------- Engine --------------------//
#include "calc_colliPoints.h"

#include "esrc_coordinate.h"

#include "tprDebug.h"


//===== static =====//
std::vector<glm::dvec2> ColliDataFromJson_Nil::emptyVec   {};


std::unique_ptr<ColliDataFromJson> ColliDataFromJson_Nil::nillInstance = std::make_unique<ColliDataFromJson_Nil>();

//--- circular ---

//--- square ---
bool        ColliDataFromJson_Square::isStaticInit {false};
std::vector<glm::dvec2> ColliDataFromJson_Square::colliPointDPosOffs {};



void ColliDataFromJson_Square::init_for_static()noexcept{

    const auto &worldCoordRef = esrc::get_worldCoordRef();

    glm::dvec2 innDPos {};
    glm::dvec2 outDPos {};

    ColliDataFromJson_Square::colliPointDPosOffs.clear();

    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){
            innDPos.x = i * Square::unifiedRadius;
            innDPos.y = j * Square::unifiedRadius;
            
            outDPos = worldCoordRef.calc_outDPos( innDPos );
            ColliDataFromJson_Square::colliPointDPosOffs.push_back( outDPos );
        }
    }
}






