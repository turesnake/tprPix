/*
 * ===================== calc_colliPoints.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.17
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "calc_colliPoints.h"

//-------------------- Engine --------------------//
#include "ColliderType.h"


namespace calc_colli_inn {//----------- namespace: calc_colli_inn -------------//

    //======= circular =========//
    const std::vector<glm::dvec2> scales_xy { //- 
        glm::dvec2{  1.0,  0.0 },
        glm::dvec2{ -1.0,  0.0 },
        glm::dvec2{  0.0,  1.0 },
        glm::dvec2{  0.0, -1.0 }
    };

    const std::vector<glm::dvec2> scales_45_degree { //- 
        glm::dvec2{  0.8,  0.8 },
        glm::dvec2{  0.8, -0.8 },
        glm::dvec2{ -0.8,  0.8 },
        glm::dvec2{ -0.8, -0.8 }
    };

    const std::vector<glm::dvec2> scales_2m2_oth { //- 
        //-- 将 2m2圆每个象限 分为 4份 --
        glm::dvec2{  0.9,  0.4 },
        glm::dvec2{  0.9, -0.4 },
        glm::dvec2{ -0.9,  0.4 },
        glm::dvec2{ -0.9, -0.4 },
        //---
        glm::dvec2{  0.4,  0.9 },
        glm::dvec2{  0.4, -0.9 },
        glm::dvec2{ -0.4,  0.9 },
        glm::dvec2{ -0.4, -0.9 }
    };

    const std::vector<glm::dvec2> scales_3m3_oth { //- 
        //-- 将 3m3圆每个象限 分为 5份 --
        glm::dvec2{  0.95,  0.31 },
        glm::dvec2{  0.95, -0.31 },
        glm::dvec2{ -0.95,  0.31 },
        glm::dvec2{ -0.95, -0.31 },
        //---
        glm::dvec2{  0.31,  0.95 },
        glm::dvec2{  0.31, -0.95 },
        glm::dvec2{ -0.31,  0.95 },
        glm::dvec2{ -0.31, -0.95 },
        //---
        glm::dvec2{  0.81,  0.59 },
        glm::dvec2{  0.81, -0.59 },
        glm::dvec2{ -0.81,  0.59 },
        glm::dvec2{ -0.81, -0.59 },
        //---
        glm::dvec2{  0.59,  0.81 },
        glm::dvec2{  0.59, -0.81 },
        glm::dvec2{ -0.59,  0.81 },
        glm::dvec2{ -0.59, -0.81 }
    };

    std::vector<glm::dvec2> colliPointScales_cir_1m1 {};
    std::vector<glm::dvec2> colliPointScales_cir_2m2 {};
    std::vector<glm::dvec2> colliPointScales_cir_3m3 {};

    //-- 移动碰撞检测时，所有 dogo 使用统一半径的 碰撞体
    std::vector<glm::dvec2> colliPointDPosOffs_for_cirDogo {};


    bool isInit {false};

    //----- funcs -----//
    void init();


}//--------------- namespace: calc_colli_inn end -----------------//


const std::vector<glm::dvec2> &get_colliPointDPosOffsRef_for_cirDogo()noexcept{
    return calc_colli_inn::colliPointDPosOffs_for_cirDogo;
}


/* ===========================================================
 *              calc_colliPoints_for_circular
 * -----------------------------------------------------------
 * 现在，dogo 改用了统一尺寸的 移动碰撞半径，但每个 go 携带的这套 碰撞点信息 仍然有效
 * 它将用于，将移动后的 go，登记到 mapent 上
 */
void calc_colliPoints_for_circular(  std::vector<glm::dvec2> &container_, double radius_ ){

    if( !calc_colli_inn::isInit ){
        calc_colli_inn::isInit = true;
        calc_colli_inn::init();
    }

    tprAssert( radius_ > 0.0 );

    double radius = radius_ + 10.0; //- 适当向外延伸 
    double scale  = radius / PIXES_PER_MAPENT_D;

    container_.clear();

    if( scale <= 1.0 ){
        for( const auto &i : calc_colli_inn::colliPointScales_cir_1m1 ){
            container_.push_back( i * radius );
        }
    }else if( scale <= 2.0 ){
        for( const auto &i : calc_colli_inn::colliPointScales_cir_2m2 ){
            container_.push_back( i * radius );
        }
    }else if( scale <= 3.0 ){
        for( const auto &i : calc_colli_inn::colliPointScales_cir_3m3 ){
            container_.push_back( i * radius );
        }
    }else{
        tprAssert(0); // not support jet
    }
}



namespace calc_colli_inn {//----------- namespace: calc_colli_inn -------------//

void init(){

    //----- colliPointScales_cir_1m1 ------//
    calc_colli_inn::colliPointScales_cir_1m1.emplace_back(  0.0, 0.0  );
    calc_colli_inn::colliPointScales_cir_1m1.insert( calc_colli_inn::colliPointScales_cir_1m1.end(),
                                                    calc_colli_inn::scales_xy.cbegin(), 
                                                    calc_colli_inn::scales_xy.cend() );

    calc_colli_inn::colliPointScales_cir_1m1.insert( calc_colli_inn::colliPointScales_cir_1m1.end(),
                                                    calc_colli_inn::scales_45_degree.cbegin(), 
                                                    calc_colli_inn::scales_45_degree.cend() );

    //----- colliPointScales_cir_2m2 ------//
    //-- 直接继承 2m2 的数据，缩小为 1/2 ----
    for( const auto &i : calc_colli_inn::colliPointScales_cir_1m1 ){
        calc_colli_inn::colliPointScales_cir_2m2.push_back( i * 0.5 );
    }

    //-- 再添加 2m2 特有的 外层一圈 --
    calc_colli_inn::colliPointScales_cir_2m2.insert( calc_colli_inn::colliPointScales_cir_2m2.end(),
                                                    calc_colli_inn::scales_xy.cbegin(), 
                                                    calc_colli_inn::scales_xy.cend() );

    calc_colli_inn::colliPointScales_cir_2m2.insert( calc_colli_inn::colliPointScales_cir_2m2.end(),
                                                    calc_colli_inn::scales_45_degree.cbegin(), 
                                                    calc_colli_inn::scales_45_degree.cend() );

    calc_colli_inn::colliPointScales_cir_2m2.insert( calc_colli_inn::colliPointScales_cir_2m2.end(),
                                                    calc_colli_inn::scales_2m2_oth.cbegin(), 
                                                    calc_colli_inn::scales_2m2_oth.cend() );

    //----- colliPointScales_cir_3m3 ------//
    //-- 直接继承 2m2 的数据，缩小为 2/3 ----
    for( const auto &i : calc_colli_inn::colliPointScales_cir_2m2 ){
        calc_colli_inn::colliPointScales_cir_3m3.push_back( i * 0.66 );
    }

    //-- 再添加 3m3 特有的 外层一圈 --
    calc_colli_inn::colliPointScales_cir_3m3.insert( calc_colli_inn::colliPointScales_cir_3m3.end(),
                                                    calc_colli_inn::scales_xy.cbegin(), 
                                                    calc_colli_inn::scales_xy.cend() );

    calc_colli_inn::colliPointScales_cir_3m3.insert( calc_colli_inn::colliPointScales_cir_3m3.end(),
                                                    calc_colli_inn::scales_3m3_oth.cbegin(), 
                                                    calc_colli_inn::scales_3m3_oth.cend() );


    //=== colliPointDPosOffs_for_cirDogo ===//
    calc_colliPoints_for_circular( calc_colli_inn::colliPointDPosOffs_for_cirDogo, Circular::radius_for_dogo );

}

}//--------------- namespace: calc_colli_inn end -----------------//











