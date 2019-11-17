/*
 * ==================== ColliDataFromJ.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ColliDataFromJ.h"

//------------------- Engine --------------------//
#include "esrc_customCoord.h"


//===== static =====//
glm::dvec2              ColliDataFromJ_Nil::emptyDvec2 {};
std::vector<glm::dvec2> ColliDataFromJ_Nil::emptyVec   {};

//---
glm::dvec2  ColliDataFromJ_Circular::emptyDvec2 {};


//---
glm::dvec2  ColliDataFromJ_Square::emptyDvec2 {};
bool        ColliDataFromJ_Square::isStaticInit {false};
std::vector<glm::dvec2> ColliDataFromJ_Square::colliPointDPosOffs {};



void ColliDataFromJ_Square::init_for_static()noexcept{

    const auto &artifactCoordRef = esrc::get_artifactCoordRef();

    glm::dvec2 innDPos {};
    glm::dvec2 outDPos {};

    ColliDataFromJ_Square::colliPointDPosOffs.clear();

    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){
            innDPos.x = i * Square::unifiedRadius;
            innDPos.y = j * Square::unifiedRadius;
            
            outDPos = artifactCoordRef.calc_outDPos( innDPos );
            ColliDataFromJ_Square::colliPointDPosOffs.push_back( outDPos );
        }
    }
}






