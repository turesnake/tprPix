/*
 * ==================== ColliDataFromJ.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ColliDataFromJ.h"

//------------------- Engine --------------------//
#include "calc_colliPoints.h"

#include "esrc_coordinate.h"


//===== static =====//
glm::dvec2              ColliDataFromJ_Nil::emptyDvec2 {};
std::vector<glm::dvec2> ColliDataFromJ_Nil::emptyVec   {};


//--- circular ---
double      ColliDataFromJ_Circular::moveColliRadius_for_dogo {24.0}; // Must small than HALF_PIXES_PER_MAPENT
glm::dvec2  ColliDataFromJ_Circular::emptyDvec2 {};
bool        ColliDataFromJ_Circular::isStaticInit {false};
std::vector<glm::dvec2> ColliDataFromJ_Circular::colliPointDPosOffs_for_dogo {};


//--- square ---
glm::dvec2  ColliDataFromJ_Square::emptyDvec2 {};
bool        ColliDataFromJ_Square::isStaticInit {false};
std::vector<glm::dvec2> ColliDataFromJ_Square::colliPointDPosOffs {};



void ColliDataFromJ_Circular::init_for_static()noexcept{
    calc_colliPoints_for_circular(  ColliDataFromJ_Circular::colliPointDPosOffs_for_dogo, 
                                    ColliDataFromJ_Circular::moveColliRadius_for_dogo );
}



void ColliDataFromJ_Square::init_for_static()noexcept{

    const auto &worldCoordRef = esrc::get_worldCoordRef();

    glm::dvec2 innDPos {};
    glm::dvec2 outDPos {};

    ColliDataFromJ_Square::colliPointDPosOffs.clear();

    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){
            innDPos.x = i * Square::unifiedRadius;
            innDPos.y = j * Square::unifiedRadius;
            
            outDPos = worldCoordRef.calc_outDPos( innDPos );
            ColliDataFromJ_Square::colliPointDPosOffs.push_back( outDPos );
        }
    }
}






