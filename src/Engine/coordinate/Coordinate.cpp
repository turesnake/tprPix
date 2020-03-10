/*
 * ====================== Coordinate.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Coordinate.h"



void Coordinate::init()noexcept{

    // 在 inn坐标系内，正方形 四条边的 边向量
    std::unordered_map<NineDirection, glm::dvec2> lineVecs_in_innCoord {
        { NineDirection::Top,       glm::dvec2{ 1.0, 0.0 } },
        { NineDirection::Bottom,    glm::dvec2{ -1.0, 0.0 } },
        { NineDirection::Left,      glm::dvec2{ 0.0, 1.0 } },
        { NineDirection::Right,     glm::dvec2{ 0.0, -1.0 } }
    };

    for( const auto &[iDir, iVec] : lineVecs_in_innCoord ){
        glm::dvec2 lineVecOut = this->calc_outDPos( iVec ); // 边在 out坐标系 中的 向量
        glm::dvec2 normalVecOut { -lineVecOut.y, lineVecOut.x }; // 边的法向量（朝向立方体外侧）
        this->normalVecs_in_outCoord.insert( { iDir, normalVecOut } );
    }

    //--- rightHand ---
    this->rightHand = glm::normalize( this->calc_outDPos( glm::dvec2{ 1.0, 0.0 } ) );
}







